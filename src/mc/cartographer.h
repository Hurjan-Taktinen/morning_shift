#pragma once

#include "messages.h"

#include "logs/log.h"

#include <array>

namespace mc
{

class Cartographer
{
public:
    void init(logs::Logger& log);
    void update(const ChunkDataMsg& chunk);
    void update(const BlockChangeMsg& block);

private:
    logs::Logger m_log;
    std::unordered_map<uint64_t, uint16_t>  map;
};
} // namespace mc
