#pragma once

#include "../geometric/geometric.hpp"

namespace rsl::math
{
    template <arithmetic_type Scalar>
    [[nodiscard]] [[rythe_always_inline]] constexpr matrix<Scalar, 4, 4>
    perspective(radians<Scalar> fovY, Scalar aspect, Scalar nearZ, Scalar farZ) noexcept;

    template <arithmetic_type Scalar>
    [[nodiscard]] [[rythe_always_inline]] constexpr matrix<Scalar, 4, 4>
    orthographic(Scalar left, Scalar right, Scalar bottom, Scalar top, Scalar nearZ, Scalar farZ) noexcept;

    template <arithmetic_type Scalar, storage_mode ModeEye, storage_mode ModeCenter, storage_mode ModeUp>
    [[nodiscard]] [[rythe_always_inline]] constexpr matrix<
        Scalar, 4, 4, elevated_storage_mode_v<ModeEye, elevated_storage_mode_v<ModeCenter, ModeUp>>>
    look_at(vector<Scalar, 3, ModeEye> eye, vector<Scalar, 3, ModeCenter> center, vector<Scalar, 3, ModeUp> up)
        noexcept;

    template <arithmetic_type Scalar, size_type Size, storage_mode Mode>
    [[nodiscard]] [[rythe_always_inline]] constexpr matrix<Scalar, Size, Size, Mode>
    transpose(const matrix<Scalar, Size, Size, Mode>& mat) noexcept;

    template <arithmetic_type Scalar, storage_mode MatMode, storage_mode VecMode>
    [[nodiscard]] [[rythe_always_inline]] constexpr matrix<Scalar, 4, 4, elevated_storage_mode_v<MatMode, VecMode>>
    translate(const matrix<Scalar, 4, 4, MatMode>& mat, const vector<Scalar, 3, VecMode>& pos) noexcept;

    /*template<typename Scalar>
    [[nodiscard]] matrix<Scalar, 4, 4> rotate(matrix<Scalar, 4, 4> matrix, quaternion<Scalar> orientation) noexcept;*/

    template <arithmetic_type Scalar, storage_mode MatMode, storage_mode VecMode>
    [[nodiscard]] [[rythe_always_inline]] constexpr matrix<Scalar, 4, 4, elevated_storage_mode_v<MatMode, VecMode>>
    rotate(const matrix<Scalar, 4, 4, MatMode>& mat, radians<Scalar> angle, const vector<Scalar, 3, VecMode>& axis)
        noexcept;

    template <arithmetic_type Scalar, storage_mode MatMode, storage_mode VecMode>
    [[nodiscard]] [[rythe_always_inline]] constexpr matrix<Scalar, 4, 4, elevated_storage_mode_v<MatMode, VecMode>>
    scale(const matrix<Scalar, 4, 4, MatMode>& mat, const vector<Scalar, 3, VecMode>& scale) noexcept;
} // namespace rsl::math
