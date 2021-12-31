#pragma once

#include "asio.hpp"
#include "logs/log.h"
#include "mc/clientimpl.h"
#include "network/session.h"

#include <functional>
#include <memory>
#include <string>
#include <thread>

namespace net
{

class ConnectionProvider final
{
public:
    ConnectionProvider();
    void launch();

    void stop()
    {
        m_ioContext.stop();
        m_contextThread.join();
    }

    void startSession(std::string const& host, int port, onConnect&& handler);

private:
    logs::Logger m_log;
    asio::io_context m_ioContext;
    std::thread m_contextThread;
};

} // namespace net
