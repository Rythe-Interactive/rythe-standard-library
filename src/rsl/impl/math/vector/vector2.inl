#pragma once
#include "vector2.hpp"

#define SIZE_SPECIALIZATION 2
#define SCALAR_SPECIALIZATION Scalar
#define VECTOR_TEMPLATE template <arithmetic_type Scalar, storage_mode Mode>

#include "vector_shared.inl"

#define SIZE_SPECIALIZATION 2
#define SCALAR_SPECIALIZATION bool
#define VECTOR_TEMPLATE template <storage_mode Mode>

#include "vector_shared.inl"

namespace rsl::math
{
    template <arithmetic_type Scalar, storage_mode Mode>
    constexpr vector<Scalar, 2, Mode>::vector(scalar _x, scalar _y) noexcept
        : data{
              {_x, _y}
    }
    {
    }

    template <storage_mode Mode>
    constexpr vector<bool, 2, Mode>::vector(scalar _x, scalar _y) noexcept
        : data{
              {_x, _y}
    }
    {
    }
} // namespace rsl::math
