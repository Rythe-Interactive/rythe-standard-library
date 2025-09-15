#pragma once
#include "hash.hpp"

#if !defined(RYTHE_HAS_INT128) && (defined(RYTHE_MSVC) || defined(RYTHE_CLANG_MSVC))
#include <intrin.h>
#pragma intrinsic(_umul128)
#endif

#include "../containers/pair.hpp"

namespace rsl
{
    namespace internal
    {
        template <hash_mode Mode>
        constexpr bool hash_static_assert_check = Mode == hash_mode::fast_hash || Mode == hash_mode::protected_hash;

        // Based on rapidhash Copyright (C) 2024 Nicolas De Carli: https://github.com/Nicoshev/rapidhash
        // rapidhash is distributed under the BSD 2-Clause License (https://www.opensource.org/licenses/bsd-license.php)
        namespace hash
        {
            constexpr size_type default_seed = 0xbdd89aa982704029ull;
            constexpr uint64 default_secret[3] = { 0x2d358dccaa6c78a5ull, 0x8bb84b93962eacc9ull, 0x4b33a62ed433d4a3ull };

            template <hash_mode Mode>
            constexpr uint64 manual_mum(const uint64 multiplier, const uint64 multiplicand, uint64* highProduct) noexcept
            {
                const uint64 ha = multiplier >> 32;
                const uint64 hb = multiplicand >> 32;
                const uint64 la = static_cast<uint32>(multiplier);
                const uint64 lb = static_cast<uint32>(multiplicand);

                const uint64 rh = ha * hb;
                const uint64 rm0 = ha * lb;
                const uint64 rm1 = hb * la;
                const uint64 rl = la * lb;
                const uint64 t = rl + (rm0 << 32);
                uint64 c = static_cast<uint64>(t < rl);

                const uint64 lo = t + (rm1 << 32);
                c += static_cast<uint64>(lo < t);
                const uint64 hi = rh + (rm0 >> 32) + (rm1 >> 32) + c;

                if constexpr (Mode == hash_mode::fast_hash)
                {
                    *highProduct = hi;
                    return lo;
                }
                else
                {
                    *highProduct = multiplicand ^ hi;
                    return multiplier ^ lo;
                }
            }

            template <hash_mode Mode>
            constexpr void mum(uint64& a, uint64& b) noexcept
            {
                #if defined(RYTHE_MSVC) || defined(RYTHE_CLANG_MSVC)
                if (is_constant_evaluated())
                {
                    a = manual_mum<Mode>(a, b, &b);
                }
                else
                {
                    if constexpr (Mode == hash_mode::fast_hash)
                    {
                        a = _umul128(a, b, &b);
                    }
                    else
                    {
                        uint64 tmpB;
                        const uint64 tmpA = _umul128(a, b, &tmpB);
                        a ^= tmpA;
                        b ^= tmpB;
                    }
                }
                #else
                a = manual_mum<Mode>(a, b, &b);
                #endif
            }

            template <hash_mode Mode>
            [[nodiscard]] [[rythe_always_inline]] constexpr uint64 mix(uint64 a, uint64 b) noexcept
            {
                mum<Mode>(a, b);
                return a ^ b;
            }

            [[nodiscard]] [[rythe_always_inline]] constexpr uint64 load_small(const byte* data, const size_type k) noexcept
            {
                return (static_cast<uint64>(data[0]) << 56) | (static_cast<uint64>(data[k >> 1]) << 32) | data[k - 1];
            }

            [[nodiscard]] [[rythe_always_inline]] constexpr uint64 load32(const byte* data) noexcept
            {
                return static_cast<uint64>(unaligned_load<uint32>(data));
            }

