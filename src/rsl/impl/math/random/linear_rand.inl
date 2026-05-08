#pragma once
#include "linear_rand.hpp"

namespace rsl::math
{
    template <typename Scalar>
    [[nodiscard]] Scalar linear_rand(Scalar min, Scalar max) noexcept
    {
        if constexpr (is_floating_point_v<Scalar>)
        {
            return min + (max - min) * random_number_generator::generate_random_float<Scalar>();
        }
        else
        {
            static_assert(
                is_arithmetic_v<Scalar>,
                "Input scalar type to linear_rand is neither a scalar nor any other supported type."
            );
            return min + random_number_generator::generate_random_integer<Scalar>() % (max - min);
        }
    }

    namespace internal
    {
        template <typename T>
        struct compute_linear_rand;

        template <typename Scalar, size_type Size>
        struct compute_linear_rand<vector<Scalar, Size>>
        {
            using vec_type = vector<Scalar, Size>;

            [[nodiscard]] static auto compute(const vec_type& min, const vec_type& max) noexcept
            {
                vec_type result;
                for (size_type i = 0; i < Size; i++)
                {
                    result[i] = linear_rand(min[i], max[i]);
                }
                return result;
            }


            [[nodiscard]] static auto compute(Scalar min, Scalar max) noexcept
            {
                vec_type result;
                for (size_type i = 0; i < Size; i++)
                {
                    result[i] = linear_rand(min, max);
                }
                return result;
            }
        };

        template <typename Scalar>
        struct compute_linear_rand<vector<Scalar, 1>>
        {
            [[nodiscard]] static auto compute(Scalar min, Scalar max) noexcept { return linear_rand(min, max); }
        };
    } // namespace internal

    template <vector_type vec_type, arithmetic_type Scalar>
            requires (remove_cvr_t<vec_type>::size != 1)
    [[nodiscard]] vec_type linear_rand(Scalar min, Scalar max) noexcept
    {
        return internal::compute_linear_rand<vec_type>::compute(
            static_cast<typename vec_type::scalar>(min), static_cast<typename vec_type::scalar>(max)
        );
    }

    template <vector_type vec_type0, vector_type vec_type1>
        requires (remove_cvr_t<vec_type0>::size == remove_cvr_t<vec_type1>::size)
    [[nodiscard]] auto linear_rand(const vec_type0& min, const vec_type1& max) noexcept
    {
        using scalar = lowest_precision_t<typename vec_type0::scalar, typename vec_type1::scalar>;
        constexpr size_type size = math::min(vec_type0::size, vec_type1::size);
        return internal::compute_linear_rand<vector<scalar, size>>::compute(min, max);
    }
} // namespace rsl::math
