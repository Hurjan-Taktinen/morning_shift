#pragma once

#include "messages.h"

#include "logs/log.h"

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
};
} // namespace mc
