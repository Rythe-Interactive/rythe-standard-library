#pragma once

namespace rsl::math::internal
{
    template <typename T>
    struct compute_tan;

    template <typename Scalar, size_type Size, storage_mode Mode>
    struct compute_tan<vector<Scalar, Size, Mode>>
    {
        using vec_type = vector<Scalar, Size, Mode>;

        [[nodiscard]] [[rythe_always_inline]] constexpr static vec_type compute(const vec_type& value) noexcept
        {
            vec_type result;
            for (size_type i = 0; i < Size; i++)
            {
                result[i] = tan(value[i]);
            }

            return result;
        }

        [[nodiscard]] [[rythe_always_inline]] constexpr static vec_type compute_inverse(const vec_type& value) noexcept
        {
            vec_type result;
            for (size_type i = 0; i < Size; i++)
            {
                result[i] = atan(value[i]);
            }

            return result;
        }

        [[nodiscard]] [[rythe_always_inline]] constexpr static vec_type
        compute_inverse2(const vec_type& x, const vec_type& y) noexcept
        {
            vec_type result;
            for (size_type i = 0; i < Size; i++)
            {
                result[i] = atan2(y[i], x[i]);
            }

            return result;
        }
    };

    template <typename Scalar, storage_mode Mode>
    struct compute_tan<vector<Scalar, 1u, Mode>>
    {
        using vec_type = vector<Scalar, 1u, Mode>;

        [[nodiscard]] [[rythe_always_inline]] constexpr static Scalar compute(Scalar value) noexcept
        {
            return tan(value[0]);
        }

        [[nodiscard]] [[rythe_always_inline]] constexpr static Scalar compute_inverse(Scalar value) noexcept
        {
            return atan(value[0]);
        }

        [[nodiscard]] [[rythe_always_inline]] constexpr static Scalar compute_inverse2(Scalar x, Scalar y) noexcept
        {
            return atan2(x[0], y[0]);
        }
    };
} // namespace rsl::math::internal
