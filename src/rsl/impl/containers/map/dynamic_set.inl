#include "dynamic_set.hpp"
namespace rsl
{
    template <
            typename Key,
            hash_map_flags Flags,
            typename Hash,
            typename KeyEqual,
            ratio_type MaxLoadFactor,
            size_type FingerprintSize>
    inline constexpr dynamic_set<Key, Flags, Hash, KeyEqual, MaxLoadFactor, FingerprintSize>::dynamic_set(const container_base& src)
            noexcept(container_base::copy_construct_container_noexcept)
        : container_base(src)
    {}
    template <
            typename Key,
            hash_map_flags Flags,
            typename Hash,
            typename KeyEqual,
            ratio_type MaxLoadFactor,
            size_type FingerprintSize>
    inline constexpr rsl::dynamic_set<Key, Flags, Hash, KeyEqual, MaxLoadFactor, FingerprintSize>::dynamic_set(container_base&& src)
            noexcept(container_base::copy_construct_container_noexcept)
        : container_base(rsl::move(src))
    {}
}
