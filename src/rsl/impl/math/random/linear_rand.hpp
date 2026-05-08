#pragma once
#include "../util/type_util.hpp"
#include "../vector/vector.hpp"

#include "randomnumbergenerator.hpp"

namespace rsl::math
{
    template <typename Scalar>
    [[nodiscard]] Scalar linear_rand(Scalar min, Scalar max) noexcept;

    template <vector_type vec_type, arithmetic_type Scalar>
            requires (remove_cvr_t<vec_type>::size != 1)
    [[nodiscard]] vec_type linear_rand(Scalar min, Scalar max) noexcept;

    template <vector_type vec_type0, vector_type vec_type1>
        requires (remove_cvr_t<vec_type0>::size == remove_cvr_t<vec_type1>::size)
    [[nodiscard]] auto linear_rand(const vec_type0& min, const vec_type1& max) noexcept;
} // namespace rsl::math

#include "linear_rand.inl"
