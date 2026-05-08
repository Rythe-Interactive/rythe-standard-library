#pragma once
#include "spherical_rand.hpp"

namespace rsl::math
{
    template <arithmetic_type Scalar, storage_mode Mode>
    vector<Scalar, 3, Mode> spherical_rand(Scalar radius) noexcept
    {
        Scalar theta = linear_rand(static_cast<Scalar>(0), tau<Scalar>());
        Scalar z = linear_rand(static_cast<Scalar>(-1), static_cast<Scalar>(1));
        Scalar phi = acos(z);
        Scalar x = sin(phi) * cos(theta);
        Scalar y = sin(phi) * sin(theta);

        return vector<Scalar, 3, Mode>(x, y, z) * radius;
    }

    template <vector_type vec_type>
        requires(vec_type::size == 3)
    vec_type spherical_rand(const vec_type& radii) noexcept
    {
        using Scalar = typename vec_type::scalar;

        Scalar theta = linear_rand(static_cast<Scalar>(0), tau<Scalar>());
        Scalar z = linear_rand(static_cast<Scalar>(-1), static_cast<Scalar>(1));
        Scalar phi = acos(z);
        Scalar x = sin(phi) * cos(theta);
        Scalar y = sin(phi) * sin(theta);

        return vec_type(x, y, z) * radii;
    }
} // namespace rsl::math
