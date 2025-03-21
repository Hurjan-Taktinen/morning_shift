#pragma once

#include "stringarchive.h"

#include <algorithm>
#include <iostream>
#include <vector>
#include <sstream>
#include <cstdint>

#include <numeric>
#include <concepts>
#include <variant>
#include <map>

namespace mc
{

enum NbtTagType
{
    TAG_End = 0,
    TAG_Byte = 1,
    TAG_Short = 2,
    TAG_Int = 3,
    TAG_Long = 4,
    TAG_Float = 5,
    TAG_Double = 6,
    TAG_Byte_Array = 7,
    TAG_String = 8,
    TAG_List = 9,
    TAG_Compound = 10,
    TAG_Int_Array = 11,
    TAG_Long_Array = 12,
};

using EndTag = std::nullptr_t;
using ByteTag = uint8_t;
using ShortTag = uint16_t;
using IntTag = uint32_t;
using LongTag = uint64_t;
using FloatTag = float;
using DoubleTag = double;
using ByteArrayTag = std::vector<uint8_t>;
using NbtString = std::string;
struct ListTag;
struct NbtCompound;
using IntArrayTag = std::vector<uint32_t>;
using LongArrayTag = std::vector<uint64_t>;

using TagVariant = std::variant<
        EndTag,
        ByteTag,
        ShortTag,
        IntTag,
        LongTag,
        FloatTag,
        DoubleTag,
        ByteArrayTag,
        NbtString,
        ListTag,
        NbtCompound,
        IntArrayTag,
        LongArrayTag>;

struct NbtCompound
{
    NbtString name;
    std::map<NbtString, TagVariant> data;
};

struct ListTag
{
    std::vector<TagVariant> data;
};

inline std::string printTagType(NbtTagType tag)
{
    switch(tag)
    {
    case TAG_End: return "TAG_End";
    case TAG_Byte: return "TAG_Byte";
    case TAG_Short: return "TAG_Short";
    case TAG_Int: return "TAG_Int";
    case TAG_Long: return "TAG_Long";
    case TAG_Float: return "TAG_Float";
    case TAG_Double: return "TAG_Double";
    case TAG_Byte_Array: return "TAG_Byte_Array";
    case TAG_String: return "TAG_String";
    case TAG_List: return "TAG_List";
    case TAG_Compound: return "TAG_Compound";
    case TAG_Int_Array: return "TAG_Int_Array";
    case TAG_Long_Array: return "TAG_Long_Array";
    }
    return "TAG_End";
}

inline std::string printNamed(const std::string& name, const std::string& tagStr)
{
    return tagStr + "('" + (name.empty() ? "None" : name) + "'):";
}

template<typename TYPE>
inline std::string printNbtVector(const std::vector<TYPE>& v)
{
    std::stringstream ss;

    ss << "\n[\n";
    for(const auto& a : v)
    {
        ss << "\t" << a << "\n";
    }
    ss << "\n]\n";
    return ss.str();
}

inline std::string printCombo(const NbtCompound& combo)
{
    std::stringstream ss;
    ss << printNamed(combo.name, printTagType(TAG_Compound)) << "\n{\n";
    for(const auto& [ename, element] : combo.data)
    {
        ss << printNamed(ename, printTagType(static_cast<NbtTagType>(element.index())));

        switch(element.index())
        {
        case TAG_End: break;
        case TAG_Byte: break;
        case TAG_Short: break;
        case TAG_Int: break;
        case TAG_Long: break;
        case TAG_Float: break;
        case TAG_Double: break;
        case TAG_Byte_Array: break;
        case TAG_String: ss << " '" << std::get<NbtString>(element) << "'\n"; break;
        case TAG_List: break;
        case TAG_Compound: printCombo(std::get<NbtCompound>(element)); break;
        case TAG_Int_Array: ss << printNbtVector(std::get<IntArrayTag>(element)); break;
        case TAG_Long_Array: ss << printNbtVector(std::get<LongArrayTag>(element)); break;
        }
    }
    ss << "}";
    return ss.str();
}

template<std::integral T>
inline auto unpack(StringArchive& packet)
{
    T val;
    uint8_t* ptr = reinterpret_cast<uint8_t*>(&val);
    std::reverse_copy(packet.resumeRead(), packet.resumeRead() + sizeof(val), ptr);
    packet.addReadOffset(sizeof(val));
    return val;
}

inline auto unpackString(StringArchive& packet)
{
    auto size = unpack<uint16_t>(packet);
    std::string s;
    s.resize(size);
    //is.read(s.data(), size);
    std::copy(packet.resumeRead(), packet.resumeRead() + size, s.begin());
    packet.addReadOffset(size);
    return s;
}

template<typename BASE>
inline std::vector<BASE> unpackVector(StringArchive& packet)
{
    auto size = unpack<uint32_t>(packet);
    std::vector<BASE> vec(size);
    for(uint32_t i = 0; i < size; ++i)
    {
        vec.push_back(unpack<BASE>(packet));
    }
    return vec;
}

inline ListTag unpackList(StringArchive&)
{
    ListTag list;

    return list;
}

inline NbtCompound unpackCombo(StringArchive& packet)
{
    NbtCompound combo;
    combo.name = unpackString(packet);

    auto tag = unpack<uint8_t>(packet);
    for(; tag != TAG_End; tag = unpack<uint8_t>(packet))
    {
        auto name = unpackString(packet);
        auto& element = combo.data[name];
        switch(tag)
        {
        case TAG_End: break;
        case TAG_Byte: element = unpack<uint8_t>(packet); break;
        case TAG_Short: element = unpack<uint16_t>(packet); break;
        case TAG_Int: element = unpack<uint32_t>(packet); break;
        case TAG_Long: element = unpack<uint64_t>(packet); break;
        case TAG_Float: element = unpack<uint32_t>(packet); break;
        case TAG_Double: element = unpack<uint64_t>(packet); break;
        case TAG_Byte_Array: element = unpackVector<uint8_t>(packet); break;
        case TAG_String: element = unpackString(packet); break;
        case TAG_List: element = unpackList(packet); break;
        case TAG_Compound: element = unpackCombo(packet); break;
        case TAG_Int_Array: element = unpackVector<uint32_t>(packet); break;
        case TAG_Long_Array: element = unpackVector<uint64_t>(packet); break;
        }
    }

    return combo;
}

struct Nbt
{
    static Nbt unpackNtb(StringArchive& packet)
    {
        Nbt nbt;

        auto tag = unpack<uint8_t>(packet);
        if(tag == 0x0a)
        {
            nbt.combo = unpackCombo(packet);
        }
        return nbt;
    }

    NbtCompound combo;

    std::string print() { return printCombo(combo); }
};

} // namespace mc
