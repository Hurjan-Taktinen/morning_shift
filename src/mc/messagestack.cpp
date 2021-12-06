#include "messagestack.h"
#include "serialization.h"
#include "stringarchive.h"
#include "mc/connection.h"

#include <cassert>
#include <zlib.h>

namespace mc
{

void MessageStack::handleCompressed(StringArchive& sa, OwnerCbsPtr& owner)
{
    VarInt packetLength = Deserialize<VarInt>::deserialize(sa);
    VarInt dataLength = Deserialize<VarInt>::deserialize(sa);

    if(dataLength._value == 0) // uncompressed
    {
        handleUncompressed(sa, owner);
        return;
    }

    StringArchive uncomp;
    uncomp.resize(dataLength._value);

    uLong compressedSize = static_cast<uLong>(dataLength._value);

    uncompress(
            (Bytef*)uncomp.data(),
            &compressedSize,
            (Bytef*)sa.data() + sa._readOffset,
            packetLength._value - get_size(dataLength));
    handleUncompressed(uncomp, owner);
}
void MessageStack::disconnect(StringArchive packet)
{
    auto owner = _owner.lock();
    if(owner)
    {
        auto msg = Deserialize<DisconnectMsg::Type>::deserialize(packet);
        std::string reason = std::get<0>(msg).getValue();
        owner->handleDisconnect(reason);
    }
}

bool MessageStack::send(const StringArchive& packet)
{
    if(m_conn)
    {
        return m_conn->send(packet);
    }
    return false;
}

void MessageStack::handlePacket(StringArchive sa)
{
    OwnerCbsPtr owner = _owner.lock();
    assert(owner);

    if(_threshold > 0)
    {
        handleCompressed(sa, owner);
    }
    else
    {
        // consume packet len. it is not needed anymore
        if(Deserialize<VarInt>::deserialize(sa)._value > 0)
            handleUncompressed(sa, owner);
    }
}

void MessageStack::handleUncompressed(StringArchive& sa, OwnerCbsPtr& owner)
{
    switch(owner->getState())
    {
    case OwnerCbs::DISCONNECTED: break;
    case OwnerCbs::CONNECTED: break;
    case OwnerCbs::LOGIN: handleLoginMessages(sa, owner); break;
    case OwnerCbs::PLAY: handlePlayMessages(sa, owner); break;
    }
}

template<class T>
inline void unpack(OwnerCbsPtr& owner, StringArchive& sa)
{
    owner->handleMessage(T(Deserialize<typename T::Type>::deserialize(sa)));
}

void MessageStack::handleLoginMessages(StringArchive& sa, OwnerCbsPtr& owner)
{
    using namespace ClientBound::Login;

    VarInt msgId = Deserialize<VarInt>::deserialize(sa);

    switch(MessageId(msgId._value))
    {
    case LOGIN_SUCCESS: unpack<LoginSuccessMsg>(owner, sa); return;
    case DISCONNECT: disconnect(sa); return;
    case SET_COMPRESSION:
        SetCompressionMsg msg(Deserialize<typename SetCompressionMsg::Type>::deserialize(sa));
        setCompression(msg.data()._value);
        std::cout << "set compression threshold to " << msg.data()._value << std::endl;
        return;
    }

    // std::cout << "Login: Unknown message inLogin state: id=" << msgId.hex()
    // << " raw[" << sa << "]" <<std::endl;
}

static void logUnhandledMessage(const std::string& str)
{
    std::cout << "Received message: " << str << std::endl;
}

void MessageStack::handlePlayMessages(StringArchive& sa, OwnerCbsPtr& owner)
{
    using namespace ClientBound::Play;

    VarInt msgId = Deserialize<VarInt>::deserialize(sa);

    switch(MessageId(msgId._value))
    {
    case KEEP_ALIVE: handleKeepAlive(sa); return;
    case CHUNK_DATA: unpack<ChunkDataMsg>(owner, sa); return;
    case BLOCK_CHANGE: unpack<BlockChangeMsg>(owner, sa); return;

    case SERVER_DIFFICULTY: logUnhandledMessage("SERVER_DIFFICULTY"); return;
    case PLUGIN_MESSAGE: logUnhandledMessage("PLUGIN_MESSAGE"); return;
    case JOIN_GAME: logUnhandledMessage("JOIN_GAME"); return;
    case PLAYER_ABILITIES: logUnhandledMessage("PLAYER_ABILITIES"); return;
    // case HELD_ITEM_CHANGE: logUnhandledMessage("HELD_ITEM_CHANGE"); return;
    case CHAT_MESSAGE: logUnhandledMessage("CHAT_MESSAGE"); return;
    case TIME_UPDATE: logUnhandledMessage("TIME_UPDATE"); return;
    case PLAYER_POSITION_AND_LOOK: logUnhandledMessage("PLAYER_POSITION_AND_LOOK"); return;
    case DISCONNECT: logUnhandledMessage("DISCONNECT"); return;
    case PLAYER_INFO: logUnhandledMessage("PLAYER_INFO"); return;
    // case UPDATE_HEALTH: logUnhandledMessage("UPDATE_HEALTH"); return;
    case ENTITY_TELEPORT: logUnhandledMessage("ENTITY_TELEPORT"); return;
    case SPAWN_PLAYER: logUnhandledMessage("SPAWN_PLAYER"); return;
    case ENTITY_POSITION: logUnhandledMessage("ENTITY_POSITION"); return;
    case ENTITY_POSITION_AND_ROTATION: logUnhandledMessage("ENTITY_POSITION_AND_ROTATION"); return;
    case ENTITY_ROTATION:
        logUnhandledMessage("ENTITY_ROTATION");
        return;

        // case SERVER_DIFFICULTY: unpack<DifficultyMsg>(owner, sa); return;
        // case PLUGIN_MESSAGE: unpack<PluginMessageMsg>(owner, sa); return;
        // case JOIN_GAME: unpack<JoinGameMsg>(owner, sa); return;
        // case PLAYER_ABILITIES: unpack<PlayerAbilitiesMsg>(owner, sa); return;
        // case HELD_ITEM_CHANGE: unpack<HeldItemChangeMsg>(owner, sa); return;
        // case CHAT_MESSAGE: unpack<ChatMessageMsg>(owner, sa); return;
        // case TIME_UPDATE: unpack<TimeUpdateMsg>(owner, sa); return;
        // case PLAYER_POSITION_AND_LOOK: unpack<PlayerPositionAndLookMsg>(owner, sa); return;
        // case DISCONNECT: unpack<DisconnectMsg>(owner, sa); return;
        // case PLAYER_INFO: unpack<PlayerInfoMsg>(owner, sa); return;
        // case UPDATE_HEALTH: unpack<UpdateHealthMsg>(owner, sa); return;
        // case ENTITY_TELEPORT: unpack<EntityTeleportMsg>(owner, sa); return;
        // case ENTITY_RELATIVE_MOVEMENT: unpack<EntityRelativeMovementMsg>(owner, sa); return;
        // case SPAWN_PLAYER: unpack<SpawnPlayerMsg>(owner, sa); return;

        // case  ENTITY_LOOK_AND_RELATIVE_MOVEMENT: unpack<EntityLookAndRelativeMovementMsg>(owner,
        // sa); return; case ENTITY_LOOK: unpack<EntityLookMsg>(owner, sa); return;
        //  case SPAWN_PLAYER: unpack<SpawnPlayerMsg>(owner, sa); return;
        //  case SPAWN_POSITION: unpack<SpawnPositionMsg>(owner, sa); return;
        //  case WORLD_BORDER: unpack<WorldBorderMsg>(owner, sa); return;
        //  case WINDOW_ITEMS: unpack<WindowItemsMsg>(owner, sa); return;
        //  case SET_SLOT: unpack<SetSlotMsg>(owner, sa); return;

    case ENTITY_HEAD_LOOK: logUnhandledMessage("ENTITY_HEAD_LOOK"); return;
    // case ENTITY_VELOCITY: logUnhandledMessage("ENTITY_VELOCITY"); return;
    case SPAWN_POSITION: logUnhandledMessage("SPAWN_POSITION"); return;
    // case WORLD_BORDER: logUnhandledMessage("WORLD_BORDER"); return;
    case WINDOW_ITEMS: logUnhandledMessage("WINDOW_ITEMS"); return;
    case SET_SLOT: logUnhandledMessage("SET_SLOT"); return;
    // case DECLARE_RECIPES: logUnhandledMessage("DECLARE_RECIPES"); return;
    case ENTITY_STATUS: logUnhandledMessage("ENTITY_STATUS"); return;
    case DECLARE_COMMANDS: logUnhandledMessage("DECLARE_COMMANDS"); return;
    case UNLOCK_RECIPES: logUnhandledMessage("UNLOCK_RECIPES"); return;
    case CHANGE_GAME_STATE: logUnhandledMessage("CHANGE_GAME_STATE"); return;
    // case TAGS: logUnhandledMessage("TAGS"); return;
    // case ADVANCEMENTS: logUnhandledMessage("ADVANCEMENTS"); return;
    // case SET_EXPERIENCE: logUnhandledMessage("SET EXPERIENCE"); return;
    case UNLOAD_CHUNK:
        logUnhandledMessage("UNLOAD_CHUNK");
        return;
        // case ENTITY_METADATA: logUnhandledMessage("ENTITY_METADATA"); return;
        // case ENTITY_PROPERTIES: logUnhandledMessage("ENTITY_PROPERTIES"); return;
        // case SOUND_EFFECT: logUnhandledMessage("SOUND_EFFECT"); return;
    }
    // std::ostringstream os;
    // os << "Play: Unknown/unsupported message: id=" << std::hex
    // << msgId << " raw[" << sa << "]" << std::endl;
    // std::cout << os.str();
}

void MessageStack::setCompression(int32_t threshold)
{
    _threshold = threshold;
}

void MessageStack::handleKeepAlive(StringArchive& sa)
{
    KeepAliveMsg msg = Deserialize<KeepAliveMsg::Type>::deserialize(sa);
    std::cout << "KeepAlive " << std::endl;
    send(msg);
}

} // namespace mc
