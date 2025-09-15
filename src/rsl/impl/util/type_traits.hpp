#pragma once

#include "../containers/constexpr_string.hpp"

#include "hash.hpp"

namespace rsl
{
    namespace internal
    {
        template <size_type I>
        consteval constexpr_string<I> post_process_compiler_dependent_type_name(constexpr_string<I> name) noexcept
        {
            constexpr_string ret = name;

            constexpr_string anonymousNamespacefilterGCC = "(anonymous namespace)::";
            ret = ret.filter(anonymousNamespacefilterGCC);
            constexpr_string anonymousNamespacefilterMSVC = "`anonymous-namespace'::";
            ret = ret.filter(anonymousNamespacefilterMSVC);
            constexpr_string cxxfilter = "__cxx11::";
            ret = ret.filter(cxxfilter);
            constexpr_string classFilter = "class ";
            constexpr_string classReplace = "";
            ret = ret.replace(classFilter, classReplace);
            constexpr_string structFilter = "struct ";
            constexpr_string structReplace = "";
            ret = ret.replace(structFilter, structReplace);
            constexpr_string cdeclFilter = "(__cdecl ";
            constexpr_string cdeclReplace = " (";
            ret = ret.replace(cdeclFilter, cdeclReplace);
            constexpr_string funcPtrFilter = "* )";
            constexpr_string funcPtrReplace = "*)";
            ret = ret.replace(funcPtrFilter, funcPtrReplace);
            constexpr_string voidParamFilter = "(void)";
            constexpr_string voidParamReplace = "()";
            ret = ret.replace(voidParamFilter, voidParamReplace);
            constexpr_string refFilter = " &";
            constexpr_string refReplace = "&";
            ret = ret.replace(refFilter, refReplace);
            constexpr_string ptrFilter = " *";
            constexpr_string ptrReplace = "*";
            ret = ret.replace(ptrFilter, ptrReplace);
            constexpr_string commaFilter = ",";
            constexpr_string commaReplace = ", ";
            ret = ret.replace(commaFilter, commaReplace, 0, true);
            constexpr_string comma2Filter = ",  ";
            constexpr_string comma2Replace = ", ";
            ret = ret.replace(comma2Filter, comma2Replace);

            return ret;
        }

        template <typename T>
        consteval auto compiler_dependent_type_name() noexcept
        {
            constexpr string_view functionName = __RYTHE_FULL_FUNC__;

            constexpr_string<functionName.size()> ret{};
            #if defined(RYTHE_MSVC)
            auto first = linear_search(functionName, '<', linear_search_sequence(functionName, "compiler_dependent_type_name"_sv)) + 1;
            auto end = reverse_linear_search(functionName, '>');

            ret.copy_from(functionName.subview(first, end - first));
            #elif defined(RYTHE_GCC)
            auto first = linear_search_not_eq(functionName, ' ', linear_search(functionName, '=') + 1);
            auto end = reverse_linear_search(functionName, ';');
            if (end == std::string_view::npos) { end = reverse_linear_search(functionName, ']'); }

            ret.copy_from(functionName.subview(first, end - first));
            #elif defined(RYTHE_CLANG)
            auto first = linear_search_not_eq(functionName, ' ', linear_search(functionName, '=') + 1);
            ret.copy_from(functionName.subview(first, reverse_linear_search(functionName, ']') - first));
            #else
            ret.copy_from(functionName);
            #endif

            return ret;
        }

        template <template <typename...> typename T>
        consteval auto compiler_dependent_templated_type_name() noexcept
        {
            constexpr string_view functionName = __RYTHE_FULL_FUNC__;

            constexpr_string<functionName.size()> ret{};
            #if defined(RYTHE_MSVC)
            auto first =
                    linear_search(functionName, '<', linear_search_sequence(functionName, "compiler_dependent_templated_type_name"_sv)) +
                    1;
            auto end = reverse_linear_search(functionName, '>');

            ret.copy_from(functionName.subview(first, end - first));
            #elif defined(RYTHE_GCC)
            auto first = linear_search_not_eq(functionName, ' ', linear_search(functionName, '=') + 1);
            ret.copy_from(functionName.subview(first, reverse_linear_search(functionName, ']') - first));
            #elif defined(RYTHE_CLANG)
            auto first = linear_search_not_eq(functionName, ' ', linear_search(functionName, '=') + 1);
            ret.copy_from(functionName.subview(first, reverse_linear_search(functionName, ']') - first));
            #else
            ret.copy_from(functionName);
            #endif

            return ret;
        }

