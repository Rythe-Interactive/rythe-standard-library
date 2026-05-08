#pragma once
#include "lerp.hpp"

namespace rsl::math::internal
{
    template <typename T>
    struct compute_lerp;

    template <typename Scalar, size_type Size>
    struct compute_lerp<vector<Scalar, Size>>
    {
        using vec_type = vector<Scalar, Size>;

        template <typename AType, typename BType, typename InterpType>
        [[nodiscard]] constexpr static auto compute(AType&& a, BType&& b, InterpType&& t) noexcept
        {
            if constexpr (is_vector_v<InterpType>)
            {
                return compute_vector(forward<AType>(a), forward<BType>(b), forward<InterpType>(t));
            }
            else
            {
                return compute_scalar(forward<AType>(a), forward<BType>(b), forward<InterpType>(t));
            }
        }

    private:
        template <typename AType, typename BType, typename InterpType>
        [[nodiscard]] constexpr static auto compute_vector(AType&& a, BType&& b, InterpType&& t) noexcept
        {
            vec_type result;
            size_type j = 0;
            for (size_type i = 0; i < Size; i++)
            {
                if constexpr (is_vector_v<AType> && make_vector_t<AType>::size >= Size)
                {
                    if constexpr (is_vector_v<BType> && make_vector_t<BType>::size >= Size)
                    {
                        result[i] = internal::_lerp_impl_(a[i], b[i], t[j]);
                    }
                    else
                    {
                        result[i] = internal::_lerp_impl_(a[i], b, t[j]);
                    }
                }
                else
                {
                    static_assert(is_vector_v<BType> && make_vector_t<BType>::size >= Size);
                    result[i] = internal::_lerp_impl_(a, b[i], t[j]);
                }

                if (j < ::std::remove_cvref_t<InterpType>::size - 1)
                {
                    j++;
                }
            }

            return result;
        }

        template <typename AType, typename BType, typename InterpType>
        [[nodiscard]] constexpr static auto compute_scalar(AType&& a, BType&& b, InterpType&& t) noexcept
        {
            vec_type result;
            for (size_type i = 0; i < Size; i++)
            {
                if constexpr (is_vector_v<AType> && make_vector_t<AType>::size >= Size)
                {
                    if constexpr (is_vector_v<BType> && make_vector_t<BType>::size >= Size)
                    {
                        result[i] = internal::_lerp_impl_(a[i], b[i], forward<InterpType>(t));
                    }
                    else
                    {
                        result[i] = internal::_lerp_impl_(a[i], b, forward<InterpType>(t));
                    }
                }
                else
                {
                    static_assert(is_vector_v<BType> && make_vector_t<BType>::size >= Size);
                    result[i] = internal::_lerp_impl_(a, b[i], forward<InterpType>(t));
                }
            }

            return result;
        }
    };

    template <typename Scalar>
    struct compute_lerp<vector<Scalar, 1u>>
    {
        using vec_type = vector<Scalar, 1u>;

        template <typename AType, typename BType, typename InterpType>
        [[nodiscard]] constexpr static Scalar compute(AType&& a, BType&& b, InterpType&& t) noexcept
        {
            if constexpr (is_vector_v<InterpType>)
            {
                return compute_vector(forward<AType>(a), forward<BType>(b), forward<InterpType>(t));
            }
            else
            {
                return compute_scalar(forward<AType>(a), forward<BType>(b), forward<InterpType>(t));
            }
        }

    private:
        template <typename AType, typename BType, typename InterpType>
        [[nodiscard]] constexpr static Scalar compute_vector(AType&& a, BType&& b, InterpType&& t) noexcept
        {
            if constexpr (is_vector_v<AType>)
            {
                if constexpr (is_vector_v<BType>)
                {
                    return internal::_lerp_impl_(a[0], b[0], t[0]);
                }
                else
                {
                    return internal::_lerp_impl_(a[0], b, t[0]);
                }
            }
            else
            {
                static_assert(is_vector_v<BType>);
                return internal::_lerp_impl_(a, b[0], t[0]);
            }
        }

        template <typename AType, typename BType, typename InterpType>
        [[nodiscard]] constexpr static Scalar compute_scalar(AType&& a, BType&& b, InterpType&& t) noexcept
        {
            if constexpr (is_vector_v<AType>)
            {
                if constexpr (is_vector_v<BType>)
                {
                    return internal::_lerp_impl_(a[0], b[0], forward<InterpType>(t));
                }
                else
                {
                    return internal::_lerp_impl_(a[0], b, forward<InterpType>(t));
                }
            }
            else
            {
                static_assert(is_vector_v<BType>);
                return internal::_lerp_impl_(a, b[0], forward<InterpType>(t));
            }
        }
    };
} // namespace rsl::math::internal
