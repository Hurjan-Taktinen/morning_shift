#pragma once

#include "messages.h"
#include "messageids.h"

#include "mc/cartographer.h"
#include "mc/client.h"
// #include "mc/connection.h"
#include "messagestack.h"
#include "network/tsdeque.h"

#include <string>
#include <vector>

namespace mc
{

class ClientImpl final : public Client, public OwnerCbs
{
public:
    explicit ClientImpl(std::string name);

    void start(std::shared_ptr<net::Session> session) override;
    void update() override;

    void handleDisconnect(const std::string& reason) override;
    void init(OwnerCbsPtr const& thisPtr);

private:
    void handleMessage(const mc::LoginSuccessMsg&) override;
    void handleMessage(const mc::ChunkDataMsg&) override;
    void handleMessage(const mc::BlockChangeMsg&) override;

    void login();
    void play();

    std::string m_name;

    // mc::ConnectionPtr m_conn;
    MessageStack m_stack;

    logs::Logger m_loki;

    Cartographer m_apper;
    std::shared_ptr<net::Session> m_session;
    net::TsQueue<std::vector<uint8_t>> m_incomingMessageQueue;
};

} // namespace mc