            template <hash_mode Mode>
            [[nodiscard]] constexpr uint64 hash_bytes(const byte_view bytes, uint64 seed, const uint64 (& secret)[3]) noexcept
            {
                const byte* data = bytes.data();
                const size_type dataSize = bytes.size();
                uint64 a;
                uint64 b;

                seed ^= mix<Mode>(seed ^ secret[0], secret[1]) ^ dataSize;

                if (dataSize <= 16) [[likely]]
                {
                    if (dataSize >= 4) [[likely]]
                    {
                        const byte* last = data + dataSize - 4;
                        a = (load32(data) << 32) | load32(last);
                        const uint64 delta = ((dataSize & 24) >> (dataSize >> 3));
                        b = ((load32(data + delta) << 32) | load32(last - delta));
                    }
                    else if (dataSize > 0) [[likely]]
                    {
                        a = load_small(data, dataSize);
                        b = 0;
                    }
                    else
                    {
                        a = b = 0;
                    }
                }
                else
                {
                    size_type i = dataSize;
                    if (i > 48) [[unlikely]]
                    {
                        uint64 seed1 = seed;
                        uint64 seed2 = seed;

                        while (i >= 96) [[likely]]
                        {
                            seed = mix<Mode>(
                                    unaligned_load<uint64>(data) ^ secret[0],
                                    unaligned_load<uint64>(data + 8) ^ seed
                                    );
                            seed1 = mix<Mode>(
                                    unaligned_load<uint64>(data + 16) ^ secret[1],
                                    unaligned_load<uint64>(data + 24) ^ seed1
                                    );
                            seed2 = mix<Mode>(
                                    unaligned_load<uint64>(data + 32) ^ secret[2],
                                    unaligned_load<uint64>(data + 40) ^ seed2
                                    );
                            seed = mix<Mode>(
                                    unaligned_load<uint64>(data + 48) ^ secret[0],
                                    unaligned_load<uint64>(data + 56) ^ seed
                                    );
                            seed1 = mix<Mode>(
                                    unaligned_load<uint64>(data + 64) ^ secret[1],
                                    unaligned_load<uint64>(data + 72) ^ seed1
                                    );
                            seed2 = mix<Mode>(
                                    unaligned_load<uint64>(data + 80) ^ secret[2],
                                    unaligned_load<uint64>(data + 88) ^ seed2
                                    );
                            data += 96;
                            i -= 96;
                        }

                        if (i >= 48) [[unlikely]]
                        {
                            seed = mix<Mode>(
                                    unaligned_load<uint64>(data) ^ secret[0],
                                    unaligned_load<uint64>(data + 8) ^ seed
                                    );
                            seed1 = mix<Mode>(
                                    unaligned_load<uint64>(data + 16) ^ secret[1],
                                    unaligned_load<uint64>(data + 24) ^ seed1
                                    );
                            seed2 = mix<Mode>(
                                    unaligned_load<uint64>(data + 32) ^ secret[2],
                                    unaligned_load<uint64>(data + 40) ^ seed2
                                    );
                            data += 48;
                            i -= 48;
                        }

                        seed ^= seed1 ^ seed2;
                    }

                    if (i > 16)
                    {
                        seed = mix<Mode>(
                                unaligned_load<uint64>(data) ^ secret[2],
                                unaligned_load<uint64>(data + 8) ^ seed ^ secret[1]
                                );
                        if (i > 32)
                        {
                            seed = mix<Mode>(
                                    unaligned_load<uint64>(data + 16) ^ secret[2],
                                    unaligned_load<uint64>(data + 24) ^ seed
                                    );
                        }
                    }

                    a = unaligned_load<uint64>(data + i - 16);
                    b = unaligned_load<uint64>(data + i - 8);
                }

                a ^= secret[1];
                b ^= seed;
                mum<Mode>(a, b);

                return mix<Mode>(a ^ secret[0] ^ dataSize, b ^ secret[1]);
            }

            template <hash_mode Mode>
            [[nodiscard]] constexpr uint64 hash_int(const uint64 val)
            {
                return mix<Mode>(val, 0x9e3779b97f4a7c15ull);
            }
        } // namespace hash
    }     // namespace internal

    template <typename T, hash_mode Mode>
    struct hash_strategy<T*, Mode>
    {
        [[rythe_always_inline]] constexpr static id_type hash(const T*& val) noexcept
        {
            return internal::hash::hash_int<Mode>(force_cast<uint64>(val));
        }
    };

    #define RSL_HASH_INT(T)                                                                                                \
	template <hash_mode Mode>                                                                                          \
	struct hash_strategy<T, Mode>                                                                                      \
	{                                                                                                                  \
		[[rythe_always_inline]] constexpr static id_type hash(const T& val) noexcept                                   \
		{                                                                                                              \
			return internal::hash::hash_int<Mode>(insert_cast<uint64>(val));                                           \
		}                                                                                                              \
	};

