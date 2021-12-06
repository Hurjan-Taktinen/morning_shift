#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include "nbt.h"
#include "types.h"

#include <algorithm>
#include <numeric>
#include <cassert>
#include <type_traits>

template<typename T>
inline size_t get_size(const T& t)
{
    return sizeof(t);
}

template<>
inline size_t get_size(const std::string& s)
{
    return s.length() * sizeof(uint8_t) + sizeof(size_t);
}

template<>
inline size_t get_size(const mc::VarInt& vint)
{
    size_t siz = 4;
    for(;siz>0;--siz)
    {
        if((vint._value >> siz*7) & 0xff)
            break;
    }
    return siz+1;
}

template<>
inline size_t get_size(const mc::ByteArray& bArr)
{
    size_t len = std::accumulate(
        bArr.begin(),
        bArr.end(),
        get_size(mc::VarInt(bArr.size())), // size included
        [](const uint8_t& a, const uint8_t& b) { return a + get_size(b); });

    return len;
}

template<>
inline size_t get_size(const mc::ByteArrayEnd& bEnd)
{
    size_t len = std::accumulate(
        bEnd.begin(),
        bEnd.end(),
        0, // size is not included
        [](const uint8_t& a, const uint8_t& b) { return a + get_size(b); });

    return len;
}

template<>
inline size_t get_size(const mc::String& str)
{
    size_t len = std::accumulate(
        str.str().begin(),
        str.str().end(),
        get_size(mc::VarInt(str.str().size())),
        [](const uint8_t& a, const uint8_t& b) { return a + get_size(b); });

    return len;
}

template<class T>
inline size_t get_size(const std::vector<T>& v)
{
    return std::accumulate(
        v.begin(),
        v.end(),
        sizeof(size_t),
        [](const T& a, const T& b) { return a + get_size(b); });
}

template<class T>
inline size_t get_size(const mc::Vector<T>& v)
{
    return std::accumulate(
        v.begin(),
        v.end(),
        get_size(mc::VarInt(v.size())),
        [](const T& a, const T& b) { return a + get_size(b); });
}

template<typename ... Ts>
inline size_t get_size(const std::tuple<Ts...>& tpl)
{
    return std::apply([](const auto&... item) -> size_t
        {
            return (get_size(item) + ...);
        }, tpl);
}

template<size_t size>
inline size_t get_size(const std::array<uint8_t, size>&)
{
    return sizeof(uint8_t) * size;
}

namespace serialHelp
{
    template <typename ARCHIVE, typename T>
    inline void serialize(ARCHIVE& a, const T& t);
    template <typename ARCHIVE>
    inline void serialize(ARCHIVE& a, const mc::VarInt& vint);
    template <typename ARCHIVE>
    inline void serialize(ARCHIVE& a, const std::string& str);
    template <typename ARCHIVE>
    inline void serialize(ARCHIVE& a, const mc::String& str);
    template <typename ARCHIVE, typename ... Ts>
    inline void serialize(ARCHIVE& a, const std::tuple<Ts...>& tpl);
    template <typename ARCHIVE, class T>
    inline void serialize(ARCHIVE& a, const std::vector<T>& v);
    template <typename ARCHIVE, class T>
    inline void serialize(ARCHIVE& a, const mc::Vector<T>& v);
    template <typename ARCHIVE>
    inline void serialize(ARCHIVE& a, const mc::ByteArray& bArr);
    template <typename ARCHIVE>
    inline void serialize(ARCHIVE& a, const mc::ByteArrayEnd& bEnd);

    template <typename ARCHIVE, typename T>
    inline void serialize(ARCHIVE& a, const T& t)
    {
        const uint8_t* ptr = reinterpret_cast<const uint8_t*>(&t);
        std::reverse_copy(ptr, ptr + sizeof(T), a.resumeWrite());
        a.addWriteOffset(sizeof(T));
    }

    template <typename ARCHIVE>
    inline void serialize(ARCHIVE& a, const mc::VarInt& vint)
    {
        uint32_t tmpInt = vint._value;
        size_t size = get_size(vint) - 1;
        for(size_t count = 0;count <= size;++count)
        {
            uint8_t tmp = (tmpInt >> count*7) & 0x7f;
            if(count<size)
                tmp |= 0x80;

            std::reverse_copy(&tmp, &tmp + sizeof(uint8_t), a.resumeWrite());
            a.addWriteOffset(sizeof(uint8_t));
        }
    }

