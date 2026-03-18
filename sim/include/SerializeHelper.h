#pragma once
#include <vector>
#include <string>
#include <stdint.h>
typedef uint8_t Byte;
typedef std::vector<Byte> ByteVector;

#define DLIB_MAX_STRING_LEGTH 256

// WRITE UINT 8, 16, 32, 64

inline void write_u8_be(ByteVector& buf, uint8_t v)
{
    buf.push_back(static_cast<Byte>(v));
}

inline void write_u16_be(ByteVector& buf, uint16_t v)
{
    buf.push_back(static_cast<Byte>(v >> 8));
    buf.push_back(static_cast<Byte>(v));
}

inline void write_u32_be(ByteVector& buf, uint32_t v)
{
    buf.push_back(static_cast<Byte>(v >> 24));
    buf.push_back(static_cast<Byte>(v >> 16));
    buf.push_back(static_cast<Byte>(v >> 8));
    buf.push_back(static_cast<Byte>(v));
}

inline void write_u64_be(ByteVector& buf, uint64_t v)
{
    buf.push_back(static_cast<Byte>(v >> 56));
    buf.push_back(static_cast<Byte>(v >> 48));
    buf.push_back(static_cast<Byte>(v >> 40));
    buf.push_back(static_cast<Byte>(v >> 32));
    buf.push_back(static_cast<Byte>(v >> 24));
    buf.push_back(static_cast<Byte>(v >> 16));
    buf.push_back(static_cast<Byte>(v >> 8));
    buf.push_back(static_cast<Byte>(v));
}

// READ UINT 8, 16, 32, 64

inline uint8_t read_u8_be(const Byte* begin, const Byte*& next)
{
    next = &begin[1];
    return Byte(begin[0]);
}

inline uint16_t read_u16_be(const Byte* begin, const Byte*& next)
{
    next = &begin[1+1];
    return (uint16_t(begin[0]) << 8 |
            uint16_t(begin[1])      );
}

inline uint32_t read_u32_be(const Byte* begin, const Byte*& next)
{
    next = &begin[3+1];
    return (uint32_t(begin[0]) << 24 |
            uint32_t(begin[1]) << 16 | 
            uint32_t(begin[2]) << 8  |
            uint32_t(begin[3])       );
}

inline uint64_t read_u64_be(const Byte* begin, const Byte*& next)
{
    next = &begin[7+1];
    return (uint64_t(begin[0]) << 56 |
            uint64_t(begin[1]) << 48 |
            uint64_t(begin[2]) << 40 |
            uint64_t(begin[3]) << 32 |
            uint64_t(begin[4]) << 24 |
            uint64_t(begin[5]) << 16 |
            uint64_t(begin[6]) << 8  |
            uint64_t(begin[7])       );
}

// WRITE STRING

inline void write_string(ByteVector& buf, const std::string& v)
{
    size_t len = v.length();
    for (size_t pos = 0; pos < len && pos < DLIB_MAX_STRING_LEGTH; pos++)
    {
        buf.push_back(static_cast<Byte>(v[pos]));
    }
    buf.push_back(static_cast<Byte>('\0'));
}

// READ STRING

inline std::string read_string(const Byte* begin, const Byte*& next)
{
    std::string out;

    for(size_t pos = 0;
        static_cast<char>(begin[pos]) != '\0' && pos < DLIB_MAX_STRING_LEGTH;
        out.push_back(static_cast<char>(begin[pos])), pos++);
    next = &begin[out.length()+1];
    return out;
}