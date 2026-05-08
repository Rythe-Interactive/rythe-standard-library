#pragma once
#include "../interpolation/map.hpp"

namespace rsl::math::internal
{
    template <typename T>
    struct compute_cos;

    template <arithmetic_type Scalar, size_type Size, storage_mode Mode>
    struct compute_cos<vector<Scalar, Size, Mode>>
    {
        using vec_type = vector<Scalar, Size, Mode>;

        [[nodiscard]] [[rythe_always_inline]] static constexpr auto compute(const vec_type& value) noexcept
        {
            vec_type result;
            for (size_type i = 0; i < Size; i++)
            {
                result[i] = cos(value[i]);
            }

            return result;
        }

        template <typename ValueType>
        [[nodiscard]] [[rythe_always_inline]] static constexpr auto compute_inverse(const vec_type& value) noexcept
        {
            vec_type result;
            for (size_type i = 0; i < Size; i++)
            {
                result[i] = acos(value[i]);
            }

            return result;
        }
    };

    template <arithmetic_type Scalar, storage_mode Mode>
    struct compute_cos<vector<Scalar, 1ull, Mode>>
    {
        using vec_type = vector<Scalar, 1ull, Mode>;

        [[nodiscard]] [[rythe_always_inline]] static constexpr auto compute(Scalar value) noexcept
        {
            return cos(value[0]);
        }

        [[nodiscard]] [[rythe_always_inline]] static constexpr auto compute_inverse(Scalar value) noexcept
        {
            return acos(value[0]);
        }
    };
} // namespace rsl::math::internal
