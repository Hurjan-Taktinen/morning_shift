#pragma once

#include "asio.hpp"

#include <cstdint>
#include <functional>
#include <memory>
#include <iostream>
#include <utility>
#include <fmt/format.h>
// #include <queue>
#include "asio/ip/address.hpp"
#include "asio/ip/basic_endpoint.hpp"
#include "asio/read.hpp"
#include "network/tsdeque.h"

namespace mc
{
class Client;
}

namespace net
{

using ReceiveHandler = std::function<void(std::vector<uint8_t>)>;
using ErrorHandler = std::function<void(std::string const&)>;

class Session;
using onConnect = std::function<void(std::shared_ptr<Session>)>;

class Session final : public std::enable_shared_from_this<Session>
{
public:
    explicit Session(asio::io_context& ctx, std::string const& host, int port) :
        m_context(ctx),
        m_socket(ctx),
        m_endpoint(asio::ip::make_address(host), asio::ip::port_type(port))
    {
    }

    void connect(onConnect&& handler)
    {
        m_socket.async_connect(
                m_endpoint,
                [handler = std::move(handler), self = shared_from_this()](asio::error_code ec) {
                    if(!ec)
                    {
                        handler(self);
                    }
                });
    }

    void start(ReceiveHandler&& receiveHandler, ErrorHandler&& errorHandler);
    void send(std::vector<uint8_t>&& msg);

    [[nodiscard]] std::string getAddress() const;
    [[nodiscard]] int getPort() const;

private:
    void asyncRead();
    void asyncWrite();
    void onReadHeader(asio::error_code error, std::size_t bytesTransferred);
    void onReadBody(asio::error_code error, std::size_t bytesTransferred);
    void onWrite(asio::error_code error, std::size_t);

    ReceiveHandler m_onReceive;
    ErrorHandler m_onError;

    asio::io_context& m_context;
    asio::ip::tcp::socket m_socket;
    asio::ip::tcp::endpoint m_endpoint;
    asio::streambuf m_streambuf;

    std::vector<uint8_t> header;
    std::vector<uint8_t> body;
    int headerSize = 0;

    TsQueue<std::vector<uint8_t>> m_outgoing;
};

} // namespace net
