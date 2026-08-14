#pragma once
#include "../../rsl_core.hpp"
#include "../util/close_enough.hpp"
#include "../vector/vector.hpp"

namespace rsl::math::internal
{
    template <typename T>
    struct compute_equality;

    template <vector_type VecType>
    struct compute_equality<VecType>
    {
        using result_type = vector<bool, VecType::size, VecType::mode>;

        [[nodiscard]] [[rythe_always_inline]] constexpr static result_type
        compute(const VecType& a, const VecType& b) noexcept
        {
            result_type result;

            for (size_type i = 0; i < VecType::size; i++)
            {
                result[i] = close_enough(a[i], b[i]);
            }

            return result;
        }

        [[nodiscard]] [[rythe_always_inline]] constexpr static result_type
        compute(const VecType& a, typename VecType::scalar b) noexcept
        {
            result_type result;
            for (size_type i = 0; i < VecType::size; i++)
            {
                result[i] = close_enough(a[i], b);
            }
            return result;
        }
    };

    template <arithmetic_type Scalar, storage_mode Mode>
    struct compute_equality<vector<Scalar, 1, Mode>>
    {
        [[nodiscard]] [[rythe_always_inline]] constexpr static bool compute(Scalar a, Scalar b) noexcept
        {
            return close_enough(a, b);
        }
    };
} // namespace rsl::math::internal
