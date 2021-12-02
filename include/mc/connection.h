#ifndef MC_CONNECTION_H
#define MC_CONNECTION_H

#include <asio.hpp>
#include <memory>
#include <optional>
#include <iostream>

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

    bool send(const std::vector<uint8_t>& packet)
    {
        asio::error_code erska;
        socket->write_some(asio::buffer(packet), erska);

        if(erska)
            return false;

        return true;
    }

    bool receive(std::vector<uint8_t>& packet)
    {
        // get length + rawdata to packet
        packet.resize(5); // max size of varInt
        size_t packetSize = getLen(packet);
        if(packetSize <= 0)
            return false;

        // std::cout << "succesfully read packet length=" << packetSize << std::endl;

        size_t offset = packet.size();
        size_t receivedCount = 0;
        packet.resize(offset + packetSize);

        asio::error_code error;

        while(receivedCount < packetSize)
        {
            receivedCount = socket->read_some(
                    asio::buffer(
                            &packet.front() + offset + receivedCount, packetSize - receivedCount),
                    error);

            if(error)
            {
                return false;
            }
        }

        return true;
    }

private:
    void connect(const std::string& addr, const unsigned short port)
    {
        using asio::ip::tcp;

        asio::io_context io_context;
        tcp::resolver resolver(io_context);

        auto ip = asio::ip::address::from_string(addr);
        asio::ip::tcp::endpoint endpoint{ip, port};

        socket = tcp::socket(io_context);
        socket->connect(endpoint);
    }

    size_t getLen(std::vector<uint8_t>& packet)
    {
        asio::error_code error;

        uint32_t tmp = 0;

        int i = 0;
        for(i = 0; i < 5; ++i)
        {
            if(asio::read(*socket, asio::buffer(&packet.front() + i, 1), error) != 1)
            {
                return 0;
            }

            uint32_t c = packet.at(i);

            tmp |= ((c & 0x7F) << 7 * i);

            if(!(c & 0x80))
                break;
        }
        packet.resize(i + 1);
        return tmp;
    }

private:
    std::optional<asio::ip::tcp::socket> socket;
};
} // namespace mc
#endif
