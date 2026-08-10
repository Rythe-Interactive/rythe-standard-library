#pragma once

#include "hash_map.hpp"
#include "map_info.hpp"

namespace rsl
{
    template <
        typename Key, typename Value, hash_map_flags Flags = hash_map_flags::default_flags,
        typename Hash = ::rsl::hash<Key>, typename KeyEqual = equal<Key>,
        ratio_type MaxLoadFactor = ratio<80, 100>,
        size_type FingerprintSize = internal::recommended_fingerprint_size<hash_map_flags_is_large(Flags)>>
    class dynamic_map :
        public hash_map_base<
            map_info<Key, Value, Flags, Hash, KeyEqual, MaxLoadFactor, FingerprintSize>>
    {
    public:
        using hash_map_base<
            map_info<Key, Value, Flags, Hash, KeyEqual, MaxLoadFactor, FingerprintSize>>::hash_map_base;
    };

    template <
            typename Key,
            typename Value,
            hash_map_flags Flags = hash_map_flags::default_flags,
            typename Hash = ::rsl::hash<Key>,
            typename KeyEqual = equal<Key>,
            ratio_type MaxLoadFactor = ratio<80, 100>,
            size_type FingerprintSize = internal::recommended_fingerprint_size<hash_map_flags_is_large(Flags)>>
    class dynamic_multi_map : public hash_map_base<map_info<Key, Value, Flags | hash_map_flags::multi, Hash, KeyEqual, MaxLoadFactor, FingerprintSize>>
    {
    public:
        using hash_map_base<map_info<Key, Value, Flags | hash_map_flags::multi, Hash, KeyEqual, MaxLoadFactor, FingerprintSize>>::hash_map_base;
    };
} // namespace rsl
