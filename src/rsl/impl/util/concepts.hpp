#pragma once
#include "../defines.hpp"

RYTHE_MSVC_SUPPRESS_WARNING_WITH_PUSH(5046)
#include <concepts>
RYTHE_MSVC_SUPPRESS_WARNING_POP

#include "common.hpp"

namespace rsl
{
    template <class T>
    concept enum_type = is_enum_v<T>;

    template <typename Func>
    concept invocable_undetermined = is_invocable_any_v<Func>;

    template <typename Func>
    concept function_ptr = is_function_ptr_v<Func>;

    template <typename Func>
    concept functor = is_functor_v<Func>;

    template <typename Func>
    concept member_function_ptr = std::is_member_function_pointer_v<Func>;

    template <typename Type>
    concept ratio_type = is_ratio_v<Type>;

    template <typename Type, template <typename...> typename Template>
    concept specialization_of = is_specialization_v<Type, Template>;

    template <bool Value>
    concept invert = !Value;

    template <typename T>
    concept integral_type = is_integral_v<remove_cvr_t<T>>;

    template <typename T>
    concept floating_point_type = is_floating_point_v<remove_cvr_t<T>>;

    template <typename T>
    concept arithmetic_type = is_arithmetic_v<remove_cvr_t<T>>;

    template <typename T>
    concept char_type = is_char_v<remove_cvr_t<T>>;

    template <typename T>
    concept unsigned_type = is_unsigned_v<remove_cvr_t<T>>;

    template <typename T>
    concept signed_type = is_signed_v<remove_cvr_t<T>>;

    template <typename T1, typename T2>
    concept same_as = is_same_v<T1, T2>;

    template <typename T1, typename T2>
    concept not_same_as = !is_same_v<T1, T2>;

    template <typename T1, typename T2>
    concept distinct_from = !same_as<remove_cvr_t<T1>, remove_cvr_t<T2>>;

    template <typename From, typename To>
    concept convertible_to = is_convertible_v<From, To> && requires { static_cast<To>(declval<From>()); };

    template <typename From, typename To>
    concept explicitly_convertible_to = requires(From val) { To(val); };

    template <typename T1, typename T2>
    concept common_reference_with =
            requires
                {
                    typename common_reference_t<T1, T2>;
                    typename common_reference_t<T2, T1>;
                } && same_as<common_reference_t<T1, T2>, common_reference_t<T2, T1>>
            && convertible_to<T1, common_reference_t<T1, T2>> && convertible_to<T2, common_reference_t<T1, T2>>;

    template <typename T1, typename T2>
    concept common_with =
            requires
                {
                    typename common_type_t<T1, T2>;
                    typename common_type_t<T2, T1>;
                }
            && same_as<common_type_t<T1, T2>, common_type_t<T2, T1>>
            && requires
                {
                    static_cast<common_type_t<T1, T2>>(declval<T1>());
                    static_cast<common_type_t<T1, T2>>(declval<T2>());
                }
            && common_reference_with<add_lval_ref_t<const T1>, add_lval_ref_t<const T2>>
            && common_reference_with<add_lval_ref_t<common_type_t<T1, T2>>,
                                     common_reference_t<add_lval_ref_t<const T1>, add_lval_ref_t<const T2>>>;

    template <typename T>
    concept nothrow_copy_constructible = is_nothrow_copy_constructible_v<T>;

    template <typename T>
    concept nothrow_move_constructible = is_nothrow_move_constructible_v<T>;

    template <typename Derived, typename Base>
    concept derived_from =
            ::rsl::is_base_of_v<Base, Derived> && ::rsl::is_convertible_v<const volatile Derived*, const volatile Base*>;

    template <typename LHS, typename RHS>
    concept assignable_from =
            ::rsl::is_lvalue_reference_v<LHS> &&
            common_reference_with<const ::rsl::remove_reference_t<LHS>&, const ::rsl::remove_reference_t<RHS>&> &&
            requires(LHS lhs, RHS&& rhs)
                {
                    { lhs = static_cast<RHS&&>(rhs) } -> same_as<LHS>;
                };

    template <typename T>
    concept destructible = ::std::destructible<T>; // Compiler magic behind the scenes.

    template <typename T, typename... Args>
    concept constructible_from = ::std::constructible_from<T, Args...>; // Compiler magic behind the scenes.

    template <typename T>
    concept default_initializable = constructible_from<T> && requires
        {
            T{};
            ::new(static_cast<void*>(nullptr)) T; // is-default-initializable<T>
        };

