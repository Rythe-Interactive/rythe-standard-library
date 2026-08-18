#include "hash.hpp"

#include <meow_hash_x64_aesni.h>

namespace rsl
{
    content_hash hash_content(byte_view bytes) noexcept
    {
        static_assert(sizeof(meow_u128) == sizeof(content_hash::hash));
        static_assert(alignof(meow_u128) == alignof(content_hash::hash));
        union
        {
            meow_u128 meow;
            content_hash::hash rythe;
        } converter;

        converter.meow = MeowHash(MeowDefaultSeed, bytes.size(), bytes.data());
        return content_hash{
            .size = bytes.size(),
            .value = converter.rythe,
        };
    }
} // namespace rsl
