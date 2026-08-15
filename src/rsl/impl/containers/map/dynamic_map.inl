#include "dynamic_map.hpp"
namespace rsl
{
    template <
            typename Key,
            typename Value,
            hash_map_flags Flags,
            typename Hash,
            typename KeyEqual,
            ratio_type MaxLoadFactor,
            size_type FingerprintSize>
    inline constexpr dynamic_map<Key, Value, Flags, Hash, KeyEqual, MaxLoadFactor, FingerprintSize>::dynamic_map(
            const container_base& src) noexcept(container_base::copy_construct_container_noexcept)
        : container_base(src)
    {}

    template <
            typename Key,
            typename Value,
            hash_map_flags Flags,
            typename Hash,
            typename KeyEqual,
            ratio_type MaxLoadFactor,
            size_type FingerprintSize>
    inline constexpr dynamic_map<Key, Value, Flags, Hash, KeyEqual, MaxLoadFactor, FingerprintSize>::dynamic_map(container_base&& src)
            noexcept(container_base::copy_construct_container_noexcept)
        : container_base(rsl::move(src))
    {}

    template <
            typename Key,
            typename Value,
            hash_map_flags Flags,
            typename Hash,
            typename KeyEqual,
            ratio_type MaxLoadFactor,
            size_type FingerprintSize>
    inline constexpr rsl::dynamic_multi_map<Key, Value, Flags, Hash, KeyEqual, MaxLoadFactor, FingerprintSize>::dynamic_multi_map(
            const container_base& src) noexcept(container_base::copy_construct_container_noexcept)
        : container_base(src)
    {}
    template <
            typename Key,
            typename Value,
            hash_map_flags Flags,
            typename Hash,
            typename KeyEqual,
            ratio_type MaxLoadFactor,
            size_type FingerprintSize>
    inline constexpr dynamic_multi_map<Key, Value, Flags, Hash, KeyEqual, MaxLoadFactor, FingerprintSize>::dynamic_multi_map(
            container_base&& src) noexcept(container_base::copy_construct_container_noexcept)
        : container_base(rsl::move(src))
    {}
}
