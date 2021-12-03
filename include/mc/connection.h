#ifndef MC_CONNECTION_H
#define MC_CONNECTION_H

#pragma GCC system_header

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
    static ConnectionPtr create(const std::string& addr, uint16_t port)
    {
        auto thisPtr = std::make_shared<Connection>();
        thisPtr->connect(addr, port);
        return thisPtr;
    }

    bool send(const std::vector<uint8_t>& packet);
    bool receive(std::vector<uint8_t>& packet);

private:
    void connect(const std::string& addr, uint16_t port);
    size_t getLen(std::vector<uint8_t>& packet);

private:
    std::optional<asio::ip::tcp::socket> socket;
};
} // namespace mc
#endif
