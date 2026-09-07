#if defined(RYTHE_MSVC)
    #include <intrin.h>
    #pragma intrinsic(_BitScanForward64)
    #pragma intrinsic(_BitScanReverse64)
#endif

namespace rsl
{
    enum struct [[rythe_closed_enum]] endian : uint16
    {
#if RYTHE_PLATFORM_WINDOWS
        little = 0,
        big = 1,
        native = little
#else
        little = __ORDER_LITTLE_ENDIAN__,
        big = __ORDER_BIG_ENDIAN__,
        native = __BYTE_ORDER__
#endif
    };

    [[rythe_always_inline]] [[maybe_unused]] static size_type count_leading_zeros(const size_type mask) noexcept
    {
#if defined(RYTHE_MSVC)
        unsigned long index;
        return _BitScanReverse64(&index, mask) ? static_cast<size_type>(index) : 64ull;
#elif defined(RYTHE_CLANG) || defined(RYTHE_GCC)
        return mask ? static_cast<size_type>(__builtin_clzll(mask)) : 64ull;
#else
        return npos;
#endif
    }

    [[rythe_always_inline]] [[maybe_unused]] static size_type count_trailing_zeros(const size_type mask) noexcept
    {
#if defined(RYTHE_MSVC)
        unsigned long index;
        return _BitScanForward64(&index, mask) ? static_cast<size_type>(index) : 64ull;
#elif defined(RYTHE_CLANG) || defined(RYTHE_GCC)
        return mask ? static_cast<size_type>(__builtin_ctzll(mask)) : 64ull;
#else
        return npos;
#endif
    }

    template <typename T, typename U>
    constexpr T force_value_cast(U value)
    {
        static_assert(alignof(T) == alignof(U), "Illegal cast of unaligned types.");
        static_assert(sizeof(T) <= sizeof(U), "Illegal cast of non size similar types.");

        return *bit_cast<T*>(&value);
    }

    template <typename To, typename From>
    [[rythe_always_inline]] constexpr To force_cast(From& f) noexcept
    {
        return *bit_cast<To*>(&f);
    }

    template <typename To, typename From>
    [[rythe_always_inline]] constexpr To force_cast(const From& f) noexcept
    {
        return *bit_cast<To*>(&f);
    }

    template <arithmetic_type To, arithmetic_type From>
    [[rythe_always_inline]] constexpr bool is_within_limits(const From& f) noexcept
    {
        if constexpr (is_signed_v<To> && is_signed_v<From>)
        {
            return f >= limits<To>::min && f <= limits<To>::max;
        }
        else if constexpr (is_unsigned_v<From>)
        {
            return f <= limits<To>::max;
        }
        else
        {
            return f >= static_cast<From>(0) && f <= limits<To>::max;
        }
    }

    template <arithmetic_type To, arithmetic_type From>
    [[rythe_always_inline]] constexpr To narrowing_cast(const From& f) noexcept
    {
        rsl_assert_msg_soft_high_impact(is_within_limits<To>(f), "Narrowing cast failed, value {} is out of range [{}, {}]", f, limits<To>::min, limits<To>::max)
        return static_cast<To>(f);
    }
} // namespace rsl
