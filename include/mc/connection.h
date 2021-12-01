#pragma once

#include <boost/asio.hpp>
#include <memory>

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

    bool send() { return true; }
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
    void connect(const std::string& addr, const int port)
    {

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
};
} // namespace mc
