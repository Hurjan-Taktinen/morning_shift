#ifndef MCMESSAGESIDS_H
#define MCMESSAGESIDS_H

namespace mc
{
static const int ProtocolVersion = 757;

namespace ClientBound
{
    namespace Login
    {
        enum MessageId
        {
            DISCONNECT = 0x00,
            // ENCRYPTION_REQUEST = 0x01,
            LOGIN_SUCCESS = 0x02,
            SET_COMPRESSION = 0x03,
            // LOGIN_PLUGIN_REQUEST = 0x04,
        };
    }

    namespace Play
    {
        enum MessageId
        {
            // SPAWN_OBJECT = 0x00, // 0x00 	Spawn Object
            // 0x01 	Spawn Experience Orb
            // 0x02 	Spawn Living Entity
            // 0x03 	Spawn Painting
            SPAWN_PLAYER = 0x04, // 0x04 	Spawn Player
            // 0x06 	Animation (clientbound)
            // 0x07 	Statistics
            // 0x08 	Block Break Animation
            // 0x09 	Update Block Entity
            // 0x0A 	Block Action
            BLOCK_CHANGE = 0x0C,// 0x0C 	Block Change
            // 0x0C 	Boss Bar
            SERVER_DIFFICULTY = 0x0E, // 0x0D 	Server Difficulty
            CHAT_MESSAGE = 0x0F, // 0x0E 	Chat Message (clientbound)
            // 0x0F 	Multi Block Change
            // 0x10 	Tab-Complete (clientbound)
            DECLARE_COMMANDS = 0x12, // 0x11 	Declare Commands
            // 0x12 	Confirm Transaction (clientbound)
            // 0x13 	Close Window (clientbound)
            WINDOW_ITEMS = 0x14, // 0x15 	Window Items
            // 0x16 	Window Property
            SET_SLOT = 0x16, // 0x17 	Set Slot
            // 0x18 	Set Cooldown
            PLUGIN_MESSAGE = 0x18, // 0x19 	Plugin Message (clientbound)
            // 0x1A 	Named Sound Effect
            DISCONNECT = 0x1A, // Disconnect (play)
            ENTITY_STATUS = 0x1B, // 0x1C 	Entity Status
            // 0x1E 	Explosion
            UNLOAD_CHUNK = 0x1D, // 0x1F 	Unload Chunk
            CHANGE_GAME_STATE = 0x1E, // 0x20 	Change Game State
            KEEP_ALIVE = 0x21, // 0x21 	Keep Alive (clientbound)
            CHUNK_DATA = 0x22, // 0x22 	Chunk Data
            // 0x23 	Effect
            // 0x24 	Particle
            JOIN_GAME = 0x26,   // 0x25 	Join Game
            // 0x26 	Map Data
            ENTITY_POSITION = 0x29, // 0x28 	Entity Relative Move
            ENTITY_POSITION_AND_ROTATION = 0x2A,// 0x29 	Entity Look And Relative Move
            ENTITY_ROTATION = 0x2B,// 0x2A 	Entity Look
            // 0x27 	Entity
            // 0x2B 	Vehicle Move (clientbound)
            // 0x14 	Open Window
            // 0x2C 	Open Sign Editor
            // 0x2D 	Craft Recipe Response
            PLAYER_ABILITIES = 0x32, // 0x2E 	Player Abilities (clientbound)
            // 0x2F 	Combat Event
            PLAYER_INFO = 0x36, // 0x30 	Player Info
            // 0x31 	Face Player
            PLAYER_POSITION_AND_LOOK = 0x38, // 0x32 	Player Position And Look (clientbound)
            // 0x33     Use Bed
            UNLOCK_RECIPES = 0x39, // 0x34 	Unlock Recipes
            // 0x35 	Destroy Entities
            // 0x36 	Remove Entity Effect
            // 0x37 	Resource Pack Send
            // 0x38 	Respawn
            ENTITY_HEAD_LOOK = 0x3E, // 0x39 	Entity Head Look
            // 0x3A 	Select Advancement Tab
            //WORLD_BORDER = 0x3B, // 0x3B 	World Border
            // 0x3C 	Camera
            //HELD_ITEM_CHANGE  = 0x3D, // 0x3D 	Held Item Change (clientbound)
            // 0x3E 	Display Scoreboard
            //ENTITY_METADATA = 0x3F, // 	Entity Metadata
            // 0x40 	Attach Entity
            //ENTITY_VELOCITY = 0x41, // 0x41 	Entity Velocity
            // 0x42 	Entity Equipment
            //SET_EXPERIENCE = 0x43, // 0x43 	Set Experience
            //UPDATE_HEALTH = 0x44, // Update Health
            // 0x45 	Scoreboard Objective
            // 0x46 	Set Passengers
            // 0x47 	Teams
            // 0x48 	Update Score
            SPAWN_POSITION = 0x4B, // 0x49 	Spawn Position
            TIME_UPDATE = 0x58, // 0x4A 	Time Update
            // 0x4B 	Title
            //SOUND_EFFECT = 0x4D, // 0x4D 	Sound Effect
            // 0x4C 	Stop Sound
            // 0x4E 	Player List Header And Footer
            // 0x1D 	NBT Query Response
            // 0x4F 	Collect Item
            ENTITY_TELEPORT = 0x60, // 0x50 	Entity Teleport
            //ADVANCEMENTS = 0x51, // 0x51 	Advancements
            //ENTITY_PROPERTIES = 0x52, // 	Entity Properties
            // 0x53 	Entity Effect
            //DECLARE_RECIPES = 0x54, // 0x54 	Declare Recipes
            //TAGS = 0x55, // 0x55 	Tags
        };
    }
}

namespace ServerBound
{
    namespace Login
    {
        enum MessageIds
        {
            HANDSHAKE = 0x00,
            LOGIN_START = 0x00,
        };
    }

