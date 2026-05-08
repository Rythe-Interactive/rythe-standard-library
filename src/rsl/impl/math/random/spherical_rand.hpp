#pragma once
#include "../constants.hpp"
#include "linear_rand.hpp"

namespace rsl::math
{
    template <arithmetic_type Scalar, storage_mode Mode>
    [[nodiscard]] vector<Scalar, 3, Mode> spherical_rand(Scalar radius) noexcept;

    template <vector_type vec_type>
        requires(vec_type::size == 3)
    [[nodiscard]] vec_type spherical_rand(const vec_type& radii) noexcept;
} // namespace rsl::math

#include "spherical_rand.inl"
