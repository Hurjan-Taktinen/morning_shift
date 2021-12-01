#pragma once

#include "logs/log.h"

#include <string>
#include <thread>
#include <iostream>

namespace mc
{

class Client final
{
public:
    Client(const std::string& addr, const int port, const std::string& name);
    void start();
    void join();

    void run();

private:
    void connect();
    void login();
    void play();

    std::string m_name;
    std::string m_addr;
    int m_port = 0;

    std::thread m_threadi;

    logs::Logger m_loki;
};
} // namespace mc
