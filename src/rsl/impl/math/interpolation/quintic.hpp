#pragma once
#include "../../rsl_core.hpp"
#include "../basic/constraint.hpp"
#include "../exponential/sqrt.hpp"
#include "../matrix/matrix.hpp"
#include "../quaternion/quaternion_base.hpp"
#include "../trigonometric/trigonometric.hpp"
#include "../util/type_util.hpp"
#include "../vector/vector.hpp"
#include "map.hpp"

namespace rsl::math
{
    // quintic
    template <typename TypeMin, typename TypeMax, typename ValueType>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto
    quintic(TypeMin&& edgeMin, TypeMax&& edgeMax, ValueType&& value) noexcept;

    // quintic
    template <typename ValueType>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto quintic(ValueType&& value) noexcept;

    // quintic_derivative
    template <typename TypeMin, typename TypeMax, typename ValueType>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto
    quintic_derivative(TypeMin&& edgeMin, TypeMax&& edgeMax, ValueType&& value) noexcept;

    // quintic_derivative
    template <typename ValueType>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto quintic_derivative(ValueType&& value) noexcept;

    // inverse_quintic
    template <typename TypeMin, typename TypeMax, typename ValueType>
    [[nodiscard]] [[rythe_always_inline]] auto
    inverse_quintic(TypeMin&& edgeMin, TypeMax&& edgeMax, ValueType&& value) noexcept;

    // inverse_quintic
    template <typename ValueType>
    [[nodiscard]] [[rythe_always_inline]] auto inverse_quintic(ValueType&& value) noexcept;

    // inverse_quintic_derivative
    template <typename TypeMin, typename TypeMax, typename ValueType>
    [[nodiscard]] [[rythe_always_inline]] auto
    inverse_quintic_derivative(TypeMin&& edgeMin, TypeMax&& edgeMax, ValueType&& value) noexcept;

    // inverse_quintic_derivative
    template <typename ValueType>
    [[nodiscard]] [[rythe_always_inline]] auto inverse_quintic_derivative(ValueType&& value) noexcept;
} // namespace rsl::math

#include "quintic.inl"
