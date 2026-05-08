#pragma once
#include "../geometric/cross.hpp"
#include "../geometric/dot.hpp"
#include "addition.hpp"

namespace rsl::math::internal
{
    template <typename T>
    struct compute_multiplication;

    template <arithmetic_type Scalar, storage_mode Mode>
    struct compute_multiplication<quaternion<Scalar, Mode>>
    {
        using value_type = quaternion<Scalar, Mode>;
        using vec_type = typename value_type::vec_type;

        [[nodiscard]] constexpr static auto compute(const value_type& a, const value_type& b) noexcept
        {
            vec_type aVec(a.i, a.j, a.k);
            vec_type bVec(b.i, b.j, b.k);

            vec_type resultVec = a.w * bVec + static_cast<Scalar>(b.w) * aVec + cross(aVec, bVec);
            return value_type(a.w * static_cast<Scalar>(b.w) - dot(aVec, bVec), resultVec);
        }

        template <vector_type VecType>
        [[nodiscard]] constexpr static auto compute(const VecType& a, const value_type& b) noexcept
        {
            vec_type bVec(b.i, b.j, b.k);
            if constexpr (VecType::size == 3)
            {
                return static_cast<Scalar>(2) * dot(bVec, a) * bVec +
                       (static_cast<Scalar>(b.w) * static_cast<Scalar>(b.w) - dot(bVec, bVec)) * a +
                       static_cast<Scalar>(2) * static_cast<Scalar>(b.w) * cross(bVec, a);
            }
            else if constexpr (VecType::size == 4)
            {
                return VecType(
                    static_cast<Scalar>(2) * dot(bVec, a.xyz) * bVec +
                    (static_cast<Scalar>(b.w) * static_cast<Scalar>(b.w) - dot(bVec, bVec)) * a.xyz +
                    static_cast<Scalar>(2) * static_cast<Scalar>(b.w) * cross(bVec, a.xyz)
                );
            }
            else
            {
                vector<Scalar, 3, Mode> a3 = a;
                return vector<Scalar, 3, Mode>(
                    static_cast<Scalar>(2) * dot(bVec, a3) * bVec +
                    (static_cast<Scalar>(b.w) * static_cast<Scalar>(b.w) - dot(bVec, bVec)) * a3 +
                    static_cast<Scalar>(2) * static_cast<Scalar>(b.w) * cross(bVec, a3)
                );
            }
        }

        template <arithmetic_type ScalarType>
        [[nodiscard]] constexpr static auto compute(const value_type& a, ScalarType b) noexcept
        {
            value_type result;
            result.i = a.i * b;
            result.j = a.j * b;
            result.k = a.k * b;
            result.w = a.w * b;
            return result;
        }
    };
} // namespace rsl::math::internal
