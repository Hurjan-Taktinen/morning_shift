#include <cstdint>
#define CATCH_CONFIG_MAIN

#include "utils/mc.h"
#include "catch2/catch.hpp"

#include <vector>

TEST_CASE("parse 0")
{
    {
        std::vector<uint8_t> data = {0x00};
        auto [vi, bytes] = mc::getLength(data);
        REQUIRE(0 == vi);
        REQUIRE(1 == bytes);
    }
    {
        std::vector<uint8_t> data = {0x00, 0x00, 0x00, 0x00, 0x00};
        auto [vi, bytes] = mc::getLength(data);
        int expected = 0;
        REQUIRE(expected == vi);
        REQUIRE(1 == bytes);
    }
}

TEST_CASE("parse 1")
{
    {
        std::vector<uint8_t> data = {0x01, 0x00, 0x00, 0x00, 0x00};
        auto [vi, bytes] = mc::getLength(data);
        REQUIRE(1 == vi);
        REQUIRE(1 == bytes);
    }
}

TEST_CASE("parse 2")
{
    {
        std::vector<uint8_t> data = {0x02};
        auto [vi, bytes] = mc::getLength(data);
        REQUIRE(2 == vi);
        REQUIRE(1 == bytes);
    }
}

TEST_CASE("parse 127")
{
    {
        std::vector<uint8_t> data = {0x7f};
        auto [vi, bytes] = mc::getLength(data);
        REQUIRE(127 == vi);
        REQUIRE(1 == bytes);
    }
}

TEST_CASE("parse 128")
{
    {
        std::vector<uint8_t> data = {0x80, 0x01};
        auto [vi, bytes] = mc::getLength(data);
        REQUIRE(128 == vi);
        REQUIRE(2 == bytes);
    }
}

TEST_CASE("parse 255")
{
    {
        std::vector<uint8_t> data = {0xff, 0x01};
        auto [vi, bytes] = mc::getLength(data);
        REQUIRE(255 == vi);
        REQUIRE(2 == bytes);
    }
}

TEST_CASE("parse 25565")
{
    {
        std::vector<uint8_t> data = {0xdd, 0xc7, 0x01};
        auto [vi, bytes] = mc::getLength(data);
        REQUIRE(25565 == vi);
        REQUIRE(3 == bytes);
    }
}

TEST_CASE("parse 2097151")
{
    {
        std::vector<uint8_t> data = {0xff, 0xff, 0x7f};
        auto [vi, bytes] = mc::getLength(data);
        REQUIRE(2097151 == vi);
        REQUIRE(3 == bytes);
    }
}

TEST_CASE("parse 2147483647")
{
    {
        std::vector<uint8_t> data = {0xff, 0xff, 0xff, 0xff, 0x07};
        auto [vi, bytes] = mc::getLength(data);
        REQUIRE(2147483647 == vi);
        REQUIRE(5 == bytes);
    }
}

TEST_CASE("parse -1")
{
    {
        std::vector<uint8_t> data = {0xff, 0xff, 0xff, 0xff, 0x0f};
        auto [vi, bytes] = mc::getLength(data);
        REQUIRE(-1 == vi);
        REQUIRE(5 == bytes);
    }
}

TEST_CASE("parse -2147483648")
{
    std::vector<uint8_t> data = {0x80, 0x80, 0x80, 0x80, 0x08};
    auto [vi, bytes] = mc::getLength(data);
    int expected = -2147483648;
    REQUIRE(expected == vi);
    REQUIRE(5 == bytes);
}

