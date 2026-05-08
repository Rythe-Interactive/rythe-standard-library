#pragma once
#include "decompose.hpp"

namespace rsl::math
{
    template <typename Scalar>
    [[nodiscard]] vector<Scalar, 3> scale(const matrix<Scalar, 4, 4>& mat) noexcept
    {
        return {length(mat[0].xyz), length(mat[1].xyz), length(mat[2].xyz)};
    }

    template <typename Scalar>
    [[nodiscard]] vector<Scalar, 3> scale(const matrix<Scalar, 3, 3>& mat) noexcept
    {
        return {length(mat[0].xyz), length(mat[1].xyz), length(mat[2].xyz)};
    }

    template <typename Scalar>
    [[nodiscard]] quaternion<Scalar> orientation(const matrix<Scalar, 4, 4>& mat) noexcept
    {
        return quaternion<Scalar>{mat};
    }

    template <typename Scalar>
    [[nodiscard]] quaternion<Scalar> orientation(const matrix<Scalar, 3, 3>& mat) noexcept
    {
        return quaternion<Scalar>{mat};
    }

    template <typename Scalar>
    [[nodiscard]] constexpr vector<Scalar, 3> translation(const matrix<Scalar, 4, 4>& mat) noexcept
    {
        return mat[3].xyz;
    }

    template <typename Scalar>
    void decompose(
        const matrix<Scalar, 4, 4>& mat, vector<Scalar, 3>& scale, quaternion<Scalar>& orientation,
        vector<Scalar, 3>& translation
    ) noexcept
    {
        scale.x = length(mat[0].xyz);
        scale.y = length(mat[1].xyz);
        scale.z = length(mat[2].xyz);
        translation = mat[3].xyz;
        orientation = quaternion<Scalar>(mat);
    }

    template <typename Scalar>
    void decompose(const matrix<Scalar, 3, 3>& mat, vector<Scalar, 3>& scale, quaternion<Scalar>& orientation) noexcept
    {
        scale.x = length(mat[0].xyz);
        scale.y = length(mat[1].xyz);
        scale.z = length(mat[2].xyz);
        orientation = quaternion<Scalar>{mat};
    }

    template <typename Scalar>
    matrix<Scalar, 4, 4> compose(
        const vector<Scalar, 3>& scale, const quaternion<Scalar>& orientation, const vector<Scalar, 3>& translation
    ) noexcept
    {
        matrix<Scalar, 4, 4> result(orientation);
        result[3].xyz = translation;
        result[0].xyz *= scale[0];
        result[1].xyz *= scale[1];
        result[2].xyz *= scale[2];
        return result;
    }

    template <typename Scalar>
    matrix<Scalar, 3, 3> compose(const vector<Scalar, 3>& scale, const quaternion<Scalar>& orientation) noexcept
    {
        matrix<Scalar, 3, 3> result(orientation);
        result[0] *= scale[0];
        result[1] *= scale[1];
        result[2] *= scale[2];
        return result;
    }
} // namespace rsl::math
