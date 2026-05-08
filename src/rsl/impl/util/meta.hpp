#pragma once

#include "../defines.hpp"

namespace rsl
{
    RYTHE_HAS_FUNC(setup);
    RYTHE_HAS_FUNC(shutdown);
    RYTHE_HAS_FUNC(update);

    RYTHE_HAS_FUNC(begin);
    RYTHE_HAS_FUNC(end);
    RYTHE_HAS_FUNC(at);

    RYTHE_HAS_FUNC(size);
    RYTHE_HAS_FUNC(resize);

    RYTHE_HAS_FUNC(push_back);
    RYTHE_HAS_FUNC(emplace);
    RYTHE_HAS_FUNC(insert);
} // namespace rsl
