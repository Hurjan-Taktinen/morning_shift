#include "mc/client.h"
#include <string>

namespace mc
{
Client::Client(const std::string& addr, int port, const std::string& name) :
    m_name(name), m_addr(addr), m_port(port)
{
    m_loki = logs::Log::create("Client\"" + m_name + "\"");
    m_loki->info("created new client");
}

void Client::start()
{
    m_threadi = std::thread(&Client::run, this);
}
void Client::join()
{
    m_threadi.join();
}

void Client::run()
{
    connect();
}

void Client::connect()
{
    m_loki->info("connecting {}:{}", m_addr, m_port);
}

void Client::login()
{
}

void Client::play()
{
}

} // namespace mc
