#include "clientimpl.h"
#include "mc/stringarchive.h"

#include <cassert>

namespace mc
{

ClientPtr Client::create(const std::string& name)
{
    auto clientPtr = std::make_shared<ClientImpl>(name);
    clientPtr->init(clientPtr);
    return std::dynamic_pointer_cast<Client>(clientPtr);
}

ClientImpl::ClientImpl(std::string name) : m_name(std::move(name))
{
    m_loki = logs::Log::create("Client\"" + m_name + "\"");
    m_loki->info("created new client");
    m_apper.init(m_loki);
}

void ClientImpl::init(OwnerCbsPtr const& thisPtr)
{
    m_stack.init(thisPtr);
}

void ClientImpl::start(std::shared_ptr<net::Session> session)
{
    m_session = std::move(session);

    m_session->start(
            [this](std::vector<uint8_t>&& data) { m_incomingMessageQueue.push(std::move(data)); },
            [this](std::string const& errorMessage) { handleDisconnect(errorMessage); });
    m_stack.setConnection(m_session);

    m_state = State::CONNECTED;
    login();
}

void ClientImpl::update()
{
    while(!m_incomingMessageQueue.empty())
    {
        auto msg = m_incomingMessageQueue.front();
        m_incomingMessageQueue.pop();

        StringArchive packet(std::move(msg));
        // m_loki->info("raw[{}]", packet.str());
        m_stack.handlePacket(packet);
    }
}

bool ClientImpl::aliveCheck() const
{
    return m_state != State::DISCONNECTED;
}

void ClientImpl::handleDisconnect(const std::string& reason)
{
    m_state = State::DISCONNECTED;
    m_loki->info("Disconnected {}", reason);
}

void ClientImpl::login()
{
    m_state = State::LOGIN;
    m_loki->info("logging in as {}", m_name);

    mc::HandshakeMsg handshake(
            {mc::ProtocolVersion, m_session->getAddress(), m_session->getPort(), 0x02});
    m_stack.send(handshake);
    mc::LoginStartMsg loginStart{m_name};
    m_stack.send(loginStart);
}

void ClientImpl::handleMessage(const mc::LoginSuccessMsg& msg)
{
    auto [uuid, name] = msg.data();

    m_loki->info("logged in as {} UUID={}", name.str(), uuid.print());
    m_state = State::PLAY;
}

void ClientImpl::handleMessage(const mc::ChunkDataMsg& msg)
{
    m_apper.update(msg);
}

void ClientImpl::handleMessage(const mc::BlockChangeMsg& msg)
{
    m_apper.update(msg);
}

} // namespace mc
