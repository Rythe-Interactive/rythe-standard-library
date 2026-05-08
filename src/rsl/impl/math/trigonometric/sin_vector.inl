#pragma once

namespace rsl::math::internal
{
    template <typename T>
    struct compute_sin;

    template <typename Scalar, size_type Size, storage_mode Mode>
    struct compute_sin<vector<Scalar, Size, Mode>>
    {
        using vec_type = vector<Scalar, Size, Mode>;

        [[nodiscard]] [[rythe_always_inline]] constexpr static vec_type compute(const vec_type& value) noexcept
        {
            vec_type result;
            for (size_type i = 0; i < Size; i++)
            {
                result[i] = sin(value[i]);
            }

            return result;
        }

        [[nodiscard]] [[rythe_always_inline]] constexpr static vec_type compute_inverse(const vec_type& value) noexcept
        {
            vec_type result;
            for (size_type i = 0; i < Size; i++)
            {
                result[i] = asin(value[i]);
            }

            return result;
        }
    };

    template <typename Scalar, storage_mode Mode>
    struct compute_sin<vector<Scalar, 1u, Mode>>
    {
        using vec_type = vector<Scalar, 1u, Mode>;

        [[nodiscard]] [[rythe_always_inline]] constexpr static Scalar compute(Scalar value) noexcept
        {
            return sin(value);
        }

        [[nodiscard]] [[rythe_always_inline]] constexpr static Scalar compute_inverse(Scalar value) noexcept
        {
            return asin(value);
        }
    };
} // namespace rsl::math::internal
