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
    auto conf = config::Config::getConnectionConfig();

    for(int i = 0; i < 1; ++i)
    {
        m_clients.emplace_back(mc::Client::create("Kone-Person-" + std::to_string(i)));
        auto& client = m_clients.back();

        m_connectionProvider->startSession(
                conf.server_ip,
                conf.server_port,
                [client = client](std::shared_ptr<net::Session> session) {
                    client->start(std::move(session));
                });
    }

    // Launch after setting connections to clients
    m_connectionProvider->launch();
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

    while(!m_clients.empty())
    {
        utils::Timer timer;

        for(auto const& client : m_clients)
        {
            client->update();
        }

        std::this_thread::sleep_for(30ms);

        m_frameTime = timer.elapsed();
        m_apprunTime += m_frameTime;
        m_frameCounter += 1;

        m_clients.erase(
                m_clients.begin(),
                std::remove_if(m_clients.begin(), m_clients.end(), [](auto& client) {
                    return client->aliveCheck();
                }));
    }
}

} // namespace app
