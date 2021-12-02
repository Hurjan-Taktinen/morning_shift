#ifndef MESSAGES_H
#define MESSAGES_H

#include "messageids.h"
#include "types.h"

#include <tuple>

namespace mc
{
template <typename Ts> class MessageBase
{
public:
    using Type = Ts;

    virtual ~MessageBase() {}
    MessageBase() {}
    MessageBase(const uint8_t id, const Type& data = Type())
        : _id(id), _data(data)
    {
    }

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
class HandshakeMsg : public MessageBase<HandshakeType>
{
public:
    virtual ~HandshakeMsg() {}
    HandshakeMsg() : MessageBase(ServerBound::Login::HANDSHAKE) {}
    HandshakeMsg(const HandshakeType& t)
        : MessageBase(ServerBound::Login::HANDSHAKE, t)
    {
    }
};

// Clientbound
using DifficultyType = uint8_t;
class DifficultyMsg : public MessageBase<DifficultyType>
{
public:
    enum difficulty
    {
        PEACEFULL = 0,
        EASY = 1,
        NORMAL = 2,
        HARD = 3
    };

    virtual ~DifficultyMsg() {}
    DifficultyMsg() : MessageBase(ClientBound::Play::SERVER_DIFFICULTY) {}
    DifficultyMsg(const DifficultyType& t)
        : MessageBase(ClientBound::Play::SERVER_DIFFICULTY, t)
    {
    }

    bool operator==(const DifficultyMsg& rhs)
    {
        return (this->getId() == rhs.getId() && this->data() == rhs.data());
    }

    bool operator!=(const DifficultyMsg& rhs) { return !(*this == rhs); }
};

using LoginStartType = String;
class LoginStartMsg : public MessageBase<LoginStartType>
{
public:
    virtual ~LoginStartMsg() {}
    LoginStartMsg() : MessageBase(ServerBound::Login::LOGIN_START) {}
    LoginStartMsg(const LoginStartType& t)
        : MessageBase(ServerBound::Login::LOGIN_START, t)
    {
    }

    bool operator==(const LoginStartMsg& rhs)
    {
        return (this->getId() == rhs.getId() && this->data() == rhs.data());
    }

    bool operator!=(const LoginStartMsg& rhs) { return !(*this == rhs); }
};

using SetCompressionType = VarInt;
class SetCompressionMsg : public MessageBase<SetCompressionType>
{
public:
    virtual ~SetCompressionMsg() {}
    SetCompressionMsg() : MessageBase(ClientBound::Login::SET_COMPRESSION) {}
    SetCompressionMsg(const SetCompressionType& t)
        : MessageBase(ClientBound::Login::SET_COMPRESSION, t)
    {
    }

    bool operator==(const SetCompressionMsg& rhs)
    {
        return (this->getId() == rhs.getId() && this->data() == rhs.data());
    }

    bool operator!=(const SetCompressionMsg& rhs) { return !(*this == rhs); }
};

// Client bound Login State
using LoginSuccessType = std::tuple<String, String>; // state Play after this
class LoginSuccessMsg : public MessageBase<LoginSuccessType>
{
public:
    virtual ~LoginSuccessMsg() {}
    LoginSuccessMsg() : MessageBase(ClientBound::Login::LOGIN_SUCCESS) {}
    LoginSuccessMsg(const LoginSuccessType& t)
        : MessageBase(ClientBound::Login::LOGIN_SUCCESS, t)
    {
    }

    bool operator==(const LoginSuccessMsg& rhs)
    {
        return (this->getId() == rhs.getId() && this->data() == rhs.data());
    }

    bool operator!=(const LoginSuccessMsg& rhs) { return !(*this == rhs); }
};

// Client bound Play State
using JoinGameType =
    std::tuple<int32_t, uint8_t, int32_t, uint8_t, uint8_t, String, uint8_t>;
class JoinGameMsg : public MessageBase<JoinGameType>
{
public:
    virtual ~JoinGameMsg() {}
    JoinGameMsg() : MessageBase(ClientBound::Play::JOIN_GAME) {}
    JoinGameMsg(const JoinGameType& t)
        : MessageBase(ClientBound::Play::JOIN_GAME, t)
    {
    }

    bool operator==(const JoinGameMsg& rhs)
    {
        return (this->getId() == rhs.getId() && this->data() == rhs.data());
    }

    bool operator!=(const JoinGameMsg& rhs) { return !(*this == rhs); }
};

// Same content for Client/Serverbound send this after join game is received
using PluginMessageType = std::tuple<String, ByteArrayEnd>;
class PluginMessageMsg : public MessageBase<PluginMessageType>
{
public:
    virtual ~PluginMessageMsg() {}
    PluginMessageMsg() : MessageBase(ServerBound::Play::PLUGIN_MESSAGE) {}
    PluginMessageMsg(const PluginMessageType& t)
        : MessageBase(ServerBound::Play::PLUGIN_MESSAGE, t)
    {
    }

    bool operator==(const PluginMessageMsg& rhs)
    {
        return (this->getId() == rhs.getId() && this->data() == rhs.data());
    }

    bool operator!=(const PluginMessageMsg& rhs) { return !(*this == rhs); }
};

using HeldItemChangeType = uint8_t;
class HeldItemChangeMsg : public MessageBase<HeldItemChangeType>
{
public:
    virtual ~HeldItemChangeMsg() {}
    HeldItemChangeMsg() : MessageBase(ClientBound::Play::HELD_ITEM_CHANGE) {}
    HeldItemChangeMsg(const HeldItemChangeType& t)
        : MessageBase(ClientBound::Play::HELD_ITEM_CHANGE, t)
    {
    }

    bool operator==(const HeldItemChangeMsg& rhs)
    {
        return (this->getId() == rhs.getId() && this->data() == rhs.data());
    }

    bool operator!=(const HeldItemChangeMsg& rhs) { return !(*this == rhs); }
};

using PlayerAbilitiesType = std::tuple<uint8_t, float, float>;
;
class PlayerAbilitiesMsg : public MessageBase<PlayerAbilitiesType>
{
public:
    virtual ~PlayerAbilitiesMsg() {}
    PlayerAbilitiesMsg() : MessageBase(ClientBound::Play::PLAYER_ABILITIES) {}
    PlayerAbilitiesMsg(const PlayerAbilitiesType& t)
        : MessageBase(ClientBound::Play::PLAYER_ABILITIES, t)
    {
    }

    bool operator==(const PlayerAbilitiesMsg& rhs)
    {
        return (this->getId() == rhs.getId() && this->data() == rhs.data());
    }

    bool operator!=(const PlayerAbilitiesMsg& rhs) { return !(*this == rhs); }
};

// Number of element + elements TODO: add recipe support
using DeclareRecipesType = std::tuple<VarInt, ByteArrayEnd>;
class DeclareRecipesMsg : public MessageBase<DeclareRecipesType>
{
public:
    virtual ~DeclareRecipesMsg() {}
    DeclareRecipesMsg() : MessageBase(ClientBound::Play::DECLARE_RECIPES) {}
    DeclareRecipesMsg(const DeclareRecipesType& t)
        : MessageBase(ClientBound::Play::DECLARE_RECIPES, t)
    {
    }

    bool operator==(const DeclareRecipesMsg& rhs)
    {
        return (this->getId() == rhs.getId() && this->data() == rhs.data());
    }

    bool operator!=(const DeclareRecipesMsg& rhs) { return !(*this == rhs); }
};

// Tag = Identifier, ArrayLen+Array<VarInt>
// BlockTags, ItemTags, FluidTags
// using TagsType = std::tuple<Vector<Tag>, Vector<Tag>, Vector<Tag>>;

// Chunck DATA
// using ChunkDataType = std::tuple<int32_t, int32_t, bool, ByteArray(including
// VarInt size), VarInt<ArraySize>, Array<NBT_Tag>>
using ChunkDataType = std::tuple<int32_t, int32_t, bool, VarInt, ByteArray,
                                 ByteArrayEnd>; // + NBT tagit
class ChunkDataMsg : public MessageBase<ChunkDataType>
{
public:
    virtual ~ChunkDataMsg() {}
    ChunkDataMsg() : MessageBase(ClientBound::Play::CHUNK_DATA) {}
    ChunkDataMsg(const ChunkDataType& t)
        : MessageBase(ClientBound::Play::CHUNK_DATA, t)
    {
    }

    bool operator==(const ChunkDataMsg& rhs)
    {
        return (this->getId() == rhs.getId() && this->data() == rhs.data());
    }

    bool operator!=(const ChunkDataMsg& rhs) { return !(*this == rhs); }
};

using KeepAliveType = int64_t;
class KeepAliveMsg : public MessageBase<KeepAliveType>
{
public:
    virtual ~KeepAliveMsg() {}
    KeepAliveMsg() : MessageBase(ServerBound::Play::KEEP_ALIVE) {}
    KeepAliveMsg(const KeepAliveType& t)
        : MessageBase(ServerBound::Play::KEEP_ALIVE, t)
    {
    }

    bool operator==(const KeepAliveMsg& rhs)
    {
        return (this->getId() == rhs.getId() && this->data() == rhs.data());
    }

    bool operator!=(const KeepAliveMsg& rhs) { return !(*this == rhs); }
};

using ChatMessageType = std::tuple<String, uint8_t>;
class ChatMessageMsg : public MessageBase<ChatMessageType>
{
public:
    virtual ~ChatMessageMsg() {}
    ChatMessageMsg() : MessageBase(ClientBound::Play::CHAT_MESSAGE) {}
    ChatMessageMsg(const ChatMessageType& t)
        : MessageBase(ClientBound::Play::CHAT_MESSAGE, t)
    {
    }

    bool operator==(const ChatMessageMsg& rhs)
    {
        return (this->getId() == rhs.getId() && this->data() == rhs.data());
    }

    bool operator!=(const ChatMessageMsg& rhs) { return !(*this == rhs); }

    std::string getText() const
    {
        std::string str;

        std::string json = std::get<0>(data())._value;

        return str;
    }
};

using DisconnectMsgType = std::tuple<String>;
class DisconnectMsg : public MessageBase<DisconnectMsgType>
{
public:
    virtual ~DisconnectMsg() {}
    DisconnectMsg() : MessageBase(ClientBound::Play::DISCONNECT) {}
    DisconnectMsg(const DisconnectMsgType& t)
        : MessageBase(ClientBound::Play::DISCONNECT, t)
    {
    }

    auto operator<=>(const DisconnectMsg& rhs) const = default;
};

// Player Position And Look (clientbound)
using PlayerPositionAndLookType =
    std::tuple<double, double, double, float, float, uint8_t, VarInt>;
class PlayerPositionAndLookMsg : public MessageBase<PlayerPositionAndLookType>
{
public:
    virtual ~PlayerPositionAndLookMsg() {}
    PlayerPositionAndLookMsg()
        : MessageBase(ClientBound::Play::PLAYER_POSITION_AND_LOOK)
    {
    }
    PlayerPositionAndLookMsg(const PlayerPositionAndLookType& msg)
        : MessageBase(ClientBound::Play::PLAYER_POSITION_AND_LOOK, msg)
    {
    }
};
//          +
// Teleport Confirm
//          +
// Timeupdate
using TimeUpdateType = std::tuple<int64_t, int64_t>;
class TimeUpdateMsg : public MessageBase<TimeUpdateType>
{
public:
    virtual ~TimeUpdateMsg() {}
    TimeUpdateMsg() : MessageBase(ClientBound::Play::TIME_UPDATE) {}
    TimeUpdateMsg(const TimeUpdateType& t)
        : MessageBase(ClientBound::Play::TIME_UPDATE, t)
    {
    }

    bool operator==(const TimeUpdateMsg& rhs)
    {
        return (this->getId() == rhs.getId() && this->data() == rhs.data());
    }

    bool operator!=(const TimeUpdateMsg& rhs) { return !(*this == rhs); }
};

using PlayerPositionType = std::tuple<double, double, double, bool>;
class PlayerPositionMsgFromClient : public MessageBase<PlayerPositionType>
{
public:
    virtual ~PlayerPositionMsgFromClient() {}
    PlayerPositionMsgFromClient()
        : MessageBase(ServerBound::Play::PLAYER_POSITION)
    {
    }
    PlayerPositionMsgFromClient(const PlayerPositionType& msg)
        : MessageBase(ServerBound::Play::PLAYER_POSITION, msg)
    {
    }
};

using PlayerLookType = std::tuple<float, float, bool>;
class PlayerLookMsg : public MessageBase<PlayerLookType>
{
public:
    virtual ~PlayerLookMsg() {}
    PlayerLookMsg() : MessageBase(ServerBound::Play::PLAYER_LOOK) {}
    PlayerLookMsg(const PlayerLookType& msg)
        : MessageBase(ServerBound::Play::PLAYER_LOOK, msg)
    {
    }
};

using TeleportConfirmType = VarInt;
class TeleportConfirmMsg : public MessageBase<TeleportConfirmType>
{
public:
    virtual ~TeleportConfirmMsg() {}
    TeleportConfirmMsg() : MessageBase(ServerBound::Play::TELEPORT_CONFIRM) {}
    TeleportConfirmMsg(const TeleportConfirmType& msg)
        : MessageBase(ServerBound::Play::TELEPORT_CONFIRM, msg)
    {
    }
};

using PlayerInfoType = std::tuple<VarInt, VarInt, ByteArrayEnd>;
class PlayerInfoMsg : public MessageBase<PlayerInfoType>
{
public:
    virtual ~PlayerInfoMsg() {}
    PlayerInfoMsg() : MessageBase(ClientBound::Play::PLAYER_INFO) {}
    PlayerInfoMsg(const PlayerInfoType& msg)
        : MessageBase(ClientBound::Play::PLAYER_INFO, msg)
    {
    }
};

using SpawnPlayerType =
    std::tuple<VarInt, Uuid, double, double, double, Angle, Angle>;
class SpawnPlayerMsg : public MessageBase<SpawnPlayerType>
{
public:
    virtual ~SpawnPlayerMsg() = default;
    SpawnPlayerMsg() : MessageBase(ClientBound::Play::SPAWN_PLAYER) {}
    SpawnPlayerMsg(const SpawnPlayerType& msg)
        : MessageBase(ClientBound::Play::SPAWN_PLAYER, msg)
    {
    }
};

using EntityTeleportType =
    std::tuple<VarInt, double, double, double, Angle, Angle,
               Bool>; // Angle, Angle, Bool(onground)
class EntityTeleportMsg : public MessageBase<EntityTeleportType>
{
public:
    virtual ~EntityTeleportMsg() = default;
    EntityTeleportMsg() : MessageBase(ClientBound::Play::ENTITY_TELEPORT) {}
    EntityTeleportMsg(const EntityTeleportType& msg)
        : MessageBase(ClientBound::Play::ENTITY_TELEPORT, msg)
    {
    }
};

using EntityRelativeMovementType =
    std::tuple<VarInt, int16_t, int16_t,
               int16_t>; // Angle, Angle, Bool(onground)
class EntityRelativeMovementMsg : public MessageBase<EntityRelativeMovementType>
{
public:
    virtual ~EntityRelativeMovementMsg() = default;
    EntityRelativeMovementMsg()
        : MessageBase(ClientBound::Play::ENTITY_RELATIVE_MOVEMENT)
    {
    }
    EntityRelativeMovementMsg(const EntityRelativeMovementType& msg)
        : MessageBase(ClientBound::Play::ENTITY_RELATIVE_MOVEMENT, msg)
    {
    }
};

using EntityLookAndRelativeMovementType =
    std::tuple<VarInt, int16_t, int16_t, int16_t, Angle, Angle,
               Bool>; // Angle, Angle, Bool(onground)
class EntityLookAndRelativeMovementMsg
    : public MessageBase<EntityLookAndRelativeMovementType>
{
public:
    virtual ~EntityLookAndRelativeMovementMsg() = default;
    EntityLookAndRelativeMovementMsg()
        : MessageBase(ClientBound::Play::ENTITY_LOOK_AND_RELATIVE_MOVEMENT)
    {
    }
    EntityLookAndRelativeMovementMsg(
        const EntityLookAndRelativeMovementType& msg)
        : MessageBase(ClientBound::Play::ENTITY_LOOK_AND_RELATIVE_MOVEMENT, msg)
    {
    }
};

using EntityLookType = std::tuple<VarInt, Angle, Angle, Bool>;
class EntityLookMsg : public MessageBase<EntityLookType>
{
public:
    virtual ~EntityLookMsg() = default;
    EntityLookMsg() : MessageBase(ClientBound::Play::ENTITY_LOOK) {}
    EntityLookMsg(const EntityLookType& msg)
        : MessageBase(ClientBound::Play::ENTITY_LOOK, msg)
    {
    }
};

using ClientStatusType = VarInt;
class ClientStatusMsg : public MessageBase<ClientStatusType>
{
public:
    virtual ~ClientStatusMsg() = default;
    ClientStatusMsg() : MessageBase(ServerBound::Play::CLIENT_STATUS) {}
    ClientStatusMsg(const ClientStatusType& msg)
        : MessageBase(ServerBound::Play::CLIENT_STATUS, msg)
    {
    }
};

using UpdateHealthType = std::tuple<float, VarInt, float>;
class UpdateHealthMsg : public MessageBase<UpdateHealthType>
{
public:
    virtual ~UpdateHealthMsg() = default;
    UpdateHealthMsg() : MessageBase(ClientBound::Play::UPDATE_HEALTH) {}
    UpdateHealthMsg(const UpdateHealthType& msg)
        : MessageBase(ClientBound::Play::UPDATE_HEALTH, msg)
    {
    }
};

using UseEntityType = std::tuple<VarInt, VarInt>;
class UseEntityMsg final : public MessageBase<UseEntityType>
{
public:
    UseEntityMsg() : MessageBase(ServerBound::Play::USE_ENTITY) {}
    UseEntityMsg(const UseEntityType& msg)
        : MessageBase(ServerBound::Play::USE_ENTITY, msg)
    {
    }

    enum Type
    {
        INTERACT = 0,
        ATTACK = 1,
        INTERRACT_AT = 2
    };
};

using ChatType = String;
class ChatMsg : public MessageBase<ChatType>
{
public:
    virtual ~ChatMsg() = default;
    ChatMsg() : MessageBase(ServerBound::Play::CHAT_MESSAGE) {}
    ChatMsg(const ChatType& msg)
        : MessageBase(ServerBound::Play::CHAT_MESSAGE, msg)
    {
    }
};
// 0 	Invalid Bed 	Would be used to switch between messages, but the only
// used message is 0 for invalid bed 1 	End raining 2 	Begin raining
// 3 	Change gamemode 	0: Survival, 1: Creative, 2: Adventure, 3:
// Spectator 4 	Exit end 	0: Immediately send Client Status of respawn
// without showing end credits; 1: Show end credits and respawn at the end (or
// when esc is pressed). 1 is sent if the player has not yet received the "The
// end?" advancement, while if they do have it 0 is used. 5 	Demo message
// 0: Show welcome to demo screen, 101: Tell movement controls, 102: Tell jump
// control, 103: Tell inventory control 6 	Arrow hitting player
// Appears to be played when an arrow strikes another player in Multiplayer 7
// Fade value 	The current darkness value. 1 = Dark, 0 = Bright, Setting the
// value higher causes the game to change color and freeze 8 	Fade time
// Time in ticks for the sky to fade 9 	Play pufferfish sting sound 10 	Play
// elder guardian mob appearance (effect and sound)

//          +
// Spawn position
} // namespace mc

#endif
