#pragma once
#include "util.hpp"

namespace rsl::math
{
    template <arithmetic_type Scalar>
    constexpr matrix<Scalar, 4, 4> perspective(radians<Scalar> fovY, Scalar aspect, Scalar nearZ, Scalar farZ) noexcept
    {
        Scalar const tanHalfFovY = tan(fovY.value * static_cast<Scalar>(0.5));
        Scalar inverseFrustumDepth = static_cast<Scalar>(1) / (farZ - nearZ);

        matrix<Scalar, 4, 4> result(static_cast<Scalar>(0));

        result[0][0] = static_cast<Scalar>(1) / (aspect * tanHalfFovY);
        result[1][1] = static_cast<Scalar>(1) / (tanHalfFovY);
        result[2][2] = (farZ + nearZ) * inverseFrustumDepth;

        result[2][3] = static_cast<Scalar>(1);
        result[3][2] = -(static_cast<Scalar>(2) * nearZ * farZ) * inverseFrustumDepth;

        return result;
    }

    template <arithmetic_type Scalar>
    constexpr matrix<Scalar, 4, 4>
    orthographic(Scalar left, Scalar right, Scalar bottom, Scalar top, Scalar nearZ, Scalar farZ) noexcept
    {
        vector<Scalar, 3> frustumSize = vector<Scalar, 3>(right, top, farZ) - vector<Scalar, 3>(left, bottom, nearZ);
        vector<Scalar, 3> inverseFrustumSize = vector<Scalar, 3>(static_cast<Scalar>(1)) / frustumSize;

        vector<Scalar, 3> result0 =
            vector<Scalar, 3>(static_cast<Scalar>(2), static_cast<Scalar>(2), static_cast<Scalar>(1)) *
            inverseFrustumSize;

        matrix<Scalar, 4, 4> result(static_cast<Scalar>(1));

        result[0][0] = result0.x;
        result[1][1] = result0.y;
        result[2][2] = result0.z;

        result[3].xyz = vector<Scalar, 3>(-(right + left), -(top + bottom), -nearZ) * inverseFrustumSize;

        return result;
    }

    template <arithmetic_type Scalar, storage_mode ModeEye, storage_mode ModeCenter, storage_mode ModeUp>
    constexpr matrix<Scalar, 4, 4, elevated_storage_mode_v<ModeEye, elevated_storage_mode_v<ModeCenter, ModeUp>>>
    look_at(vector<Scalar, 3, ModeEye> eye, vector<Scalar, 3, ModeCenter> center, vector<Scalar, 3, ModeUp> up) noexcept
    {
        constexpr storage_mode mode = elevated_storage_mode_v<ModeEye, elevated_storage_mode_v<ModeCenter, ModeUp>>;
        const vector<Scalar, 4, mode> f(normalize(center - eye));
        const vector<Scalar, 4, mode> r(normalize(cross(up, vector<Scalar, 3, mode>(f))));
        // Length of u is 1 because the angle between f and r is 90 degrees
        const vector<Scalar, 4, mode> u(cross(vector<Scalar, 3, mode>(f), vector<Scalar, 3, mode>(r)));

        matrix<Scalar, 4, 4, mode> result(static_cast<Scalar>(1));
        result[0] = r;
        result[1] = u;
        result[2] = f;
        result[3] = vector<Scalar, 4, mode>(eye, static_cast<Scalar>(1));

        return result;
    }

    namespace internal
    {
        template <arithmetic_type Scalar, size_type Size, storage_mode Mode>
        constexpr matrix<Scalar, Size, Size, Mode> transpose_impl(const matrix<Scalar, Size, Size, Mode>& mat) noexcept
        {
            matrix<Scalar, Size, Size, Mode> result{};
            for (size_t i = 0; i < Size; ++i)
            {
                for (size_t j = 0; j < Size; ++j)
                {
                    result[j][i] = mat[i][j];
                }
            }
            return result;
        }
    } // namespace internal

