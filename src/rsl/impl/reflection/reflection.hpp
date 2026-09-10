#pragma once
#include "../memory/pointer.hpp"

#include "reflection_registry.hpp"

namespace rsl::rfl
{
    class reflection
    {
    public:
        [[rythe_always_inline]] constexpr void add_registry(pointer<reflection_registry> registry);
        [[nodiscard]] [[rythe_always_inline]] constexpr array_view<pointer<reflection_registry>> registries() noexcept;

    private:
        dynamic_array<pointer<reflection_registry>> m_registries;
    };

    RYTHE_DECLARE_SINGLETON(reflection)
}

namespace rrfl = rsl::rfl;

#include "reflection.inl"
