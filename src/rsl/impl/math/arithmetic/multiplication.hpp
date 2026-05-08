#pragma once
#include "../basic/constraint.hpp"
#include "../util/type_util.hpp"

namespace rsl::math
{
    template <typename TypeA, typename TypeB>
        requires(
            (linear_algebraic_construct<TypeA> || arithmetic_type<TypeA>) &&
            (linear_algebraic_construct<TypeB> || arithmetic_type<TypeB>)
        )
    [[nodiscard]] [[rythe_always_inline]] constexpr auto mul(const TypeA& a, const TypeB& b) noexcept;

    template <linear_algebraic_construct TypeA, linear_algebraic_construct TypeB>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto operator*(const TypeA& a, const TypeB& b) noexcept;

    template <typename TypeA, typename TypeB>
        requires(
            (linear_algebraic_construct<TypeA> && arithmetic_type<TypeB>) ||
            (linear_algebraic_construct<TypeB> && arithmetic_type<TypeA>)
        )
    [[nodiscard]] [[rythe_always_inline]] constexpr auto operator*(const TypeA& a, const TypeB& b) noexcept;

    template <linear_algebraic_construct TypeA, linear_algebraic_construct TypeB>
    [[rythe_always_inline]] constexpr TypeA& mul_assign(TypeA& a, const TypeB& b) noexcept;

    template <typename TypeA, typename TypeB>
        requires(
            (linear_algebraic_construct<TypeA> && arithmetic_type<TypeB>) ||
            (linear_algebraic_construct<TypeB> && arithmetic_type<TypeA>)
        )
    [[rythe_always_inline]] constexpr TypeA& mul_assign(TypeA& a, const TypeB& b) noexcept;

    template <linear_algebraic_construct TypeA, linear_algebraic_construct TypeB>
    [[rythe_always_inline]] constexpr TypeA& operator*=(TypeA& a, const TypeB& b) noexcept;

    template <typename TypeA, typename TypeB>
        requires(
            (linear_algebraic_construct<TypeA> && arithmetic_type<TypeB>) ||
            (linear_algebraic_construct<TypeB> && arithmetic_type<TypeA>)
        )
    [[rythe_always_inline]] constexpr TypeA& operator*=(TypeA& a, const TypeB& b) noexcept;
} // namespace rsl::math

#include "multiplication.inl"
