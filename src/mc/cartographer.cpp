#include "cartographer.h"
#include "messages.h"

#include "nbt.h"
#include "stringarchive.h"

#include "serialization.h"

namespace mc
{

void Cartographer::init(logs::Logger& log)
{
    m_log = log;
}

void Cartographer::update(const ChunkDataMsg& chunk)
{
    int x = std::get<0>(chunk.data());
    int z = std::get<1>(chunk.data());
    Nbt n = std::get<2>(chunk.data());
    auto dataa = std::get<3>(chunk.data());
    auto datab = std::get<4>(chunk.data());
    m_log->info("ChunkUpdate [{},{}] size={}, rest={}", x, z, dataa.size(), datab.size());

    // get actual chunk stuff here->
    StringArchive sa(std::move(dataa._v));
    if(x == 0 && (z == 0 || z == -1))
    {
        //m_log->info("raw=[{}]", sa.str());

        auto blockCount = Deserialize<uint16_t>::deserialize(sa);

        m_log->info("blockCount={}", blockCount);

        auto bits = Deserialize<uint8_t>::deserialize(sa);
        auto palette = Deserialize<Vector<VarInt>>::deserialize(sa);
        auto dataArray = Deserialize<Vector<uint64_t>>::deserialize(sa);

        std::stringstream ss;
        for(const auto& a : palette)
        {
            ss << a._value << " ";
        }

        m_log->info(
                "block states[bits={}, paletteLen={}, dataLen={}]",
                bits,
                ss.str(),
                dataArray.size());
        m_log->info("dump=[\n{}]", sa.str());
    }
}
void Cartographer::update(const BlockChangeMsg& chunk)
{
    auto location = std::get<0>(chunk.data());
    m_log->info(
            "BlockUpdate x={},z={},y={} newBlock={}",
            location.getX(),
            location.getZ(),
            location.getY(),
            std::get<1>(chunk.data()).hex());
}

} // namespace mc