    template <typename T>
    concept move_constructible = constructible_from<T, T> && convertible_to<T, T>;

    template <typename T>
    concept copy_constructible = move_constructible<T> && constructible_from<T, T&> && convertible_to<T&, T> &&
            constructible_from<T, const T&> && convertible_to<const T&, T> &&
            constructible_from<T, const T> && convertible_to<const T, T>;

    template <typename T>
    concept constructible_at_all = is_constructible_any_v<T>;

    template <typename T>
    concept not_constructible = !is_constructible_any_v<T>;

    template <typename T>
    concept boolean_testable = convertible_to<T, bool> && requires(T&& val)
        {
            { !static_cast<T&&>(val) } -> convertible_to<bool>;
        };

    namespace internal
    {
        template <typename LHS, typename RHS>
        concept half_equality_comparable =
                requires(const remove_reference_t<LHS>& lhs, const remove_reference_t<RHS>& rhs)
                    {
                        { lhs == rhs } -> boolean_testable;
                        { lhs != rhs } -> boolean_testable;
                    };
    } // namespace internal

    template <typename LHS, typename RHS>
    concept weakly_equality_comparable_with =
            internal::half_equality_comparable<LHS, RHS> && internal::half_equality_comparable<RHS, LHS>;

    template <typename T>
    concept equality_comparable = internal::half_equality_comparable<T, T>;

    template <typename LHS, typename RHS>
    concept equality_comparable_with =
            equality_comparable<LHS> && equality_comparable<RHS> &&
            common_reference_with<const remove_reference_t<LHS>&, const remove_reference_t<RHS>&> &&
            equality_comparable<common_reference_t<const remove_reference_t<LHS>&, const remove_reference_t<RHS>&>> &&
            weakly_equality_comparable_with<LHS, RHS>;

    namespace internal
    {
        template <typename LHS, typename RHS>
        concept half_ordered = requires(const remove_reference_t<LHS>& lhs, const remove_reference_t<RHS>& rhs)
            {
                { lhs < rhs } -> boolean_testable;
                { lhs > rhs } -> boolean_testable;
                { lhs <= rhs } -> boolean_testable;
                { lhs >= rhs } -> boolean_testable;
            };

        template <typename LHS, typename RHS>
        concept partially_ordered_with = half_ordered<LHS, RHS> && half_ordered<RHS, LHS>;
    } // namespace internal

    template <typename T>
    concept totally_ordered = equality_comparable<T> && internal::half_ordered<T, T>;

    template <typename LHS, typename RHS>
    concept totally_ordered_with =
            totally_ordered<LHS> && totally_ordered<RHS> && equality_comparable_with<LHS, RHS> &&
            totally_ordered<common_reference_t<const remove_reference_t<LHS>&, const remove_reference_t<RHS>&>> &&
            internal::partially_ordered_with<LHS, RHS>;

    template <typename T>
    concept movable = is_object_v<T> && move_constructible<T> && assignable_from<T&, T> && ::std::swappable<T>;

    template <typename T>
    concept copyable = copy_constructible<T> && movable<T> && assignable_from<T&, T&> &&
            assignable_from<T&, const T&> && assignable_from<T&, const T>;

    template <typename T>
    concept semiregular = copyable<T> && default_initializable<T>;

    template <typename T>
    concept regular = semiregular<T> && equality_comparable<T>;

    namespace internal
    {
        template <typename T>
        struct func_traits
        {
            static_assert(
                    ::rsl::integral_constant<bool_t<T>, false>::value,
                    "Template param needs to be of function type."
                    );
        };

        template <typename Ret, typename... Args>
        struct func_traits<Ret(Args...)>
        {
            using return_type = Ret;
            using parameter_list = type_sequence<Args...>;

            template <typename Func>
            auto test_func(Func&&) -> decltype(rsl::declval<Func>()(rsl::declval<Args>()...)); // NOLINT

            template <typename Func>
            constexpr static bool is_compatible_with = requires(Func&& func, Args&&... args)
                {
                    { func(forward<Args>(args)...) } -> convertible_to<Ret>;
                };
        };
    } // namespace internal

    template <typename Func, typename FuncType>
    concept invocable = internal::func_traits<FuncType>::template is_compatible_with<Func>;

    template <typename T, typename... Args>
    concept constructable_type = is_constructible_v<T, Args...>;
} // namespace rsl
