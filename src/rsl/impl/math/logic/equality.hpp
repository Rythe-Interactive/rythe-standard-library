#pragma once
#include "../util/close_enough.hpp"
#include "../util/type_util.hpp"

namespace rsl::math
{
    namespace internal
    {
        template <typename A, typename B>
        concept equal_comparable_vector =
            (vector_type<A> && vector_type<B>) ||
            (vector_type<A> && same_as<typename remove_cvr_t<A>::scalar, remove_cvr_t<B>>) ||
            (vector_type<B> && same_as<typename remove_cvr_t<B>::scalar, remove_cvr_t<A>>);

        template <typename A, typename B>
        concept equal_comparable_matrix =
            (matrix_type<A> && matrix_type<B> && remove_cvr_t<A>::row_count == remove_cvr_t<B>::row_count &&
             remove_cvr_t<A>::col_count == remove_cvr_t<B>::col_count) ||
            (matrix_type<A> && vector_type<B> && remove_cvr_t<A>::row_count == remove_cvr_t<B>::size) ||
            (matrix_type<A> && same_as<typename remove_cvr_t<A>::scalar, remove_cvr_t<B>>) ||
            (matrix_type<B> && same_as<typename remove_cvr_t<B>::scalar, remove_cvr_t<A>>);

        template <typename A, typename B>
        concept equal_comparable_quaternion = quat_type<A> && quat_type<B>;

        template <typename A, typename B>
        concept equal_comparable =
            equal_comparable_vector<A, B> || equal_comparable_matrix<A, B> || equal_comparable_quaternion<A, B>;
    } // namespace internal

    template <typename A, typename B>
        requires internal::equal_comparable<A, B>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto equal(A&& a, B&& b) noexcept;

    template <typename A, typename B>
        requires internal::equal_comparable<A, B>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto operator==(A&& a, B&& b) noexcept;

    template <typename A, typename B>
        requires internal::equal_comparable<A, B>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto operator!=(A&& a, B&& b) noexcept;
} // namespace rsl::math

#include "equality.inl"
