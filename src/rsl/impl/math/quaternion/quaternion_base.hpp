#pragma once
#include "../../util/primitives.hpp"

#include "../trigonometric/angle.hpp"
#include "../util/type_util.hpp"
#include "../vector/vector.hpp"

RYTHE_MSVC_SUPPRESS_WARNING_WITH_PUSH(4201)

namespace rsl::math
{
    template <arithmetic_type Scalar, size_type RowCount, size_type ColCount, storage_mode Mode>
    struct matrix;

    template <arithmetic_type Scalar, storage_mode Mode = storage_mode::defaultStorage>
    struct quaternion
    {
        using scalar = Scalar;
        static constexpr size_type size = 4;
        static constexpr storage_mode mode = Mode;
        using vec_type = vector<scalar, 3, Mode>;

        union
        {
            struct
            {
                scalar scal;
                vec_type vec;
            };
            struct
            {
                scalar w, i, j, k;
            };
            __m128 intrin[(sizeof(scalar) * size) / sizeof(__m128)];

            scalar data[size];
        };

        [[rythe_always_inline]] constexpr quaternion() noexcept;
        [[rythe_always_inline]] constexpr quaternion(const quaternion&) noexcept = default;
        [[rythe_always_inline]] constexpr quaternion(scalar _w, scalar _i, scalar _j, scalar _k) noexcept;

        template <vector_type VecType>
            requires(VecType::size == 3)
        [[rythe_always_inline]] constexpr quaternion(typename VecType::scalar s, const VecType& v) noexcept;

        template <vector_type VecType>
            requires(VecType::size == 4)
        [[rythe_always_inline]] explicit constexpr quaternion(const VecType& other) noexcept;

        template <math::storage_mode M>
        [[rythe_always_inline]] explicit constexpr quaternion(const matrix<scalar, 3, 3, M>& m) noexcept;
        template <math::storage_mode M>
        [[rythe_always_inline]] explicit constexpr quaternion(const matrix<scalar, 4, 4, M>& m) noexcept;

        static const quaternion identity;
        static const quaternion rotate_x_45;
        static const quaternion rotate_y_45;
        static const quaternion rotate_z_45;
        static const quaternion rotate_x_90;
        static const quaternion rotate_y_90;
        static const quaternion rotate_z_90;
        static const quaternion rotate_x_180;
        static const quaternion rotate_y_180;
        static const quaternion rotate_z_180;

        constexpr quaternion& operator=(const quaternion&) noexcept = default;

        [[nodiscard]] [[rythe_always_inline]] constexpr scalar& operator[](size_type index) noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const scalar& operator[](size_type index) const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr vec_type right() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr vec_type forward() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr vec_type up() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr vec_type euler_angles() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] static constexpr quaternion
        angle_axis(radians<scalar> angle, const vec_type& vec) noexcept;
        [[nodiscard]] [[rythe_always_inline]] static constexpr quaternion
        look_at(const vec_type& pos, const vec_type& center, const vec_type& up = vec_type::up) noexcept;
        [[nodiscard]] [[rythe_always_inline]] static constexpr quaternion conjugate(const quaternion& quat) noexcept;
        [[nodiscard]] [[rythe_always_inline]] static constexpr quaternion from_euler(const vec_type& euler) noexcept;
    };

    template <arithmetic_type Scalar, storage_mode Mode>
    [[nodiscard]] [[rythe_always_inline]] constexpr vector<Scalar, 4, Mode> as_vec(quaternion<Scalar, Mode> quat
    ) noexcept;

    using quat = quaternion<float32>;
    using quat64 = quaternion<float64>;

#ifdef RYTHE_PCH
    template struct quaternion<float32>;
    template struct quaternion<float64>;
#endif
} // namespace rsl::math

#include "matrix_quat_conv.inl"

#include "quaternion_base.inl"

RYTHE_MSVC_SUPPRESS_WARNING_POP
