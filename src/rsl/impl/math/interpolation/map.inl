#include "map.hpp"
#pragma once

namespace rsl::math::internal
{
    template <typename T, typename InMin, typename InMax, typename OutMin, typename OutMax>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto
    _map_impl_(T&& value, InMin&& inMin, InMax&& inMax, OutMin&& outMin, OutMax&& outMax) noexcept
    {
        return outMin + (outMax - outMin) * (value - inMin) / (inMax - inMin);
    }
} // namespace rsl::math::internal

#include "map_vector.inl"
// #include "lerp_matrix.inl"
// #include "lerp_quaternion.inl"

namespace rsl::math
{
    // map
    template <typename T, typename InMin, typename InMax, typename OutMin, typename OutMax>
    [[nodiscard]] constexpr auto map(T&& value, InMin&& inMin, InMax&& inMax, OutMin&& outMin, OutMax&& outMax) noexcept
    {
        using ValueType = ::std::remove_cvref_t<T>;
        using InMinType = ::std::remove_cvref_t<InMin>;
        using InMaxType = ::std::remove_cvref_t<InMax>;
        using OutMinType = ::std::remove_cvref_t<OutMin>;
        using OutMaxType = ::std::remove_cvref_t<OutMax>;

        if constexpr (is_quat_v<ValueType> && is_quat_v<InMinType> && is_quat_v<InMaxType> && is_quat_v<OutMinType> &&
                      is_quat_v<OutMaxType>)
        {
            using scalar = elevated_t<
                typename ValueType::scalar,
                elevated_t<
                    typename InMinType::scalar,
                    elevated_t<
                        typename InMaxType::scalar,
                        elevated_t<typename OutMinType::scalar, typename OutMaxType::scalar>>>>;
            return internal::compute_map<quaternion<scalar>>::compute(
                forward<T>(value), forward<InMin>(inMin), forward<InMax>(inMax), forward<OutMin>(outMin),
                forward<OutMax>(outMax)
            );
        }
        else if constexpr (is_matrix_v<ValueType> && is_matrix_v<InMinType> && is_matrix_v<InMaxType> &&
                           is_matrix_v<OutMinType> && is_matrix_v<OutMaxType>)
        {
            using scalar = elevated_t<
                typename ValueType::scalar,
                elevated_t<
                    typename InMinType::scalar,
                    elevated_t<
                        typename InMaxType::scalar,
                        elevated_t<typename OutMinType::scalar, typename OutMaxType::scalar>>>>;
            return internal::compute_map<matrix<scalar, ValueType::row_count, ValueType::col_count>>::compute(
                forward<T>(value), forward<InMin>(inMin), forward<InMax>(inMax), forward<OutMin>(outMin),
                forward<OutMax>(outMax)
            );
        }
        else if constexpr (is_vector_v<ValueType> && is_vector_v<InMinType> && is_vector_v<InMaxType> &&
                           is_vector_v<OutMinType> && is_vector_v<OutMaxType>)
        {
            using scalar = elevated_t<
                typename ValueType::scalar,
                elevated_t<
                    typename InMinType::scalar,
                    elevated_t<
                        typename InMaxType::scalar,
                        elevated_t<typename OutMinType::scalar, typename OutMaxType::scalar>>>>;
            constexpr size_type size =
                min(ValueType::size,
                    min(InMinType::size, min(InMaxType::size, min(OutMinType::size, OutMaxType::size))));
            return internal::compute_map<vector<scalar, size>>::compute(
                forward<T>(value), forward<InMin>(inMin), forward<InMax>(inMax), forward<OutMin>(outMin),
                forward<OutMax>(outMax)
            );
        }
        else
        {
            return internal::_map_impl_(
                forward<T>(value), forward<InMin>(inMin), forward<InMax>(inMax), forward<OutMin>(outMin),
                forward<OutMax>(outMax)
            );
        }
    }

    // map01
    template <typename T, typename InMin, typename InMax>
    [[nodiscard]] constexpr auto map01(T&& value, InMin&& inMin, InMax&& inMax) noexcept
    {
        return map(forward<T>(value), forward<InMin>(inMin), forward<InMax>(inMax), zero<T>, one<T>);
    }
} // namespace rsl::math
