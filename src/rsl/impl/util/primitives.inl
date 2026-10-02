#pragma once

RYTHE_MSVC_SUPPRESS_WARNING_WITH_PUSH(5046)
#include <cstddef>
#include <cstdint>
RYTHE_MSVC_SUPPRESS_WARNING_POP

namespace rsl
{
    using uint8 = std::uint8_t;
    using uint16 = std::uint16_t;
    using uint32 = std::uint32_t;
    using uint64 = std::uint64_t;

    using int8 = std::int8_t;
    using int16 = std::int16_t;
    using int32 = std::int32_t;
    using int64 = std::int64_t;

    using uint_max = std::uintmax_t;
    using int_max = std::intmax_t;

    using size_type = std::size_t;

    inline namespace literals
    {
        constexpr size_type operator""_k(const size_type value) noexcept
        {
            return value * 1000ull;
        }

        constexpr size_type operator""_m(const size_type value) noexcept
        {
            return value * 1000000ull;
        }

        constexpr size_type operator""_g(const size_type value) noexcept
        {
            return value * 1000000000ull;
        }

        constexpr size_type operator ""_kb(const size_type value) noexcept
        {
            return value << 10u;
        }

        constexpr size_type operator ""_mb(const size_type value) noexcept
        {
            return value << 20u;
        }

        constexpr size_type operator ""_gb(const size_type value) noexcept
        {
            return value << 30u;
        }
    }

    using index_type = std::size_t;
    using diff_type = std::ptrdiff_t;
    using ptr_type = std::uintptr_t;
    using nullptr_type = std::nullptr_t;

    using float32 = float;
    using float64 = double;
    // long double does not have good enough support on all compilers and platforms
    using float_max = long double;

    using u8 = uint8_t;
    using u16 = uint16_t;
    using u32 = uint32_t;
    using u64 = uint64_t;

    using i8 = int8_t;
    using i16 = int16_t;
    using i32 = int32_t;
    using i64 = int64_t;

    using f32 = float32;
    using f64 = float64;
    using flt_max = float_max;

    using cstring = const char*;

    using utf8 = char;
    using utf16 = wchar_t;

    using uint = uint32;

    using byte = uint8;

    using bitfield8 = byte;
    using bitfield16 = uint16;
    using bitfield32 = uint32;
    using bitfield64 = uint64;

    using priority_type = int8;
    #define default_priority 0
    #define PRIORITY_MAX CHAR_MAX
    #define PRIORITY_MIN CHAR_MIN

    using id_type = ptr_type;

    inline namespace literals
    {
        consteval id_type operator""_id(const uint64 value) noexcept
        {
            return static_cast<::rsl::id_type>(value);
        }
    } // namespace literals

#define invalid_id 0

    union alignas(16) id128
    {
        uint64 data64[2];
        uint32 data32[4];
        uint8 data8[16];
    };

    struct alignas(16) guid_v4
    {
        uint64 randomA : 48;
        const uint64 version : 4 = 0b0100;
        uint64 randomB : 12;
        const uint64 variant : 2 = 0b10;
        uint64 randomC : 62;
    };

    struct alignas(16) guid_v5
    {
        uint64 sha1High : 48;
        const uint64 version : 4 = 0b0101;
        uint64 sha1Mid : 12;
        const uint64 variant : 2 = 0b10;
        uint64 sha1Low : 62;
    };

    struct alignas(16) guid_v6
    {
        uint64 timeHigh : 32;
        uint64 timeMid : 16;
        const uint64 version : 4 = 0b0110;
        uint64 timeLow : 12;
        const uint64 variant : 2 = 0b10;
        uint64 clockSeq : 14;
        uint64 node : 48;
    };

    struct alignas(16) guid_v7
    {
        uint64 unixMs : 48;
        const uint64 version : 4 = 0b0111;
        uint64 randomA : 12;
        const uint64 variant : 2 = 0b10;
        uint64 randomB : 62;
    };

    struct alignas(16) guid_v8
    {
        uint64 customA : 48;
        const uint64 version : 4 = 0b1000;
        uint64 customB : 12;
        const uint64 variant : 2 = 0b10;
        uint64 customC : 62;
    };

