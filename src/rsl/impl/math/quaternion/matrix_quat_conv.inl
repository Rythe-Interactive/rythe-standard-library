#pragma once

namespace rsl::math
{
    template <arithmetic_type Scalar, storage_mode Mode>
    template <arithmetic_type Scal, storage_mode M>
    constexpr matrix<Scalar, 2, 2, Mode>::matrix(const quaternion<Scal, M>& orientation) noexcept
        : columns{}
    {
        const Scalar i2 = static_cast<Scalar>(orientation.i * orientation.i);
        const Scalar j2 = static_cast<Scalar>(orientation.j * orientation.j);
        const Scalar k2 = static_cast<Scalar>(orientation.k * orientation.k);
        const Scalar ij = static_cast<Scalar>(orientation.i * orientation.j);
        const Scalar wk = static_cast<Scalar>(orientation.w * orientation.k);

        columns[0][0] = static_cast<Scalar>(1) - static_cast<Scalar>(2) * (j2 + k2);
        columns[0][1] = static_cast<Scalar>(2) * (ij + wk);

        columns[1][0] = static_cast<Scalar>(2) * (ij - wk);
        columns[1][1] = static_cast<Scalar>(1) - static_cast<Scalar>(2) * (i2 + k2);
    }

    template <arithmetic_type Scalar, storage_mode Mode>
    template <arithmetic_type Scal, storage_mode M>
    constexpr matrix<Scalar, 3, 3, Mode>::matrix(const quaternion<Scal, M>& orientation) noexcept
        : columns{}
    {
        const Scalar i2 = static_cast<Scalar>(orientation.i * orientation.i);
        const Scalar j2 = static_cast<Scalar>(orientation.j * orientation.j);
        const Scalar k2 = static_cast<Scalar>(orientation.k * orientation.k);
        const Scalar ik = static_cast<Scalar>(orientation.i * orientation.k);
        const Scalar ij = static_cast<Scalar>(orientation.i * orientation.j);
        const Scalar jk = static_cast<Scalar>(orientation.j * orientation.k);
        const Scalar iw = static_cast<Scalar>(orientation.i * orientation.w);
        const Scalar jw = static_cast<Scalar>(orientation.j * orientation.w);
        const Scalar kw = static_cast<Scalar>(orientation.k * orientation.w);

        columns[0][0] = static_cast<Scalar>(1) - static_cast<Scalar>(2) * (j2 + k2);
        columns[0][1] = static_cast<Scalar>(2) * (ij + kw);
        columns[0][2] = static_cast<Scalar>(2) * (ik - jw);

        columns[1][0] = static_cast<Scalar>(2) * (ij - kw);
        columns[1][1] = static_cast<Scalar>(1) - static_cast<Scalar>(2) * (i2 + k2);
        columns[1][2] = static_cast<Scalar>(2) * (jk + iw);

        columns[2][0] = static_cast<Scalar>(2) * (ik + jw);
        columns[2][1] = static_cast<Scalar>(2) * (jk - iw);
        columns[2][2] = static_cast<Scalar>(1) - static_cast<Scalar>(2) * (i2 + j2);
    }

    template <arithmetic_type Scalar, storage_mode Mode>
    template <arithmetic_type Scal0, math::storage_mode M0, arithmetic_type Scal1, math::storage_mode M1>
    constexpr matrix<Scalar, 4, 4, Mode>::matrix(
        const quaternion<Scal0, M0>& orientation, const vector<Scal1, 3, M1>& position
    ) noexcept
        : columns{}
    {
        const Scalar i2 = static_cast<Scalar>(orientation.i * orientation.i);
        const Scalar j2 = static_cast<Scalar>(orientation.j * orientation.j);
        const Scalar k2 = static_cast<Scalar>(orientation.k * orientation.k);
        const Scalar ik = static_cast<Scalar>(orientation.i * orientation.k);
        const Scalar ij = static_cast<Scalar>(orientation.i * orientation.j);
        const Scalar jk = static_cast<Scalar>(orientation.j * orientation.k);
        const Scalar iw = static_cast<Scalar>(orientation.i * orientation.w);
        const Scalar jw = static_cast<Scalar>(orientation.j * orientation.w);
        const Scalar kw = static_cast<Scalar>(orientation.k * orientation.w);

        columns[0][0] = static_cast<Scalar>(1) - static_cast<Scalar>(2) * (j2 + k2);
        columns[0][1] = static_cast<Scalar>(2) * (ij + kw);
        columns[0][2] = static_cast<Scalar>(2) * (ik - jw);

        columns[1][0] = static_cast<Scalar>(2) * (ij - kw);
        columns[1][1] = static_cast<Scalar>(1) - static_cast<Scalar>(2) * (i2 + k2);
        columns[1][2] = static_cast<Scalar>(2) * (jk + iw);

        columns[2][0] = static_cast<Scalar>(2) * (ik + jw);
        columns[2][1] = static_cast<Scalar>(2) * (jk - iw);
        columns[2][2] = static_cast<Scalar>(1) - static_cast<Scalar>(2) * (i2 + j2);

        columns[3][0] = position[0];
        columns[3][1] = position[1];
        columns[3][2] = position[2];
        columns[3][3] = static_cast<Scalar>(1);
    }

