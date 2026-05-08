#pragma once
#include <cmath>

#include "../../util/primitives.hpp"
#include "../util/type_util.hpp"

namespace rsl::math
{
    namespace internal
    {
        template <typename T>
        struct compute_mod;

        template <arithmetic_type Scalar, size_type Size, storage_mode Mode>
        struct compute_mod<vector<Scalar, Size, Mode>>
        {
            static constexpr size_type size = Size;
            using value_type = vector<Scalar, size, Mode>;

            [[nodiscard]] [[rythe_always_inline]] static value_type computef(const value_type& val, Scalar m) noexcept
            {
                value_type result;
                for (size_type i = 0; i < size; i++)
                {
                    result[i] = ::std::fmod(val[i], m);
                }
                return result;
            }

            [[nodiscard]] [[rythe_always_inline]] static value_type
            computef(const value_type& val, const value_type& m) noexcept
            {
                value_type result;
                for (size_type i = 0; i < size; i++)
                {
                    result[i] = ::std::fmod(val[i], m[i]);
                }
                return result;
            }

            [[nodiscard]] constexpr static value_type compute(const value_type& val, Scalar m) noexcept
            {
                value_type result;
                for (size_type i = 0; i < size; i++)
                {
                    result[i] = val[i] % m;
                }
                return result;
            }

            [[nodiscard]] constexpr static value_type compute(const value_type& val, const value_type& m) noexcept
            {
                value_type result;
                for (size_type i = 0; i < size; i++)
                {
                    result[i] = val[i] % m[i];
                }
                return result;
            }
        };

        template <arithmetic_type Scalar, storage_mode Mode>
        struct compute_mod<vector<Scalar, 1ull, Mode>>
        {
            static constexpr size_type size = 1ull;
            using value_type = vector<Scalar, size, Mode>;

            [[nodiscard]] [[rythe_always_inline]] static Scalar computef(Scalar val, Scalar m) noexcept
            {
                return ::std::fmod(val, m);
            }

            [[nodiscard]] constexpr static Scalar compute(Scalar val, Scalar m) noexcept { return val % m; }
        };
    } // namespace internal

    template <typename T>
    [[nodiscard]] [[rythe_always_inline]] static auto fmod(const T& val, const T& m)
    {
        if constexpr (is_vector_v<T>)
        {
            static_assert(
                is_floating_point_v<typename T::scalar>,
                "Value must be floating point in order to use fmod, use mod instead."
            );
            return internal::compute_mod<T>::computef(val, m);
        }
        else
        {
            static_assert(
                is_floating_point_v<T>, "Value must be floating point in order to use fmod, use mod instead."
            );
            return ::std::fmod(val, m);
        }
    }

    template <vector_type VecType>
    [[nodiscard]] [[rythe_always_inline]] static auto fmod(const VecType& val, typename VecType::scalar m)
    {
        static_assert(
            is_floating_point_v<typename VecType::scalar>,
            "Value must be floating point in order to use fmod, use mod instead."
        );
        return internal::compute_mod<VecType>::computef(val, m);
    }

    template <typename T>
    [[nodiscard]] [[rythe_always_inline]] static auto mod(const T& val, const T& m)
    {
        if constexpr (is_vector_v<T>)
        {
            if constexpr (is_floating_point_v<typename T::scalar>)
            {
                return internal::compute_mod<T>::computef(val, m);
            }
            else
            {
                return internal::compute_mod<T>::compute(val, m);
            }
        }
        else if constexpr (is_floating_point_v<T>)
        {
            return ::std::fmod(val, m);
        }
        else
        {
            return val % m;
        }
    }

    template <vector_type VecType>
    [[nodiscard]] [[rythe_always_inline]] static auto mod(const VecType& val, typename VecType::scalar m)
    {
        if constexpr (is_floating_point_v<typename VecType::scalar>)
        {
            return internal::compute_mod<VecType>::computef(val, m);
        }
        else
        {
            return internal::compute_mod<VecType>::compute(val, m);
        }
    }

    template <vector_type VecType>
    [[nodiscard]] [[rythe_always_inline]] auto operator%(const VecType& val, typename VecType::scalar m)
    {
        return mod(val, m);
    }

    template <vector_type VecType>
    [[nodiscard]] [[rythe_always_inline]] auto operator%(const VecType& val, const VecType& m)
    {
        return mod(val, m);
    }
} // namespace rsl::math
