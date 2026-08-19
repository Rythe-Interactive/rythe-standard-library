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
} // namespace rsl
