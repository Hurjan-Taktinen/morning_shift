#include "utils/mc.h"

namespace mc
{

// Read single varint from input data
// return [varint, nbytes]
auto getLength(std::vector<uint8_t> const& input) -> std::tuple<int, int>
{
    int result = 0;
    int numread = 0;
    std::uint8_t byte = 0;

    do
    {
        byte = input.at(numread);
        int value = (byte & 0x7f);
        result |= (value << (7 * numread));
        numread += 1;
        assert(numread <= 5);
    } while((byte & 0x80));

    return std::make_tuple(result, numread);
    // for(auto byte : input)
    // {
    //     result |= ((byte & 0x7F) << 7 * numread);
    //
    //     if(!(byte & 0x80))
    //     {
    //         break;
    //     }
    //
    //     numread += 1;
    // }

    // for(i = 0; i < 5; ++i)
    // {
    //     // if(asio::read(*socket, asio::buffer(&packet.front() + i, 1), error) != 1 || error)
    //     // {
    //     //     return 0;
    //     // }
    //
    //     uint32_t c = packet.at(i);
    //
    //     tmp |= ((c & 0x7F) << 7 * i);
    //     if(!(c & 0x80))
    //         break;
    // }
    // packet.resize(i + 1);
    // return tmp;
}
} // namespace mc