    namespace Play
    {
        enum MessageIds
        {
            TELEPORT_CONFIRM = 0x00,        // 0x00 	Teleport Confirm
            // 0x01 	Query Block NBT
            CHAT_MESSAGE = 0x03, // 0x02 	Chat Message (serverbound)
            CLIENT_STATUS = 0x04, // 0x03 	Client Status
            CLIENT_SETTINGS = 0x05,         // 0x04 	Client Settings
            // 0x05 	Tab-Complete (serverbound)
            // 0x06 	Confirm Transaction (serverbound)
            // 0x07 	Click Window Button
            // 0x08 	Click Window
            // 0x09 	Close Window (serverbound)
            //PLUGIN_MESSAGE = 0x0A,          // 0x0A 	Plugin Message (serverbound)
            // 0x0B 	Edit Book
            // 0x0C 	Query Entity NBT
            //USE_ENTITY = 0x0D, // 0x0D 	Use Entity
            KEEP_ALIVE = 0x0F,              // 0x0E 	Keep Alive (serverbound)
            PLAYER_POSITION = 0x11,         // 0x10 	Player Position
            PLAYER_POSITION_AND_ROTATION = 0x12,// 0x11 	Player Position And Look (serverbound)
            PLAYER_ROTATION = 0x13, // 0x12 	Player Look
            PLAYER_MOVEMENT = 0x14,                  // 0x0F 	Player
            // 0x13 	Vehicle Move (serverbound)
            // 0x14 	Steer Boat
            // 0x15 	Pick Item
            // 0x16 	Craft Recipe Request
            // 0x17 	Player Abilities (serverbound)
            PLAYER_DIGGING = 0x1A, // 0x18 	Player Digging
            // 0x19 	Entity Action
            // 0x1A 	Steer Vehicle
            // 0x1B 	Recipe Book Data
            // 0x1C 	Name Item
            // 0x1D 	Resource Pack Status
            // 0x1E 	Advancement Tab
            // 0x1F 	Select Trade
            // 0x20 	Set Beacon Effect
            // 0x21 	Held Item Change (serverbound)
            // 0x22 	Update Command Block
            // 0x23 	Update Command Block Minecart
            // 0x24 	Creative Inventory Action
            // 0x25 	Update Structure Block
            // 0x26 	Update Sign
            //ANIMATION = 0x27, // 0x27 	Animation (serverbound)
            // 0x28 	Spectate
            // 0x29 	Player Block Placement
            // 0x2A 	Use Item
        };
    }
}

}
#endif
