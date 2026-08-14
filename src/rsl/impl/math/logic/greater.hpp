#pragma once
#include "../../rsl_core.hpp"
#include "../vector/vector.hpp"

namespace rsl::math
{
    namespace internal
    {
        template <vector_type vec_type>
        struct compute_greater
        {
            using result_type = vector<bool, vec_type::size, vec_type::mode>;

            [[nodiscard]] [[rythe_always_inline]] constexpr static result_type
            compute(const vec_type& a, const vec_type& b) noexcept
            {
                result_type result;
                for (size_type i = 0; i < vec_type::size; i++)
                {
                    result[i] = a[i] > b[i];
                }
                return result;
            }

            [[nodiscard]] [[rythe_always_inline]] constexpr static result_type
            compute(const vec_type& a, typename vec_type::scalar b) noexcept
            {
                result_type result;
                for (size_type i = 0; i < vec_type::size; i++)
                {
                    result[i] = a[i] > b;
                }
                return result;
            }
        };

        template <vector_type vec_type>
        struct compute_gequal
        {
            using result_type = vector<bool, vec_type::size, vec_type::mode>;

            [[nodiscard]] [[rythe_always_inline]] constexpr static result_type
            compute(const vec_type& a, const vec_type& b) noexcept
            {
                result_type result;
                for (size_type i = 0; i < vec_type::size; i++)
                {
                    result[i] = a[i] >= b[i];
                }
                return result;
            }

            [[nodiscard]] [[rythe_always_inline]] constexpr static result_type
            compute(const vec_type& a, typename vec_type::scalar b) noexcept
            {
                result_type result;
                for (size_type i = 0; i < vec_type::size; i++)
                {
                    result[i] = a[i] >= b;
                }
                return result;
            }
        };
    } // namespace internal

    template <vector_type vec_type0, vector_type vec_type1>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto greater(const vec_type0& a, const vec_type1& b) noexcept
    {
        return internal::compute_greater<elevated_t<vec_type0, vec_type1>>::compute(a, b);
    }

    template <vector_type vec_type0, vector_type vec_type1>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto operator>(const vec_type0& a, const vec_type1& b) noexcept
    {
        return internal::compute_greater<elevated_t<vec_type0, vec_type1>>::compute(a, b);
    }

    template <vector_type vec_type>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto
    greater(const vec_type& a, typename vec_type::scalar b) noexcept
    {
        return internal::compute_greater<vec_type>::compute(a, b);
    }

    template <vector_type vec_type>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto
    operator>(const vec_type& a, typename vec_type::scalar b) noexcept
    {
        return internal::compute_greater<vec_type>::compute(a, b);
    }

    template <vector_type vec_type0, vector_type vec_type1>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto gequal(const vec_type0& a, const vec_type1& b) noexcept
    {
        return internal::compute_gequal<elevated_t<vec_type0, vec_type1>>::compute(a, b);
    }

    template <vector_type vec_type0, vector_type vec_type1>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto operator>=(const vec_type0& a, const vec_type1& b) noexcept
    {
        return internal::compute_gequal<elevated_t<vec_type0, vec_type1>>::compute(a, b);
    }

    template <vector_type vec_type>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto gequal(const vec_type& a, typename vec_type::scalar b) noexcept
    {
        return internal::compute_gequal<vec_type>::compute(a, b);
    }

    template <vector_type vec_type>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto
    operator>=(const vec_type& a, typename vec_type::scalar b) noexcept
    {
        return internal::compute_gequal<vec_type>::compute(a, b);
    }
} // namespace rsl::math
