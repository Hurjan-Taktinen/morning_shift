#ifndef OWNERCALLBACKS_H
#define OWNERCALLBACKS_H

#include "messages.h"

#include <memory>

namespace mc
{
class OwnerCbs;
using OwnerCbsPtr = std::shared_ptr<OwnerCbs>;
using OwnerCbsWPtr = std::weak_ptr<OwnerCbs>;

class OwnerCbs
{
public:
    virtual ~OwnerCbs() = default;
    virtual void handleMessage(const mc::LoginSuccessMsg&) = 0;
    // virtual void handleMessage(const mc::JoinGameMsg&) = 0;
    // virtual void handleMessage(const mc::PluginMessageMsg&) = 0;
    // virtual void handleMessage(const mc::DifficultyMsg& msg) = 0;
    // virtual void handleMessage(const mc::DeclareRecipesMsg& msg) = 0;
    // virtual void handleMessage(const mc::PlayerAbilitiesMsg& msg) = 0;
    // virtual void handleMessage(const mc::HeldItemChangeMsg& msg) = 0;
     virtual void handleMessage(const mc::ChunkDataMsg& msg) = 0;
    // virtual void handleMessage(const mc::ChatMessageMsg& msg) = 0;
    // virtual void handleMessage(const mc::TimeUpdateMsg& msg) = 0;
    // virtual void handleMessage(const mc::PlayerPositionAndLookMsg& msg) = 0;
    // virtual void handleMessage(const mc::DisconnectMsg& msg) = 0;
    // virtual void handleMessage(const mc::PlayerInfoMsg& msg) = 0;
    // virtual void handleMessage(const mc::UpdateHealthMsg& msg) = 0;
    // virtual void handleMessage(const mc::EntityTeleportMsg& msg) = 0;
    // virtual void handleMessage(const mc::EntityRelativeMovementMsg& msg) = 0;
    // virtual void handleMessage(const mc::EntityLookAndRelativeMovementMsg& msg) = 0;
    // virtual void handleMessage(const mc::EntityLookMsg& msg) = 0;
    // virtual void handleMessage(const mc::SpawnPlayerMsg& msg) = 0;
    // virtual void handleMessage(const mc::SpawnPositionMsg& msg) = 0;
    // virtual void handleMessage(const mc::WorldBorderMsg& msg) = 0;
    // virtual void handleMessage(const mc::WindowItemsMsg& msg) = 0;
    // virtual void handleMessage(const mc::SetSlotMsg& msg) = 0;

    virtual void handleDisconnect(const std::string& reason) = 0;
    virtual bool compression() const { return false; }

    enum State
    {
        DISCONNECTED,
        CONNECTED,
        LOGIN,
        PLAY
    };

    State getState() const { return _state; }

    State _state = DISCONNECTED;
};
} // namespace mc

#endif
