#include "network/session.h"

#include <cstddef>
#include <cstdint>
#include <sstream>
#include <sys/types.h>

#include "utils/mc.h"

namespace net
{

void Session::start(ReceiveHandler&& receiveHandler, ErrorHandler&& errorHandler)
{
    m_onReceive = std::move(receiveHandler);
    m_onError = std::move(errorHandler);
    asyncRead();
}

void Session::send(std::vector<uint8_t>&& msg)
{
    asio::post(m_context, [this, msg = std::move(msg)]() mutable {
        auto idle = m_outgoing.empty();
        m_outgoing.push(std::move(msg));

        if(idle)
        {
            asyncWrite();
        }
    });
}

void Session::asyncRead()
{
    header.clear();
    header.resize(3);
    body.clear();

    asio::async_read(
            m_socket,
            asio::buffer(header),
            [self = shared_from_this()](asio::error_code error, std::size_t bytesTransferred) {
                self->onReadHeader(error, bytesTransferred);
            });
}

void Session::asyncWrite()
{
    asio::async_write(
            m_socket,
            asio::buffer(m_outgoing.front()),
            [this, self = shared_from_this()](
                    asio::error_code error, std::size_t /*bytesTransferred*/) {
                if(!error)
                {
                    m_outgoing.pop();
                    if(!m_outgoing.empty())
                    {
                        asyncWrite();
                    }
                }
                else
                {
                    m_onError(error.message() + "| FROM asyncWrite");
                    m_socket.close();
                }
            });
}

void Session::onReadHeader(asio::error_code error, std::size_t bytesTransferred)
{
    if(!error)
    {
        auto const [bodySize, bytes] = mc::getLength(header);
        headerSize = bytes;

        // Resize body-buffer for remaining bytes. Header may already contain 1-2 bytes of data
        // belonging to body
        body.resize(bodySize - (header.size() - bytes));

        // ASYNC read for rest of the body
        asio::async_read(
                m_socket,
                asio::buffer(body),
                [self = shared_from_this()](asio::error_code error, std::size_t bytesTransferred) {
                    self->onReadBody(error, bytesTransferred);
                });
    }
    else
    {
        m_onError(
                error.message() + "| FROM onReadHeader | READ " + std::to_string(bytesTransferred)
                + " bytes");
        m_socket.close(error);
    }
}

void Session::onReadBody(asio::error_code error, std::size_t /*bytesTransferred*/)
{
    if(!error)
    {
        std::vector<uint8_t> payload;
        payload.reserve(header.size() - headerSize + body.size());
        payload.insert(payload.end(), header.begin() + headerSize, header.end());
        payload.insert(payload.end(), body.begin(), body.end());

        m_onReceive(std::move(payload));

        asyncRead();
    }
    else
    {
        m_onError(error.message() + "| FROM onReadBody");
        m_socket.close(error);
    }
}

void Session::onWrite(asio::error_code error, std::size_t /*unused*/)
{
    if(!error)
    {
        m_outgoing.pop();
        if(!m_outgoing.empty())
        {
            asyncWrite();
        }
    }
    else
    {
        m_onError(error.message() + "| FROM onWrite");
        m_socket.close();
    }
}

std::string Session::getAddress() const
{
    return m_socket.remote_endpoint().address().to_string();
}

int Session::getPort() const
{
    return m_socket.remote_endpoint().port();
}

} // namespace net