    template <arithmetic_type Scalar, size_type Size, storage_mode Mode>
    constexpr matrix<Scalar, Size, Size, Mode> transpose(const matrix<Scalar, Size, Size, Mode>& mat) noexcept
    {
        if constexpr (Size == 4)
        {
            if (is_constant_evaluated())
            {
                return internal::transpose_impl(mat);
            }
            else
            {
                matrix<Scalar, 4, 4, Mode> result(1);
                result.row0 = mat.col0;
                result.row1 = mat.col1;
                result.row2 = mat.col2;
                result.row3 = mat.col3;
                return result;
            }
        }
        else if constexpr (Size == 3)
        {
            if (is_constant_evaluated())
            {
                return internal::transpose_impl(mat);
            }
            else
            {
                matrix<Scalar, 3, 3, Mode> result(1);
                result.row0 = mat.col0;
                result.row1 = mat.col1;
                result.row2 = mat.col2;
                return result;
            }
        }
        else
        {
            return internal::transpose_impl(mat);
        }
    }

    template <arithmetic_type Scalar, storage_mode MatMode, storage_mode VecMode>
    constexpr matrix<Scalar, 4, 4, elevated_storage_mode_v<MatMode, VecMode>>
    translate(const matrix<Scalar, 4, 4, MatMode>& mat, const vector<Scalar, 3, VecMode>& pos) noexcept
    {
        constexpr storage_mode mode = elevated_storage_mode_v<MatMode, VecMode>;
        if (is_constant_evaluated())
        {
            matrix<Scalar, 4, 4, mode> result(mat);
            result[3][0] += pos[0];
            result[3][1] += pos[1];
            result[3][2] += pos[2];
            return result;
        }
        else
        {
            matrix<Scalar, 4, 4, mode> result(mat);
            result[3].xyz += pos;
            return result;
        }
    }

    // template<typename Scalar>
    //[[nodiscard]] matrix<Scalar, 4, 4> rotate(matrix<Scalar, 4, 4> matrix, quaternion<Scalar> orientation) noexcept
    //{

    //}

    template <arithmetic_type Scalar, storage_mode MatMode, storage_mode VecMode>
    constexpr matrix<Scalar, 4, 4, elevated_storage_mode_v<MatMode, VecMode>> rotate(
        const matrix<Scalar, 4, 4, MatMode>& mat, radians<Scalar> angle, const vector<Scalar, 3, VecMode>& _axis
    ) noexcept
    {
        constexpr storage_mode mode = elevated_storage_mode_v<MatMode, VecMode>;
        Scalar const a = angle;
        Scalar const c = cos(a);
        Scalar const s = sin(a);

        vector<Scalar, 3, mode> axis(normalize(_axis));
        vector<Scalar, 3, mode> temp((Scalar(1) - c) * axis);

        matrix<Scalar, 4, 4, mode> rot;
        rot[0][0] = c + temp[0] * axis[0];
        rot[0][1] = temp[0] * axis[1] + s * axis[2];
        rot[0][2] = temp[0] * axis[2] - s * axis[1];

        rot[1][0] = temp[1] * axis[0] - s * axis[2];
        rot[1][1] = c + temp[1] * axis[1];
        rot[1][2] = temp[1] * axis[2] + s * axis[0];

        rot[2][0] = temp[2] * axis[0] + s * axis[1];
        rot[2][1] = temp[2] * axis[1] - s * axis[0];
        rot[2][2] = c + temp[2] * axis[2];

        matrix<Scalar, 4, 4, mode> result;
        result[0] = mat[0] * rot[0][0] + mat[1] * rot[0][1] + mat[2] * rot[0][2];
        result[1] = mat[0] * rot[1][0] + mat[1] * rot[1][1] + mat[2] * rot[1][2];
        result[2] = mat[0] * rot[2][0] + mat[1] * rot[2][1] + mat[2] * rot[2][2];
        result[3] = mat[3];
        return result;
    }

    template <arithmetic_type Scalar, storage_mode MatMode, storage_mode VecMode>
    constexpr matrix<Scalar, 4, 4, elevated_storage_mode_v<MatMode, VecMode>>
    scale(const matrix<Scalar, 4, 4, MatMode>& mat, const vector<Scalar, 3, VecMode>& scale) noexcept
    {
        constexpr storage_mode mode = elevated_storage_mode_v<MatMode, VecMode>;
        matrix<Scalar, 4, 4, mode> result;
        result[0] = mat[0] * scale[0];
        result[1] = mat[1] * scale[1];
        result[2] = mat[2] * scale[2];
        result[3] = mat[3];
        return result;
    }
} // namespace rsl::math
