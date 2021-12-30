#include "application.h"
#include "config/config.h"
#include "logs/log.h"
#include "utils/timer.h"

#include <chrono>
#include <thread>
#include <utility>

namespace app
{

Application::~Application()
{
    m_connectionProvider->stop();
}

void Application::init()
{
    m_log = logs::Log::create("Application");
    m_connectionProvider = std::make_unique<net::ConnectionProvider>();
    m_connectionProvider->init();
    auto conf = config::Config::getConnectionConfig();

    {
        m_client = mc::Client::create("Karen");
        m_connectionProvider->connectSession(
                conf.server_ip,
                conf.server_port,
                [client = m_client](std::shared_ptr<net::Session> session) {
                    client->start(std::move(session));
                });
    }
}

void Application::run()
{
    init();
    mainloop();
}

void Application::mainloop()
{
    m_log->info("Enter mainloop");
    using namespace std::chrono_literals;

    while(true)
    {
        utils::Timer timer;
        m_client->update();

        std::this_thread::sleep_for(30ms);

        m_frameTime = timer.elapsed();
        m_apprunTime += m_frameTime;
        m_frameCounter += 1;
    }
}

} // namespace app
