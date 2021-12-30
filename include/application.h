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

private:
    void init();
    void mainloop();

    std::unique_ptr<net::ConnectionProvider> m_connectionProvider;
    logs::Logger m_log;
    mc::ClientPtr m_client;

    // Total runtime of application
    float m_apprunTime = 0.0f;

    // Total time taken by one cycle of mainloop
    float m_frameTime = 0.0f;

    uint64_t m_frameCounter = 0;

};

} // namespace app
