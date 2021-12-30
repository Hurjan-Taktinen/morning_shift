#include "network/connectionprovider.h"

#include <iostream>
#include <string>
#include <functional>

namespace net
{

void ConnectionProvider::init()
{
}

ConnectionProvider::ConnectionProvider()
{
    m_log = logs::Log::create("ConnectionProvider");
}

void ConnectionProvider::connectSession(std::string const& host, int port, onConnect&& handler)
{
    using namespace asio::ip;
    auto session = std::make_shared<Session>(m_ioContext, host, port);
    session->connect(std::move(handler));

    m_contextThread = std::thread([this] { m_ioContext.run(); });
}

} // namespace net

