#pragma once

#include <cassert>
#include <cstdint>
#include <tuple>
#include <vector>

namespace mc
{

auto getLength(std::vector<uint8_t> const& input) -> std::tuple<int, int>;

}