    template <typename ARCHIVE>
    inline void serialize(ARCHIVE& a, const std::string& str)
    {
        serialHelp::serialize(a, str.length());

        for(const auto& item : str)
            serialHelp::serialize(a, item);
    }

    template <typename ARCHIVE>
    inline void serialize(ARCHIVE& a, const mc::String& str)
    {
        serialHelp::serialize(a, mc::VarInt(str.str().length()));

        for(const auto& item : str.str())
            serialHelp::serialize(a, item);
    }

    template <typename ARCHIVE, typename ... Ts>
    inline void serialize(ARCHIVE& a, const std::tuple<Ts...>& tpl)
    {
        std::apply([&a](const auto&... item)
            {
                (serialHelp::serialize(a, item), ...);
            },
            tpl);
    }

    template <typename ARCHIVE, size_t size>
    inline void serialize(ARCHIVE& a, const std::array<uint8_t, size>& arr)
    {
        for(const auto& item : arr)
            serialHelp::serialize(a, item);
    }

    template <typename ARCHIVE, class T>
    inline void serialize(ARCHIVE& a, const std::vector<T>& v)
    {
        serialHelp::serialize(a, v.size());

        for(const auto& item : v)
            serialHelp::serialize(a, item);
    }

    template <typename ARCHIVE, class T>
    inline void serialize(ARCHIVE& a, const mc::Vector<T>& bArr)
    {
        serialHelp::serialize(a, mc::VarInt(bArr.size()));

        for(const auto& item : bArr)
            serialHelp::serialize(a, item);
    }

    template <typename ARCHIVE>
    inline void serialize(ARCHIVE& a, const mc::ByteArray& bArr)
    {
        serialHelp::serialize(a, mc::VarInt(bArr.size()));

        for(const auto& item : bArr)
            serialHelp::serialize(a, item);
    }

    template <typename ARCHIVE>
    inline void serialize(ARCHIVE& a, const mc::ByteArrayEnd& bEnd)
    {
        for(const auto& item : bEnd)
        {
            serialHelp::serialize(a,item);
        }
    }
}

template<typename ARCHIVE, typename T>
inline void serialize(ARCHIVE& a, const T& t)
{
    size_t size = get_size(t);
    a.resize(a.size() + size, 0);

    serialHelp::serialize(a, t);
}

template<class T>
struct Deserialize
{
    template <typename ARCHIVE>
    static T deserialize(ARCHIVE& a)
    {
        assert(a.resumeRead() + sizeof(T) <= a.end());
        T val;
        uint8_t* ptr = reinterpret_cast<uint8_t*>(&val);
        std::reverse_copy(a.resumeRead(), a.resumeRead() + sizeof(T), ptr);
        a.addReadOffset(sizeof(T));

        return val;
    }
};

template<>
struct Deserialize<std::string>
{
    template <typename ARCHIVE>
    static std::string deserialize(ARCHIVE& a)
    {
        const size_t size = Deserialize<size_t>::deserialize(a);

        if(size == 0u)
            return std::string();

        std::string str;
        for(size_t i=0; i<size;++i)
        {
            str.push_back(Deserialize<uint8_t>::deserialize(a));
        }
        return str;
    }
};

template<>
struct Deserialize<mc::VarInt>
{
    template <typename ARCHIVE>
    static mc::VarInt deserialize(ARCHIVE& a)
    {
        uint32_t tmp = 0;
        for(int i = 0; i<5; ++i)
        {
            uint32_t c = Deserialize<uint8_t>::deserialize(a);

            tmp |=  ((c & 0x7F) << 7*i) ;

            if(!(c & 0x80))
                break;
        }

        return mc::VarInt(tmp);
    }
};

