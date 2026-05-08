#pragma once

namespace rsl::math::internal
{
    template <typename T>
    struct compute_inverse;

    template <typename Scalar, size_type RowCount, size_type ColCount, storage_mode Mode>
    struct compute_inverse<matrix<Scalar, RowCount, ColCount, Mode>>
    {
        using mat_type = matrix<Scalar, RowCount, ColCount, Mode>;

        [[nodiscard]] [[rythe_always_inline]] static mat_type compute(const mat_type& m) noexcept
        {
            static_assert(RowCount == ColCount, "Determinants can only be calculated of square matrices");

            if constexpr (RowCount == 2)
            {
                Scalar oneOverDeterminant = static_cast<Scalar>(1) / (+m[0][0] * m[1][1] - m[1][0] * m[0][1]);

                matrix<Scalar, 2, 2, Mode> inverse(
                    +m[1][1] * oneOverDeterminant, -m[0][1] * oneOverDeterminant, -m[1][0] * oneOverDeterminant,
                    +m[0][0] * oneOverDeterminant
                );

                return inverse;
            }
            else if constexpr (RowCount == 3)
            {
                Scalar oneOverDeterminant =
                    static_cast<Scalar>(1) / (+m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2]) -
                                              m[1][0] * (m[0][1] * m[2][2] - m[2][1] * m[0][2]) +
                                              m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]));

                matrix<Scalar, 3, 3, Mode> inverse;
                inverse[0][0] = +(m[1][1] * m[2][2] - m[2][1] * m[1][2]) * oneOverDeterminant;
                inverse[1][0] = -(m[1][0] * m[2][2] - m[2][0] * m[1][2]) * oneOverDeterminant;
                inverse[2][0] = +(m[1][0] * m[2][1] - m[2][0] * m[1][1]) * oneOverDeterminant;
                inverse[0][1] = -(m[0][1] * m[2][2] - m[2][1] * m[0][2]) * oneOverDeterminant;
                inverse[1][1] = +(m[0][0] * m[2][2] - m[2][0] * m[0][2]) * oneOverDeterminant;
                inverse[2][1] = -(m[0][0] * m[2][1] - m[2][0] * m[0][1]) * oneOverDeterminant;
                inverse[0][2] = +(m[0][1] * m[1][2] - m[1][1] * m[0][2]) * oneOverDeterminant;
                inverse[1][2] = -(m[0][0] * m[1][2] - m[1][0] * m[0][2]) * oneOverDeterminant;
                inverse[2][2] = +(m[0][0] * m[1][1] - m[1][0] * m[0][1]) * oneOverDeterminant;
                return inverse;
            }
            else if constexpr (RowCount == 4)
            {

                Scalar coef00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
                Scalar coef02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
                Scalar coef03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

                Scalar coef04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
                Scalar coef06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
                Scalar coef07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

                Scalar coef08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
                Scalar coef10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
                Scalar coef11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

                Scalar coef12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
                Scalar coef14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
                Scalar coef15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

                Scalar coef16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
                Scalar coef18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
                Scalar coef19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

                Scalar coef20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
                Scalar coef22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
                Scalar coef23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

                vector<Scalar, 4> fac0(coef00, coef00, coef02, coef03);
                vector<Scalar, 4> fac1(coef04, coef04, coef06, coef07);
                vector<Scalar, 4> fac2(coef08, coef08, coef10, coef11);
                vector<Scalar, 4> fac3(coef12, coef12, coef14, coef15);
                vector<Scalar, 4> fac4(coef16, coef16, coef18, coef19);
                vector<Scalar, 4> fac5(coef20, coef20, coef22, coef23);

                vector<Scalar, 4> vec0(m[1][0], m[0][0], m[0][0], m[0][0]);
                vector<Scalar, 4> vec1(m[1][1], m[0][1], m[0][1], m[0][1]);
                vector<Scalar, 4> vec2(m[1][2], m[0][2], m[0][2], m[0][2]);
                vector<Scalar, 4> vec3(m[1][3], m[0][3], m[0][3], m[0][3]);

                vector<Scalar, 4> inv0(vec1 * fac0 - vec2 * fac1 + vec3 * fac2);
                vector<Scalar, 4> inv1(vec0 * fac0 - vec2 * fac3 + vec3 * fac4);
                vector<Scalar, 4> inv2(vec0 * fac1 - vec1 * fac3 + vec3 * fac5);
                vector<Scalar, 4> inv3(vec0 * fac2 - vec1 * fac4 + vec2 * fac5);

                vector<Scalar, 4> signA(+1, -1, +1, -1);
                vector<Scalar, 4> signB(-1, +1, -1, +1);
                matrix<Scalar, 4, 4> inverse(inv0 * signA, inv1 * signB, inv2 * signA, inv3 * signB);

                vector<Scalar, 4> row0(inverse[0][0], inverse[1][0], inverse[2][0], inverse[3][0]);

                vector<Scalar, 4> dot0(m[0] * row0);
                Scalar dot1 = (dot0.x + dot0.y) + (dot0.z + dot0.w);

                matrix<Scalar, 4, 4> result(inverse[0] / dot1, inverse[1] / dot1, inverse[2] / dot1, inverse[3] / dot1);
                return result;
            }
            else
            {
                mat_type result;
                auto inverseDet = static_cast<Scalar>(1) / determinant(m);
                auto adj = adjoint(m);
                for (size_type i = 0; i < RowCount; i++)
                {
                    for (size_type j = 0; j < ColCount; j++)
                    {
                        result[i][j] = adj[i][j] * inverseDet;
                    }
                }

                return result;
            }
        }
    };
} // namespace rsl::math::internal
