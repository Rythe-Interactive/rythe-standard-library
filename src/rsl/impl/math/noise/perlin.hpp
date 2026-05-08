#pragma once
#include "../../util/primitives.hpp"
#include "../basic/basic_funcs.hpp"
#include "../interpolation/interpolation.hpp"
#include "../util/type_util.hpp"
#include "../vector/vector.hpp"

namespace rsl::math
{
    // perlin
    template <typename VectorType>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto
    perlin(VectorType&& value, int_least<sizeof(vector_scalar_t<VectorType>)> seed = 0) noexcept;

    // perlin
    template <typename VectorType>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto perlin(
        VectorType&& value, decay_vector_t<VectorType>& derivative,
        int_least<sizeof(vector_scalar_t<VectorType>)> seed = 0
    ) noexcept;
} // namespace rsl::math

#include "perlin.inl"
