#include "hash.hpp"

#include <meow_hash_x64_aesni.h>

namespace rsl
{
    content_hash hash_content(byte_view bytes) noexcept
    {
        meow_u128 meow = MeowHash(MeowDefaultSeed, bytes.size(), bytes.data());
        return content_hash{
            .size = bytes.size(),
            .value = unaligned_load<content_hash::hash>(&meow),
        };
    }

    static_assert(alignof(hash_state::internal_state) == alignof(meow_state));
    static_assert(sizeof(hash_state::internal_state) >= sizeof(meow_state));

    void begin_content_hash(hash_state& hashState) noexcept
    {
        hashState.size = 0ull;
        MeowBegin(reinterpret_cast<meow_state*>(&hashState.internalState), MeowDefaultSeed);
    }

    void append_content_hash(hash_state& hashState, byte_view bytes) noexcept
    {
        hashState.size += bytes.size();
        MeowAbsorb(reinterpret_cast<meow_state*>(&hashState.internalState), bytes.size(), bytes.data());
    }

    content_hash end_content_hash(hash_state& hashState) noexcept
    {
        meow_u128 meow = MeowEnd(reinterpret_cast<meow_state*>(&hashState.internalState), nullptr);
        return content_hash{
            .size = hashState.size,
            .value = unaligned_load<content_hash::hash>(&meow),
        };
    }
} // namespace rsl