    template <arithmetic_type Scalar, storage_mode Mode>
    template <storage_mode M>
    constexpr quaternion<Scalar, Mode>::quaternion(const matrix<Scalar, 3, 3, M>& m) noexcept
    {
        const vector<Scalar, 3, Mode> invScale{1.f / length(m[0]), 1.f / length(m[1]), 1.f / length(m[2])};
        const vector<Scalar, 3, Mode> m0 = m[0] * invScale[0];
        const vector<Scalar, 3, Mode> m1 = m[1] * invScale[1];
        const vector<Scalar, 3, Mode> m2 = m[2] * invScale[2];

        const Scalar qwijk[] = {
            m0[0] + m1[1] + m2[2], m0[0] - m1[1] - m2[2], m1[1] - m0[0] - m2[2], m2[2] - m0[0] - m1[1]
        };

        size_type idx = 0;
        Scalar qMax = qwijk[0];
        for (size_type i = 1; i < 4; ++i)
        {
            if (qwijk[i] > qMax)
            {
                qMax = qwijk[i];
                idx = i;
            }
        }

        qMax = sqrt(qMax + static_cast<Scalar>(1)) * static_cast<Scalar>(0.5);
        Scalar mult = static_cast<Scalar>(0.25) / qMax;

        switch (idx)
        {
            case 0:
            {
                w = qMax;
                i = (m1[2] - m2[1]) * mult;
                j = (m2[0] - m0[2]) * mult;
                k = (m0[1] - m1[0]) * mult;
            }
            break;
            case 1:
            {
                w = (m1[2] - m2[1]) * mult;
                i = qMax;
                j = (m0[1] + m1[0]) * mult;
                k = (m2[0] + m0[2]) * mult;
            }
            break;
            case 2:
            {
                w = (m2[0] - m0[2]) * mult;
                i = (m0[1] + m1[0]) * mult;
                j = qMax;
                k = (m1[2] + m2[1]) * mult;
            }
            break;
            case 3:
            {
                w = (m0[1] - m1[0]) * mult;
                i = (m2[0] + m0[2]) * mult;
                j = (m1[2] + m2[1]) * mult;
                k = qMax;
            }
            break;
            default: rsl_assert_unreachable();
        }
    }

    template <arithmetic_type Scalar, storage_mode Mode>
    template <storage_mode M>
    constexpr quaternion<Scalar, Mode>::quaternion(const matrix<Scalar, 4, 4, M>& m) noexcept
    {
        vector<Scalar, 3, Mode> m0(m[0]);
        vector<Scalar, 3, Mode> m1(m[1]);
        vector<Scalar, 3, Mode> m2(m[2]);

        const vector<Scalar, 3, Mode> invScale{1.f / length(m0), 1.f / length(m1), 1.f / length(m2)};

        m0 *= invScale[0];
        m1 *= invScale[1];
        m2 *= invScale[2];

        const Scalar qwijk[] = {
            m0[0] + m1[1] + m2[2], m0[0] - m1[1] - m2[2], m1[1] - m0[0] - m2[2], m2[2] - m0[0] - m1[1]
        };

        size_type idx = 0;
        Scalar qMax = qwijk[0];
        for (size_type i = 1; i < 4; ++i)
        {
            if (qwijk[i] > qMax)
            {
                qMax = qwijk[i];
                idx = i;
            }
        }

        qMax = sqrt(qMax + static_cast<Scalar>(1)) * static_cast<Scalar>(0.5);
        Scalar mult = static_cast<Scalar>(0.25) / qMax;

        switch (idx)
        {
            case 0:
            {
                w = qMax;
                i = (m1[2] - m2[1]) * mult;
                j = (m2[0] - m0[2]) * mult;
                k = (m0[1] - m1[0]) * mult;
            }
            break;
            case 1:
            {
                w = (m1[2] - m2[1]) * mult;
                i = qMax;
                j = (m0[1] + m1[0]) * mult;
                k = (m2[0] + m0[2]) * mult;
            }
            break;
            case 2:
            {
                w = (m2[0] - m0[2]) * mult;
                i = (m0[1] + m1[0]) * mult;
                j = qMax;
                k = (m1[2] + m2[1]) * mult;
            }
            break;
            case 3:
            {
                w = (m0[1] - m1[0]) * mult;
                i = (m2[0] + m0[2]) * mult;
                j = (m1[2] + m2[1]) * mult;
                k = qMax;
            }
            break;
            default: rsl_assert_unreachable();
        }
    }
} // namespace rsl::math