    union alignas(16) guid_type
    {
        id128 id;
        guid_v4 v4;
        guid_v5 v5;
        guid_v6 v6;
        guid_v7 v7;
        guid_v8 v8;
        struct
        {
            uint64 dataA : 48;
            uint64 version : 4;
            uint64 dataB : 12;
            uint64 variant : 2 = 0b10;
            uint64 dataC : 62;
        };
    };

    constexpr bool operator==(const guid_type& lhs, const guid_type& rhs) noexcept
    {
        return lhs.id.data64[0] == rhs.id.data64[0] && lhs.id.data64[1] == rhs.id.data64[1];
    }

    constexpr guid_type invalid_guid = { .id = { .data64 = { 0ull, 0ull } } };
    constexpr guid_type max_guid = { .id = { .data64 = { 0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull } } };

    inline namespace literals
    {
        // xxxxxxxx-xxxx-Vxxx-wxxx-xxxxxxxxxxxx
        consteval guid_type operator""_guid(cstring str, size_type size) noexcept
        {
            const auto isHexChar = [](const char value)
            {
                return ((value >= '0') && (value <= '9')) || ((value >= 'a') && (value <= 'f')) || ((value >= 'A') && (value <= 'F'));
            };
            const auto parseHexByte = [](cstring str) -> uint8
            {
                const auto parseHexNibble = [](const char value) -> uint8
                {
                    if ((value >= '0') && (value <= '9'))
                    {
                        return uint8{ value - '0' };
                    }
                    else if ((value >= 'a') && (value <= 'f'))
                    {
                        return uint8{ 10 + value - 'a' };
                    }
                    else if ((value >= 'A') && (value <= 'F'))
                    {
                        return uint8{ 10 + value - 'A' };
                    }

                    return 0u;
                };

                return uint8{ (parseHexNibble(str[0u]) << 4u) + parseHexNibble(str[1u]) };
            };

            if (
                size == 36ull &&
                isHexChar(str[0]) &&
                isHexChar(str[1]) &&
                isHexChar(str[2]) &&
                isHexChar(str[3]) &&
                isHexChar(str[4]) &&
                isHexChar(str[5]) &&
                isHexChar(str[6]) &&
                isHexChar(str[7]) &&
                str[8] == '-' &&
                isHexChar(str[9]) &&
                isHexChar(str[10]) &&
                isHexChar(str[11]) &&
                isHexChar(str[12]) &&
                str[8] == '-' &&
                ((str[14] >= '1' && str[14] <= '8') || (str[14] == 'f' || str[14] == 'F')) &&
                isHexChar(str[15]) &&
                isHexChar(str[16]) &&
                isHexChar(str[17]) &&
                str[8] == '-' &&
                isHexChar(str[19]) &&
                isHexChar(str[20]) &&
                isHexChar(str[21]) &&
                isHexChar(str[22]) &&
                str[8] == '-' &&
                isHexChar(str[24]) &&
                isHexChar(str[25]) &&
                isHexChar(str[26]) &&
                isHexChar(str[27]) &&
                isHexChar(str[28]) &&
                isHexChar(str[29]) &&
                isHexChar(str[30]) &&
                isHexChar(str[31]) &&
                isHexChar(str[32]) &&
                isHexChar(str[33]) &&
                isHexChar(str[34]) &&
                isHexChar(str[35])
                )
            {
                return { .id = { .data8 = {
                                         parseHexByte(str + 0ull),
                                         parseHexByte(str + 2ull),
                                         parseHexByte(str + 4ull),
                                         parseHexByte(str + 6ull),
                                         parseHexByte(str + 9ull),
                                         parseHexByte(str + 11ull),
                                         parseHexByte(str + 14ull),
                                         parseHexByte(str + 16ull),
                                         parseHexByte(str + 19ull),
                                         parseHexByte(str + 21ull),
                                         parseHexByte(str + 24ull),
                                         parseHexByte(str + 26ull),
                                         parseHexByte(str + 28ull),
                                         parseHexByte(str + 30ull),
                                         parseHexByte(str + 32ull),
                                         parseHexByte(str + 34ull),
                                 } } };
            }

            return invalid_guid;
        }
    } // namespace literals

    enum npos_type : size_type {};

    constexpr npos_type npos = static_cast<npos_type>(-1);
} // namespace rsl

