#pragma once

#include "mc/client.h"
#include "network/connectionprovider.h"
#include "logs/log.h"

#include <memory>

namespace app
{

class Application final
{
public:
    void run();
    ~Application();
    Application() = default;
    Application(Application const&) = default;
    Application(Application &&) = default;
    Application& operator=(Application const&) = default;
    Application& operator=(Application &&) = default;

private:
    void init();
    void mainloop();

    std::unique_ptr<net::ConnectionProvider> m_connectionProvider;
    logs::Logger m_log;

    std::vector<mc::ClientPtr> m_clients;

    // Total runtime of application
    float m_apprunTime = 0.0F;

    // Total time taken by one cycle of mainloop
    float m_frameTime = 0.0F;

    uint64_t m_frameCounter = 0;

};

} // namespace app
