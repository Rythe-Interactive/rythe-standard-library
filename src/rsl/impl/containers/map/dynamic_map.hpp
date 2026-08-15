#pragma once

#include "hash_map.hpp"
#include "map_info.hpp"

namespace rsl
{
    template <
            typename Key,
            typename Value,
            hash_map_flags Flags = hash_map_flags::default_flags,
            typename Hash = ::rsl::hash<Key>,
            typename KeyEqual = equal<Key>,
            ratio_type MaxLoadFactor = ratio<80, 100>,
            size_type FingerprintSize = internal::recommended_fingerprint_size<hash_map_flags_is_large(Flags)>>
    class dynamic_map : public hash_map_base<map_info<Key, Value, Flags, Hash, KeyEqual, MaxLoadFactor, FingerprintSize>>
    {
    public:
        using container_base = hash_map_base<map_info<Key, Value, Flags, Hash, KeyEqual, MaxLoadFactor, FingerprintSize>>;

        static constexpr bool is_flat = container_base::is_flat;
        static constexpr bool is_large = container_base::is_large;
        static constexpr bool is_multi = container_base::is_multi;

        using key_type = typename container_base::key_type;
        using mapped_type = typename container_base::mapped_type;
        using hasher_type = typename container_base::hasher_type;
        using key_comparer_type = typename container_base::key_comparer_type;

        using iterator_type = typename container_base::iterator_type;
        using const_iterator_type = typename container_base::const_iterator_type;
        using reverse_iterator_type = typename container_base::reverse_iterator_type;
        using const_reverse_iterator_type = typename container_base::const_reverse_iterator_type;

        using view_type = typename container_base::view_type;
        using const_view_type = typename container_base::const_view_type;

        using hash_map_base<map_info<Key, Value, Flags, Hash, KeyEqual, MaxLoadFactor, FingerprintSize>>::hash_map_base;

        [[rythe_always_inline]] constexpr dynamic_map(const container_base& src)
                noexcept(container_base::copy_construct_container_noexcept);
        // TODO(Glyn): moving maps and sets
        [[rythe_always_inline]] constexpr dynamic_map(container_base&& src)
                noexcept(container_base::copy_construct_container_noexcept);

        using container_base::operator view_type;
        using container_base::operator const_view_type;

        using container_base::operator=;
    };

    template <
            typename Key,
            typename Value,
            hash_map_flags Flags = hash_map_flags::default_flags,
            typename Hash = ::rsl::hash<Key>,
            typename KeyEqual = equal<Key>,
            ratio_type MaxLoadFactor = ratio<80, 100>,
            size_type FingerprintSize = internal::recommended_fingerprint_size<hash_map_flags_is_large(Flags)>>
    class dynamic_multi_map :
        public hash_map_base<map_info<Key, Value, Flags | hash_map_flags::multi, Hash, KeyEqual, MaxLoadFactor, FingerprintSize>>
    {
    public:
        using container_base =
                hash_map_base<map_info<Key, Value, Flags | hash_map_flags::multi, Hash, KeyEqual, MaxLoadFactor, FingerprintSize>>;

        static constexpr bool is_flat = container_base::is_flat;
        static constexpr bool is_large = container_base::is_large;
        static constexpr bool is_multi = container_base::is_multi;

        using key_type = typename container_base::key_type;
        using mapped_type = typename container_base::mapped_type;
        using hasher_type = typename container_base::hasher_type;
        using key_comparer_type = typename container_base::key_comparer_type;

        using iterator_type = typename container_base::iterator_type;
        using const_iterator_type = typename container_base::const_iterator_type;
        using reverse_iterator_type = typename container_base::reverse_iterator_type;
        using const_reverse_iterator_type = typename container_base::const_reverse_iterator_type;

        using view_type = typename container_base::view_type;
        using const_view_type = typename container_base::const_view_type;

        using hash_map_base<
                map_info<Key, Value, Flags | hash_map_flags::multi, Hash, KeyEqual, MaxLoadFactor, FingerprintSize>>::hash_map_base;

        [[rythe_always_inline]] constexpr dynamic_multi_map(const container_base& src)
                noexcept(container_base::copy_construct_container_noexcept);
        // TODO(Glyn): moving maps and sets
        [[rythe_always_inline]] constexpr dynamic_multi_map(container_base&& src)
                noexcept(container_base::copy_construct_container_noexcept);

        using container_base::operator view_type;
        using container_base::operator const_view_type;

        using container_base::operator=;
    };
} // namespace rsl

#include "dynamic_map.inl"
