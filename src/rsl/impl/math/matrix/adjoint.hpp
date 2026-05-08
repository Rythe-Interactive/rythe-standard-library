#pragma once

#include "determinant.hpp"

namespace rsl::math
{
    template <typename MatType, ::std::enable_if_t<is_matrix_v<MatType>, bool> = true>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto adjoint(const MatType& mat) noexcept;
}

#include "adjoint.inl"
