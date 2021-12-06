#include "messages.h"
#include "messageids.h"
#include "cartographer.h"

#include "mc/client.h"
#include "mc/connection.h"
#include "mc/messagestack.h"

#include <string>
#include <thread>

namespace mc
{

class ClientImpl final : public Client, public OwnerCbs
{
public:
    ClientImpl(const std::string& addr, int port, const std::string& name);

    void start() override;
    void join() override;

    void handleDisconnect(const std::string& reason) override
    {
        _state = DISCONNECTED;
        m_loki->info("Disconnected {}", reason);
    };

    void init(OwnerCbsPtr thisPtr) { m_stack.init(thisPtr); }

    void handleMessage(const mc::LoginSuccessMsg&) override;
    void handleMessage(const mc::ChunkDataMsg&) override;
    void handleMessage(const mc::BlockChangeMsg&) override;

private:
    void run();
    void connect();
    void login();
    void main();

    std::string m_name;
    std::string m_addr;
    int m_port = 0;

    mc::ConnectionPtr m_conn;
    MessageStack m_stack;

    std::thread m_thread;
    logs::Logger m_loki;

    Cartographer m_apper;
};

ClientPtr Client::create(const std::string& addr, const int port, const std::string& name)
{
    auto clientPtr = std::make_shared<ClientImpl>(addr, port, name);
    clientPtr->init(clientPtr);
    return std::dynamic_pointer_cast<Client>(clientPtr);
}

ClientImpl::ClientImpl(const std::string& addr, int port, const std::string& name) :
    m_name(name), m_addr(addr), m_port(port)
{
    m_loki = logs::Log::create("Client\"" + m_name + "\"");
    m_loki->info("created new client");
    m_apper.init(m_loki);
}

void ClientImpl::start()
{
    m_thread = std::thread(&ClientImpl::run, this);
}
void ClientImpl::join()
{
    m_thread.join();
}

void ClientImpl::run()
{
    connect();
    login();
    main();
}

void ClientImpl::connect()
{
    if(_state == DISCONNECTED)
    {
        m_loki->info("connecting {}:{}", m_addr, m_port);

        m_conn = Connection::create(m_addr, m_port);
        if(m_conn)
        {
            m_loki->info("connected {}:{}", m_addr, m_port);
            m_stack.setConnection(m_conn);
            _state = CONNECTED;
        }
    }
}

void ClientImpl::login()
{
    if(_state == CONNECTED)
    {
        _state = LOGIN;
        m_loki->info("logging in as {}", m_name);

        mc::HandshakeMsg handshake({mc::ProtocolVersion, m_addr, m_port, 0x02});
        m_stack.send(handshake);
        mc::LoginStartMsg loginStart{m_name};
        m_stack.send(loginStart);

        for(;;)
        {
            StringArchive packet;
            if(m_conn->receive(packet))
            {
                // m_loki->info("raw[{}]", packet.str());
                m_stack.handlePacket(packet);
            }
        }
    }
}

void ClientImpl::main()
{
    for(;;)
    {
        // do things like
        StringArchive packet;
        while(m_conn->receive(packet))
        {
            m_stack.handlePacket(packet);
        }
    }
}

void ClientImpl::handleMessage(const mc::LoginSuccessMsg&)
{
    _state = PLAY;
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
