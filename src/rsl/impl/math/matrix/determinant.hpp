#pragma once

#include "../util/type_util.hpp"

namespace rsl::math
{
    template <matrix_type MatType>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto determinant(const MatType& mat) noexcept;
}

#include "determinant.inl"
