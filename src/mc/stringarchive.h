#ifndef STRINGARCHIVE_H
#define STRINGARCHIVE_H

#include <sstream>
#include <iomanip>
#include <vector>
#include <cassert>

class StringArchive : public std::vector<uint8_t>
{
public:
    StringArchive() = default;
    explicit StringArchive(const std::string& packet)
    {
        size_t len = packet.length();
        assert((len % 2) == 0);
        reserve(len / 2);
        for(size_t i = 0; i < len; i += 2)
        {
            int x = std::stoi(packet.substr(i, 2), 0, 16);
            push_back(x);
        }
    }
    explicit StringArchive(std::vector<uint8_t>&& vec) : vector<uint8_t>{std::move(vec)} {}

    friend std::ostream& operator<<(std::ostream& os, const StringArchive& sa)
    {
        return os << sa.str();
    }

    void addWriteOffset(const size_t offset) { _writeOffset += offset; }

    void addReadOffset(const size_t offset) { _readOffset += offset; }

    [[nodiscard]] std::string str() const
    {
        std::stringstream ss;

        for(const auto a : *this)
        {
            ss << std::uppercase << std::setfill('0') << std::setw(2) << std::hex
               << (static_cast<int>(a) & 0xFF);
        }

        return ss.str();
    }

    std::vector<uint8_t>::iterator resumeWrite() { return begin() + _writeOffset; }
    std::vector<uint8_t>::iterator resumeRead() { return begin() + _readOffset; }

    size_t _writeOffset = 0;
    size_t _readOffset = 0;
};

#endif
