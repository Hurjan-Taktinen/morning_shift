#include "mc/connection.h"

#include <asio.hpp>
#include <memory>
#include <optional>
#include <iostream>

namespace mc
{

bool Connection::send(const std::vector<uint8_t>& packet)
{
    asio::error_code erska;
    socket->write_some(asio::buffer(packet), erska);

    if(erska)
        return false;

    return true;
}

bool Connection::receive(std::vector<uint8_t>& packet)
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
                asio::buffer(&packet.front() + offset + receivedCount, packetSize - receivedCount),
                error);

        if(error)
        {
            return false;
        }
    }

    return true;
}

void Connection::connect(const std::string& addr, const uint16_t port)
{
    using asio::ip::tcp;

    asio::io_context io_context;
    asio::error_code error;

    socket = tcp::socket(io_context);
    tcp::endpoint endpoint = asio::connect(
            *socket, tcp::resolver{io_context}.resolve(addr, std::to_string(port)), error);

    std::cout << error.message() << " Connected to " << endpoint << "\n";
}

size_t Connection::getLen(std::vector<uint8_t>& packet)
{
    asio::error_code error;

    uint32_t tmp = 0;

    int i = 0;
    for(i = 0; i < 5; ++i)
    {
        if(asio::read(*socket, asio::buffer(&packet.front() + i, 1), error) != 1 || error)
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

} // namespace mc
