#ifndef MCSTACK_H
#define MCSTACK_H

#include "types.h"
#include "messages.h"
#include "messageids.h"
#include "serialization.h"
#include "stringarchive.h"
#include "ownercallbacks.h"
#include "network/session.h"

// #include "mc/connection.h"

#include <memory>

namespace mc
{
class MessageStack;
using MessageStackPtr = std::shared_ptr<MessageStack>;

class MessageStack final
{
public:
    void init(const mc::OwnerCbsPtr& owner) { _owner = owner; }
    void setConnection(const std::shared_ptr<net::Session>& conn) { m_connection = conn; }

    template<typename... Ts>
    bool send(const MessageBase<Ts...>& msg)
    {
        StringArchive sa;

        VarInt length = get_size(msg.getId()) + get_size(msg.data());

        if(_threshold > 0)
        {
            // compressed
            assert(length._value < _threshold);

            VarInt compDataLen(0);
            length._value += static_cast<int>(get_size(compDataLen));
            serialize(sa, length);
            serialize(sa, compDataLen);
            serialize(sa, msg.getId());
            serialize(sa, msg.data());
        }
        else
        {
            // uncompressed
            serialize(sa, length);
            serialize(sa, msg.getId());
            serialize(sa, msg.data());
        }

        return send(sa);
    }

    void handlePacket(StringArchive sa);

    void setCompression(int32_t maxLen);

private:
    void disconnect(StringArchive packet);
    bool send(const StringArchive& packet);

    void handleCompressed(StringArchive&, OwnerCbsPtr& owner);
    void handleUncompressed(StringArchive& sa, OwnerCbsPtr& owner);
    void handleLoginMessages(StringArchive& sa, OwnerCbsPtr& owner);
    void handlePlayMessages(StringArchive& sa, OwnerCbsPtr& owner);

    void handleKeepAlive(StringArchive& sa);

    int32_t _threshold = 0;

    OwnerCbsWPtr _owner;
    std::shared_ptr<net::Session> m_connection;
};
} // namespace mc
#endif
