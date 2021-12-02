#ifndef MC_CONNECTION_H
#define MC_CONNECTION_H

#include <asio.hpp>
#include <memory>
#include <optional>

namespace mc
{

class Connection;
using ConnectionPtr = std::shared_ptr<Connection>;

class Connection final
{
public:
    static ConnectionPtr create(const std::string& addr, const int port)
    {
        auto thisPtr = std::make_shared<Connection>();
        thisPtr->connect(addr, port);
        return thisPtr;
    }

    bool send(const std::vector<uint8_t>&) { return true; }
    bool receive()
    {
        // get length + rawdata to packet
        // packet.resize(5); // max size of varInt
        // size_t packetSize = getLen(fd, packet);
        //// std::cout << "succesfully read packet length=" << packetSize
        //// << std::endl;
        // if(packetSize <= 0)
        // return false;
        //// std::cout << "packet size=" << packet.size() << " raw[" <<
        //// packet
        //// <<
        //// "]" <<  std::endl;

        // size_t receivedCount = 0;
        // size_t offset = packet.size();

        // packet.resize(offset + packetSize);

        // while(receivedCount < packetSize)
        //{
        // receivedCount +=
        // recv(fd,
        // packet.data() + offset + receivedCount,
        // packetSize - receivedCount);
        //}
        // return true;
        return false;
    }

private:
    void connect(const std::string& addr, const unsigned short port)
    {
        using asio::ip::tcp;

        asio::io_context io_context;
        tcp::resolver resolver(io_context);
        //auto endpoints = resolver.resolve(addr, std::to_string(port));
        auto ip = asio::ip::address::from_string(addr);
        asio::ip::tcp::endpoint endpoint{ip, port};

        socket = tcp::socket(io_context);

        socket->connect(endpoint);
    }

    size_t getLen()
    {
        // uint32_t tmp = 0;

        // int i = 0;
        // for(i = 0; i < 5; ++i)
        //{
        // if(recv(fd, packet.data() + i, sizeof(uint8_t)) != 1)
        // return 0;

        // uint32_t c = static_cast<uint8_t>(*(packet.data() + i));

        // tmp |= ((c & 0x7F) << 7 * i);

        // if(!(c & 0x80))
        // break;
        //}
        //// std::cout << "i=" << i+1 << std::endl;
        // packet.resize(i + 1);
        // return tmp;
        return 0;
    }

private:
    std::optional<asio::ip::tcp::socket> socket;
};
} // namespace mc
#endif
