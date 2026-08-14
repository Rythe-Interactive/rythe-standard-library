#pragma once
#include "../../rsl_core.hpp"
#include "../util/type_util.hpp"
#include "../vector/vector.hpp"
#include "common.hpp"

namespace rsl::math::noise
{
    // value noise
    template <typename VectorType>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto
    value(VectorType&& value, int_least<sizeof(vector_scalar_t<VectorType>)> seed = 0) noexcept;

    // value noise
    template <typename VectorType>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto value(
        VectorType&& value, decay_vector_t<VectorType>& derivative,
        int_least<sizeof(vector_scalar_t<VectorType>)> seed = 0
    ) noexcept;
} // namespace rsl::math::noise

#include "value.inl"
