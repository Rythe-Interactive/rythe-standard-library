#pragma once
#include "multiplication.hpp"

#include "multiplication_matrix.inl"
#include "multiplication_quaternion.inl"
#include "multiplication_vector.inl"

namespace rsl::math
{
    // mul
    template <typename TypeA, typename TypeB>
        requires(
            (linear_algebraic_construct<TypeA> || arithmetic_type<TypeA>) &&
            (linear_algebraic_construct<TypeB> || arithmetic_type<TypeB>)
        )
    [[nodiscard]] constexpr auto mul(const TypeA& a, const TypeB& b) noexcept
    {
        if constexpr (is_quat_v<TypeA> && is_quat_v<TypeB>)
        {
            return internal::compute_multiplication<elevated_t<TypeA, TypeB>>::compute(a, b);
        }
        else if constexpr (is_vector_v<TypeA> && is_quat_v<TypeB>)
        {
            using scalar = elevated_t<typename TypeA::scalar, typename TypeB::scalar>;
            constexpr storage_mode mode = elevated_storage_mode_v<TypeA::mode, TypeB::mode>;
            return internal::compute_multiplication<quaternion<scalar, mode>>::compute(a, b);
        }
        else if constexpr (is_quat_v<TypeA> && is_vector_v<TypeB>)
        {
            using scalar = elevated_t<typename TypeA::scalar, typename TypeB::scalar>;
            constexpr storage_mode mode = elevated_storage_mode_v<TypeA::mode, TypeB::mode>;
            return internal::compute_multiplication<quaternion<scalar, mode>>::compute(b, a);
        }
        else if constexpr (is_matrix_v<TypeA> && is_matrix_v<TypeB>)
        {
            return internal::compute_multiplication<elevated_t<TypeA, TypeB>>::compute(a, b);
        }
        else if constexpr (is_matrix_v<TypeA> && is_vector_v<TypeB>)
        {
            using scalar = elevated_t<typename TypeA::scalar, typename TypeB::scalar>;
            constexpr storage_mode mode = elevated_storage_mode_v<TypeA::mode, TypeB::mode>;
            return internal::compute_multiplication<matrix<scalar, TypeA::row_count, TypeA::col_count, mode>>::compute(
                b, a
            );
        }
        else if constexpr (is_vector_v<TypeA> && is_matrix_v<TypeB>)
        {
            using scalar = elevated_t<typename TypeA::scalar, typename TypeB::scalar>;
            constexpr storage_mode mode = elevated_storage_mode_v<TypeA::mode, TypeB::mode>;
            return internal::compute_multiplication<matrix<scalar, TypeB::row_count, TypeB::col_count, mode>>::compute(
                a, b
            );
        }
        else if constexpr (is_vector_v<TypeA> && is_vector_v<TypeB>)
        {
            return internal::compute_multiplication<elevated_t<TypeA, TypeB>>::compute(a, b);
        }
        else if constexpr (is_vector_v<TypeA> || is_vector_v<TypeB>)
        {
            using vec_type = select_vector_type_t<TypeA, TypeB>;
            using scalar = elevated_t<typename vec_type::scalar, select_floating_type_t<TypeA, TypeB>>;
            return internal::compute_multiplication<vector<scalar, vec_type::size, vec_type::mode>>::compute(a, b);
        }
        else if constexpr (is_quat_v<TypeA> && is_arithmetic_v<TypeB>)
        {
            return internal::compute_multiplication<
                quaternion<elevated_t<typename TypeA::scalar, TypeB>, TypeA::mode>>::compute(a, b);
        }
        else if constexpr (is_arithmetic_v<TypeA> && is_quat_v<TypeB>)
        {
            return internal::compute_multiplication<
                quaternion<elevated_t<TypeA, typename TypeB::scalar>, TypeB::mode>>::compute(b, a);
        }
        else
        {
            return a * b;
        }
    }

    template <linear_algebraic_construct TypeA, linear_algebraic_construct TypeB>
    [[nodiscard]] constexpr auto operator*(const TypeA& a, const TypeB& b) noexcept
    {
        return mul(a, b);
    }

    template <typename TypeA, typename TypeB>
        requires(
            (linear_algebraic_construct<TypeA> && arithmetic_type<TypeB>) ||
            (linear_algebraic_construct<TypeB> && arithmetic_type<TypeA>)
        )
    constexpr auto operator*(const TypeA& a, const TypeB& b) noexcept
    {
        return mul(a, b);
    }

    template <linear_algebraic_construct TypeA, linear_algebraic_construct TypeB>
    constexpr TypeA& mul_assign(TypeA& a, const TypeB& b) noexcept
    {
        return a = a * b;
    }

    template <typename TypeA, typename TypeB>
        requires(
            (linear_algebraic_construct<TypeA> && arithmetic_type<TypeB>) ||
            (linear_algebraic_construct<TypeB> && arithmetic_type<TypeA>)
        )
    constexpr TypeA& mul_assign(TypeA& a, const TypeB& b) noexcept
    {
        return a = a * b;
    }

    template <linear_algebraic_construct TypeA, linear_algebraic_construct TypeB>
    constexpr TypeA& operator*=(TypeA& a, const TypeB& b) noexcept
    {
        return a = a * b;
    }

    template <typename TypeA, typename TypeB>
        requires(
            (linear_algebraic_construct<TypeA> && arithmetic_type<TypeB>) ||
            (linear_algebraic_construct<TypeB> && arithmetic_type<TypeA>)
        )
    constexpr TypeA& operator*=(TypeA& a, const TypeB& b) noexcept
    {
        return a = a * b;
    }

} // namespace rsl::math
