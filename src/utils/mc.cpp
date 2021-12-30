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
}
} // namespace mc
