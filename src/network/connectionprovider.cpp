#include "network/connectionprovider.h"

#include <iostream>
#include <string>
#include <functional>

namespace net
{

ConnectionProvider::ConnectionProvider()
{
    m_log = logs::Log::create("ConnectionProvider");
}

void ConnectionProvider::launch()
{
    m_contextThread = std::thread([this] { m_ioContext.run(); });
}

void ConnectionProvider::stop()
{
    m_ioContext.stop();
    m_contextThread.join();
}

void ConnectionProvider::startSession(std::string const& host, int port, onConnect&& handler)
{
    auto session = std::make_shared<Session>(m_ioContext, host, port);
    session->connect(std::move(handler));
}

} // namespace net