    RSL_HASH_INT(uint8);

    RSL_HASH_INT(uint16);

    RSL_HASH_INT(uint32);

    RSL_HASH_INT(uint64);

    RSL_HASH_INT(int8);

    RSL_HASH_INT(int16);

    RSL_HASH_INT(int32);

    RSL_HASH_INT(int64);

    RSL_HASH_INT(bool);

    RSL_HASH_INT(char);

    #if defined(RYTHE_MSVC) || defined(RYTHE_CLANG_MSVC)
    RSL_HASH_INT(long);

    RSL_HASH_INT(unsigned long);
    #endif

    RSL_HASH_INT(char16_t);

    RSL_HASH_INT(char32_t);

    RSL_HASH_INT(wchar_t);

    #undef HASH_INT

    template <enum_type Enum, hash_mode Mode>
    struct hash_strategy<Enum, Mode>
    {
        [[rythe_always_inline]] constexpr static id_type hash(const Enum& val) noexcept
        {
            using underlying = std::underlying_type_t<Enum>;
            return hash_strategy<underlying, Mode>::hash(static_cast<underlying>(val));
        }
    };

    template <typename LHS, typename RHS, hash_mode Mode>
    struct hash_strategy<pair<LHS, RHS>, Mode>
    {
        [[rythe_always_inline]] constexpr static id_type hash(const pair<LHS, RHS>& val) noexcept
        {
            return combine_hash(hash_strategy<LHS, Mode>::hash(val.first), hash_strategy<RHS, Mode>::hash(val.second));
        }
    };

    template <hash_mode Mode>
    constexpr id_type hash_bytes(const byte_view bytes) noexcept
    {
        static_assert(internal::hash_static_assert_check<Mode>, "unknown hash mode.");
        return internal::hash::hash_bytes<Mode>(bytes, internal::hash::default_seed, internal::hash::default_secret);
    }

    constexpr id_type hash_bytes(const byte_view bytes) noexcept
    {
        return hash_bytes<hash_mode::default_hash>(bytes);
    }

    template <hash_mode Mode>
    constexpr id_type hash_string(const string_view str) noexcept
    {
        if (is_constant_evaluated())
        {
            byte* data = new byte[str.size()];
            constexpr_memcpy(data, str.data(), str.size());

            const id_type result = hash_bytes<Mode>(byte_view::from_buffer(data, str.size()));

            delete[] data;

            return result;
        }
        else
        {
            return hash_bytes<Mode>(byte_view::from_buffer(bit_cast<const byte*>(str.data()), str.size()));
        }
    }

    constexpr id_type hash_string(const string_view str) noexcept
    {
        return hash_string<hash_mode::default_hash>(str);
    }

    template <hash_mode Mode, typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
    constexpr id_type hash_array(array_view<const T, Iter, ConstIter> arr) noexcept
    {
        return hash_bytes<Mode>(byte_view::from_buffer(bit_cast<const byte*>(arr.data()), arr.size() * sizeof(T)));
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
    constexpr id_type hash_array(array_view<const T, Iter, ConstIter> arr) noexcept
    {
        return hash_array<hash_mode::default_hash>(arr);
    }

    template <hash_mode Mode, typename T>
    constexpr id_type hash_value(const T& val) noexcept
    {
        return hash_strategy<remove_cvr_t<T>, Mode>::hash(val);
    }

    template <typename T>
    constexpr id_type hash_value(const T& val) noexcept
    {
        return hash_value<hash_mode::default_hash>(val);
    }

    template <hash_mode Mode, same_as<id_type>... HashTypes>
    constexpr id_type combine_hash(id_type seed, const id_type hash, const HashTypes... hashes) noexcept
    {
        seed = internal::hash::mix<Mode>(seed + hash, 0x9ddfea08eb382d69ull);

        if constexpr (sizeof...(HashTypes) != 0)
        {
            return combine_hash<Mode>(seed, hashes...);
        }
        else
        {
            return seed;
        }
    }

    template <same_as<id_type>... HashTypes>
    constexpr id_type combine_hash(id_type seed, const id_type hash, const HashTypes... hashes) noexcept
    {
        return combine_hash<hash_mode::default_hash>(seed, hash, hashes...);
    }
} // namespace rsl
