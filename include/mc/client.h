#pragma once

#include "logs/log.h"
#include "network/session.h"

#include <boost/lexical_cast/try_lexical_convert.hpp>
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
    static ClientPtr create(const std::string& name);

    virtual ~Client() = default;
    Client() = default;

    Client(Client const&) = delete;
    Client& operator=(const Client&) = delete;

    Client(Client&&) noexcept = default;
    Client& operator=(Client&& rhs) noexcept = default;

    virtual void start(std::shared_ptr<net::Session> session) = 0;
    virtual void update() = 0;
};
} // namespace mc
