#pragma once

#include "logs/log.h"

#include <string>
#include <thread>
#include <iostream>

namespace mc
{

class Client;
using ClientPtr = std::shared_ptr<Client>;

class Client
{
public:
    static ClientPtr create(const std::string& addr, const int port, const std::string& name);

    virtual ~Client() = default;
    virtual void start() = 0;
    virtual void join() = 0;
};
} // namespace mc