        template <typename T>
        struct compose_type_name
        {
            consteval static auto get_value() noexcept
            {
                constexpr auto ret = post_process_compiler_dependent_type_name(compiler_dependent_type_name<T>());
                return ret.template refit<ret.size() + 1>();
            }
        };

        template <template <typename...> typename T, typename... Args>
        struct compose_type_name<T<Args...>>
        {
            template <typename A, typename... As, size_type N>
            consteval static auto add_types(constexpr_string<N> original)
            {
                auto ret = original + compose_type_name<A>::get_value();

                if constexpr (sizeof...(As) != 0) { return add_types<As...>(ret + constexpr_string(", ")); }
                else { return ret; }
            }

            consteval static auto construct_value() noexcept
            {
                auto a = compiler_dependent_templated_type_name<T>();
                auto b = a + constexpr_string("<");
                auto c = add_types<Args...>(b) + constexpr_string(">");

                return c;
            }

            consteval static auto get_value() noexcept
            {
                constexpr auto ret = post_process_compiler_dependent_type_name(construct_value());
                return ret.template refit<ret.size() + 1>();
            }
        };

    } // namespace internal

    template <typename T, size_type N>
    consteval constexpr_string<N> type_name() noexcept; // NOLINT

    template <>
    consteval constexpr_string<5> type_name<void>() noexcept
    {
        return "void";
    }

    template <typename T>
    consteval auto type_name() noexcept
    {
        return internal::compose_type_name<remove_cvr_t<T>>::get_value();
    }

    template <typename T>
    consteval id_type type_id() noexcept
    {
        return hash_string(string_view(type_name<T>()));
    }

    namespace internal
    {
        template <typename T>
        struct get_first_parameter;

        template <template <typename, typename...> typename T, typename First, typename... Rest>
        struct get_first_parameter<T<First, Rest...>>
        {
            using type = First;
        };

        template <typename Newfirst, typename T>
        struct replace_first_parameter;

        template <typename Newfirst, template <typename, typename...> typename T, typename First, typename... Rest>
        struct replace_first_parameter<Newfirst, T<First, Rest...>>
        {
            using type = T<Newfirst, Rest...>;
        };

        template <typename T, typename Other, typename = void>
        struct get_rebind_alias
        {
            using type = typename replace_first_parameter<Other, T>::type;
        };

        template <typename T, typename Other>
        struct get_rebind_alias<T, Other, void_t<typename T::template rebind<Other>>>
        {
            using type = typename T::template rebind<Other>;
        };

        template <typename T, typename Elem>
        struct ptr_traits_base
        {
            using ptr_type = T;
            using element_type = Elem;
            using difference_type = typename get_first_parameter<T>::type;

            template <typename Other>
            using rebind = typename get_rebind_alias<T, Other>::type;

            [[nodiscard]] [[rythe_always_inline]] static constexpr ptr_type pointer_to(
                    conditional_t<is_void_v<Elem>, char, Elem>& val
                    )
                noexcept(noexcept(T::pointer_to(val))) /* strengthened */
            {                                          // Per LWG-3454
                return T::pointer_to(val);
            }
        };

        template <typename, typename = void, typename = void>
        struct ptr_traits_sfinae_layer {};

        template <typename T, typename Uty>
        struct ptr_traits_sfinae_layer<T, Uty, void_t<typename get_first_parameter<T>::type>>
                : ptr_traits_base<T, typename get_first_parameter<T>::type> {};

        template <typename T>
        struct ptr_traits_sfinae_layer<T, void_t<typename T::element_type>, void> : ptr_traits_base<T, typename T::element_type> {};
    } // namespace internal

    template <typename T>
    struct pointer_traits : internal::ptr_traits_sfinae_layer<T> {};

    template <typename T>
    struct pointer_traits<T*>
    {
        using ptr_type = T*;
        using element_type = T;
        using difference_type = ptrdiff_t;

        template <typename Other>
        using rebind = Other*;

        [[nodiscard]] [[rythe_always_inline]] static constexpr ptr_type pointer_to(conditional_t<is_void_v<T>, char, T>& val) noexcept
        {
            return addressof(val);
        }
    };
} // namespace rsl