template<>
struct Deserialize<mc::String>
{
    template <typename ARCHIVE>
    static mc::String deserialize(ARCHIVE& a)
    {
        const mc::VarInt size = Deserialize<mc::VarInt>::deserialize(a);

        if(size == 0u)
            return mc::String();

        std::string str;
        for(int32_t i=0; i<size._value;++i)
        {
            str.push_back(Deserialize<uint8_t>::deserialize(a));
        }
        return mc::String(std::move(str));
    }
};

template<class T>
struct Deserialize<std::vector<T>>
{
    template <typename ARCHIVE>
    static std::vector<T> deserialize(ARCHIVE& a)
    {
        const size_t size = Deserialize<size_t>::deserialize(a);

        std::vector<T> vec;
        for(size_t i=0; i<size;++i)
        {
            vec.push_back(Deserialize<T>::deserialize(a));
        }
        return vec;
    }
};

template<size_t size>
struct Deserialize<std::array<uint8_t, size>>
{
    template <typename ARCHIVE>
    static std::array<uint8_t, size> deserialize(ARCHIVE& a)
    {
        std::array<uint8_t, size> arr;
        for(size_t i=0; i<size;++i)
        {
            arr[i]= Deserialize<uint8_t>::deserialize(a);
        }
        return arr;
    }
};

template <typename ... Ts>
struct Deserialize<std::tuple<Ts...>>
{
    template <size_t N, typename ARCHIVE>
    static void unpack(ARCHIVE& a, std::tuple<Ts...>& tpl)
    {
        using Tupla = std::tuple<Ts...>;

        static_assert(N < 1u + sizeof...(Ts));

        constexpr size_t indexi = std::tuple_size<Tupla>::value-N-1;
        typedef typename std::tuple_element<indexi, Tupla>::type T;

        std::get<indexi>(tpl) = std::move(Deserialize<T>::deserialize(a));

        if constexpr(0u == N)
        {
            return;
        }
        else
        {
            unpack<N-1u>(a, tpl);
        }
    }

    template <typename ARCHIVE>
    static std::tuple<Ts...> deserialize(ARCHIVE& a)
    {
        using Tupla = std::tuple<Ts...>;
        Tupla tpl;

        unpack<sizeof...(Ts)-1>(a, tpl);

        return tpl;
    }

    // try this
    // template <typename... T, std::size_t... I>
    // auto subtuple_(const std::tuple<T...>& t, std::index_sequence<I...>) {
    //     return std::make_tuple(std::get<I>(t)...);
    // }

    // template <int Trim, typename... T>
    // auto subtuple(const std::tuple<T...>& t) {
    //     return subtuple_(t, std::make_index_sequence<sizeof...(T) - Trim>());
    // }
};

template <>
struct Deserialize<mc::ByteArray>
{
    template <typename ARCHIVE>
    static mc::ByteArray deserialize(ARCHIVE& a)
    {
        mc::ByteArray bArr;

        const mc::VarInt size = Deserialize<mc::VarInt>::deserialize(a);

        for(int32_t i=0;i<size._value;++i)
        {
            bArr._v.push_back(Deserialize<uint8_t>::deserialize(a));
        }

        return bArr;
    }
};

template <typename TYPE>
struct Deserialize<mc::Vector<TYPE>>
{
    template <typename ARCHIVE>
    static mc::Vector<TYPE> deserialize(ARCHIVE& a)
    {
        mc::Vector<TYPE> bArr;

        const mc::VarInt size = Deserialize<mc::VarInt>::deserialize(a);

        for(int32_t i=0;i<size._value;++i)
        {
            bArr.push_back(Deserialize<TYPE>::deserialize(a));
        }

        return bArr;
    }
};

template <>
struct Deserialize<mc::ByteArrayEnd>
{
    template <typename ARCHIVE>
    static mc::ByteArrayEnd deserialize(ARCHIVE& a)
    {
        mc::ByteArrayEnd bEnd;

        while(a.resumeRead() != a.end())
        {
            bEnd._v.push_back(Deserialize<uint8_t>::deserialize(a));
        }

        return bEnd;
    }
};

template <>
struct Deserialize<mc::Nbt>
{
    template <typename ARCHIVE>
    static mc::Nbt deserialize(ARCHIVE& a)
    {
        mc::Nbt nbt = mc::Nbt::unpackNtb(a);
        return nbt;
    }
};

#endif
