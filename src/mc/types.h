#ifndef MCTYPE_H
#define MCTYPE_H

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <bitset>

namespace mc
{
class String;

class Uuid : public std::array<uint8_t, 16>
{
public:
    auto operator<=>(const Uuid&) const = default;
    std::string print()
    {
        // just lol TODO:
        std::ostringstream os;
        os << std::hex << std::setw(2) << std::setfill('0');
        os << static_cast<int>(this->at(0));
        os << static_cast<int>(this->at(1));
        os << static_cast<int>(this->at(2));
        os << static_cast<int>(this->at(3));
        os << "-";
        os << static_cast<int>(this->at(4));
        os << static_cast<int>(this->at(5));
        os << "-";
        os << static_cast<int>(this->at(6));
        os << static_cast<int>(this->at(7));
        os << "-";
        os << static_cast<int>(this->at(8));
        os << static_cast<int>(this->at(9));
        os << "-";
        os << static_cast<int>(this->at(10));
        os << static_cast<int>(this->at(11));
        os << static_cast<int>(this->at(12));
        os << static_cast<int>(this->at(13));
        os << static_cast<int>(this->at(14));
        os << static_cast<int>(this->at(15));
        return os.str();
    }
};

class Angle : public std::array<uint8_t, 1>
{
};

class Bool : public std::array<uint8_t, 1>
{
};

struct Position : public std::array<uint64_t, 1>
{
    std::string dump() const
    {
        std::bitset<64> bits{this->at(0)};
        return bits.to_string();
    }

    int getX() const
    {
        std::bitset<26> bits{this->at(0) >> 38};
        return bits.test(25) ? (bits.to_ulong() + 0xFFFFFFFFFC000000) : bits.to_ulong();
    }

    int getY() const
    {
        std::bitset<12> bits{this->at(0)};
        return bits.test(11) ? (bits.to_ulong() + 0xFFFFFFFFFFFFF000) : bits.to_ulong();
    }

    int getZ() const
    {
        std::bitset<26> bits{this->at(0) << 26 >> 38};
        return bits.test(25) ? (bits.to_ulong() + 0xFFFFFFFFFC000000) : bits.to_ulong();
    }
};

class VarInt
{
public:
    typedef int32_t type;

    constexpr VarInt() = default;
    constexpr VarInt(type i) : _value(i) {}

    auto operator<=>(const VarInt& rhs) const = default;

    std::string hex() const
    {
        std::stringstream ss;

        ss << std::uppercase << std::setfill('0') << std::setw(2) << std::hex
           << (static_cast<int>(_value) & 0xFF);

        return ss.str();
    }

    friend std::ostream& operator<<(std::ostream& os, const VarInt& vint)
    {
        os << vint._value;
        return os;
    }

    int32_t _value = 0;
};

class ByteArray
{
public:
    ByteArray() {}

    ByteArray(const size_t& size) : _v(size) {}

    ByteArray(std::initializer_list<uint8_t> l) : _v(l) {}

    bool operator==(const ByteArray& rhs) const { return _v == rhs._v; }

    bool operator!=(const ByteArray& rhs) const { return !(*this == rhs); }

    friend std::ostream& operator<<(std::ostream& os, const ByteArray& bArr)
    {
        os << "bArr: size=" << bArr.size() << std::hex << std::setw(2) << std::setfill('0')
           << " data=[";
        for(const auto& item : bArr._v)
            os << static_cast<int>(item);
        os << "]";
        return os;
    }

    std::vector<uint8_t>::const_iterator begin() const { return _v.begin(); }

    std::vector<uint8_t>::const_iterator end() const { return _v.end(); }

    std::size_t size() const { return _v.size(); }

    std::vector<uint8_t> _v;
};

template<typename TYPE>
class Vector : public std::vector<TYPE>
{
    auto operator<=>(const Vector<TYPE>& rhs) const = default;

    friend std::ostream& operator<<(std::ostream& os, const Vector<TYPE>& bArr)
    {
        os << "bArr: size=" << bArr.size() << std::hex << std::setw(2) << std::setfill('0')
           << " data=[";
        for(const auto& item : bArr)
            os << item;
        os << "]";
        return os;
    }
};

// template<class T>
// class Array
// {
// public:
//     using type = std::vector<T>;
//     Array() {}

//     Array(const size_t& size) :
//         _v(size)
//     {}

//     Array(std::initializer_list<T> l) :
//         _v(l)
//     {}

//     bool operator==(const Array<T>& rhs) const
//     {
//         return _v == rhs._v;
//     }

//     bool operator!=(const Array<T>& rhs) const
//     {
//         return !(*this == rhs);
//     }

//     friend std::ostream& operator<<(std::ostream& os, const Array<T>& arr)
//     {
//         os << "arr<" << typeName<T>() << "> size=" << arr.size() << "
//         data=["; for(const auto& item : arr._v)
//             os << static_cast<int>(item);
//         os << "]";
//         return os;
//     }

//     type::const_iterator begin() const
//     {
//         return _v.begin();
//     }

//     type::const_iterator end() const
//     {
//         return _v.end();
//     }

//     std::size_t size() const
//     {
//         return _v.size();
//     }

//     type _v;
// };

class ByteArrayEnd
{
public:
    ByteArrayEnd() {}

    ByteArrayEnd(const size_t& size) : _v(size) {}

    ByteArrayEnd(std::initializer_list<uint8_t> l) : _v(l) {}

    ByteArrayEnd(const std::string& str) : _v(str.begin(), str.end()) {}

    bool operator==(const ByteArrayEnd& rhs) const { return _v == rhs._v; }

    bool operator!=(const ByteArrayEnd& rhs) const { return !(*this == rhs); }

    friend std::ostream& operator<<(std::ostream& os, const ByteArrayEnd& bArr)
    {
        os << "bArr: size=" << bArr.size() << " data=[";
        for(const auto& item : bArr._v)
            os << static_cast<int>(item) << " ";
        os << "]";
        return os;
    }

    std::vector<uint8_t>::const_iterator begin() const { return _v.begin(); }

    std::vector<uint8_t>::const_iterator end() const { return _v.end(); }

    std::size_t size() const { return _v.size(); }

    std::vector<uint8_t> _v;
};

class String
{
public:
    String() {}
    String(const std::string& str) : _value(str) {}

    bool operator==(const String& rhs) const { return _value == rhs._value; }

    bool operator!=(const String& rhs) const { return !(*this == rhs); }

    friend std::ostream& operator<<(std::ostream& os, const String& str)
    {
        os << str.str();
        return os;
    }

    const std::string& str() const { return _value; }

    std::string& getValue() { return _value; }

    std::string _value;
};
} // namespace mc

#endif
