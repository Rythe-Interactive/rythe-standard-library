#pragma once
#include "../../util/primitives.hpp"
#include "../vector/vector.hpp"

namespace rsl::math
{
    namespace internal
    {
        template <vector_type vec_type>
        struct compute_less
        {
            using result_type = vector<bool, vec_type::size, vec_type::mode>;

            [[nodiscard]] [[rythe_always_inline]] constexpr static result_type
            compute(const vec_type& a, const vec_type& b) noexcept
            {
                result_type result;
                for (size_type i = 0; i < vec_type::size; i++)
                {
                    result[i] = a[i] < b[i];
                }
                return result;
            }

            [[nodiscard]] [[rythe_always_inline]] constexpr static result_type
            compute(const vec_type& a, typename vec_type::scalar b) noexcept
            {
                result_type result;
                for (size_type i = 0; i < vec_type::size; i++)
                {
                    result[i] = a[i] < b;
                }
                return result;
            }
        };

        template <vector_type vec_type>
        struct compute_lequal
        {
            using result_type = vector<bool, vec_type::size, vec_type::mode>;

            [[nodiscard]] [[rythe_always_inline]] constexpr static result_type
            compute(const vec_type& a, const vec_type& b) noexcept
            {
                result_type result;
                for (size_type i = 0; i < vec_type::size; i++)
                {
                    result[i] = a[i] <= b[i];
                }
                return result;
            }

            [[nodiscard]] [[rythe_always_inline]] constexpr static result_type
            compute(const vec_type& a, typename vec_type::scalar b) noexcept
            {
                result_type result;
                for (size_type i = 0; i < vec_type::size; i++)
                {
                    result[i] = a[i] <= b;
                }
                return result;
            }
        };
    } // namespace internal

    template <vector_type vec_type0, vector_type vec_type1>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto less(const vec_type0& a, const vec_type1& b) noexcept
    {
        return internal::compute_less<elevated_t<vec_type0, vec_type1>>::compute(a, b);
    }

    template <vector_type vec_type0, vector_type vec_type1>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto operator<(const vec_type0& a, const vec_type1& b) noexcept
    {
        return internal::compute_less<elevated_t<vec_type0, vec_type1>>::compute(a, b);
    }

    template <vector_type vec_type>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto less(const vec_type& a, typename vec_type::scalar b) noexcept
    {
        return internal::compute_less<vec_type>::compute(a, b);
    }

    template <vector_type vec_type>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto
    operator<(const vec_type& a, typename vec_type::scalar b) noexcept
    {
        return internal::compute_less<vec_type>::compute(a, b);
    }

    template <vector_type vec_type0, vector_type vec_type1>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto lequal(const vec_type0& a, const vec_type1& b) noexcept
    {
        return internal::compute_lequal<elevated_t<vec_type0, vec_type1>>::compute(a, b);
    }

    template <vector_type vec_type0, vector_type vec_type1>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto operator<=(const vec_type0& a, const vec_type1& b) noexcept
    {
        return internal::compute_lequal<elevated_t<vec_type0, vec_type1>>::compute(a, b);
    }

    template <vector_type vec_type>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto lequal(const vec_type& a, typename vec_type::scalar b) noexcept
    {
        return internal::compute_lequal<vec_type>::compute(a, b);
    }

    template <vector_type vec_type>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto
    operator<=(const vec_type& a, typename vec_type::scalar b) noexcept
    {
        return internal::compute_lequal<vec_type>::compute(a, b);
    }
} // namespace rsl::math
