#pragma once
#include "multiplication.hpp"

namespace rsl::math::internal
{
    template <typename T>
    struct compute_multiplication;

    template <arithmetic_type Scalar, size_type Size, storage_mode Mode>
    struct compute_multiplication<vector<Scalar, Size, Mode>>
    {
        using vec_type = vector<Scalar, Size, Mode>;

        template <typename AType, typename BType>
        [[nodiscard]] constexpr static auto compute(AType&& a, BType&& b) noexcept
        {
            vec_type result;
            for (size_type i = 0; i < Size; i++)
            {
                if constexpr (is_vector_v<AType> && make_vector_t<AType>::size >= Size)
                {
                    if constexpr (is_vector_v<BType> && make_vector_t<BType>::size >= Size)
                    {
                        result[i] = a[i] * b[i];
                    }
                    else
                    {
                        result[i] = a[i] * b;
                    }
                }
                else
                {
                    static_assert(is_vector_v<BType> && make_vector_t<BType>::size >= Size);
                    result[i] = a * b[i];
                }
            }

            return result;
        }
    };

    template <typename Scalar, storage_mode Mode>
    struct compute_multiplication<vector<Scalar, 1u, Mode>>
    {
        using vec_type = vector<Scalar, 1u, Mode>;

        template <typename AType, typename BType>
        [[nodiscard]] constexpr static Scalar compute(AType&& a, BType&& b) noexcept
        {
            if constexpr (is_vector_v<AType>)
            {
                if constexpr (is_vector_v<BType>)
                {
                    return a[0] * b[0];
                }
                else
                {
                    return a[0] * b;
                }
            }
            else if constexpr (is_vector_v<BType>)
            {
                return a * b[0];
            }
            else
            {
                return a * b;
            }
        }
    };
} // namespace rsl::math::internal
