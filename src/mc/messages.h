#ifndef MESSAGES_H
#define MESSAGES_H

#include "nbt.h"
#include "messageids.h"
#include "types.h"

#include <tuple>

namespace mc
{
template<typename Ts>
class MessageBase
{
public:
    using Type = Ts;

    virtual ~MessageBase() {}
    MessageBase() {}
    MessageBase(const uint8_t id, const Type& data = Type()) : _id(id), _data(data) {}

    auto operator<=>(const MessageBase& rhs) const = default;

    bool isValid() const { return _id != -1; }

    const VarInt& getId() const { return _id; }

    const Type& data() const { return _data; };
    Type& data() { return _data; };

private:
    VarInt _id = -1;
    Type _data;
};

using HandshakeType = std::tuple<VarInt, String, uint16_t, VarInt>;
// Server bound Login State
class HandshakeMsg final : public MessageBase<HandshakeType>
{
public:
    HandshakeMsg() : MessageBase(ServerBound::Login::HANDSHAKE) {}
    HandshakeMsg(const HandshakeType& t) : MessageBase(ServerBound::Login::HANDSHAKE, t) {}
};

// Clientbound
using LoginStartType = String;
class LoginStartMsg final : public MessageBase<LoginStartType>
{
public:
    LoginStartMsg() : MessageBase(ServerBound::Login::LOGIN_START) {}
    LoginStartMsg(const LoginStartType& t) : MessageBase(ServerBound::Login::LOGIN_START, t) {}
};

using SetCompressionType = VarInt;
class SetCompressionMsg final : public MessageBase<SetCompressionType>
{
public:
    SetCompressionMsg() : MessageBase(ClientBound::Login::SET_COMPRESSION) {}
    SetCompressionMsg(const SetCompressionType& t) :
        MessageBase(ClientBound::Login::SET_COMPRESSION, t)
    {
    }
};

// Client bound Login State
using LoginSuccessType = std::tuple<Uuid, String>; // state Play after this
class LoginSuccessMsg final : public MessageBase<LoginSuccessType>
{
public:
    LoginSuccessMsg() : MessageBase(ClientBound::Login::LOGIN_SUCCESS) {}
    LoginSuccessMsg(const LoginSuccessType& t) : MessageBase(ClientBound::Login::LOGIN_SUCCESS, t)
    {
    }
};

// Client bound Play State
using JoinGameType = std::tuple<int32_t, Bool, uint8_t>; //ignored -> , uint8_t, int32_t, uint8_t, uint8_t, String, uint8_t>;
class JoinGameMsg final : public MessageBase<JoinGameType>
{
public:
    JoinGameMsg() : MessageBase(ClientBound::Play::JOIN_GAME) {}
    JoinGameMsg(const JoinGameType& t) : MessageBase(ClientBound::Play::JOIN_GAME, t) {}
};

using ChunkDataType = std::tuple<int32_t, int32_t, Nbt, ByteArray, ByteArrayEnd>;
class ChunkDataMsg final : public MessageBase<ChunkDataType>
{
public:
    ChunkDataMsg() : MessageBase(ClientBound::Play::CHUNK_DATA) {}
    ChunkDataMsg(const ChunkDataType& t) : MessageBase(ClientBound::Play::CHUNK_DATA, t) {}
};

using BlockChangeType = std::tuple<Position, VarInt>;
class BlockChangeMsg final : public MessageBase<BlockChangeType>
{
public:
    BlockChangeMsg() : MessageBase(ClientBound::Play::BLOCK_CHANGE) {}
    BlockChangeMsg(const BlockChangeType& t) : MessageBase(ClientBound::Play::BLOCK_CHANGE, t) {}
};


using KeepAliveType = int64_t;
class KeepAliveMsg final : public MessageBase<KeepAliveType>
{
public:
    KeepAliveMsg() : MessageBase(ServerBound::Play::KEEP_ALIVE) {}
    KeepAliveMsg(const KeepAliveType& t) : MessageBase(ServerBound::Play::KEEP_ALIVE, t) {}
};

using DisconnectMsgType = std::tuple<String>;
class DisconnectMsg final : public MessageBase<DisconnectMsgType>
{
public:
    DisconnectMsg() : MessageBase(ClientBound::Play::DISCONNECT) {}
    DisconnectMsg(const DisconnectMsgType& t) : MessageBase(ClientBound::Play::DISCONNECT, t) {}
};

} // namespace mc

#endif
