#pragma once
#include "../defines.hpp"

RYTHE_MSVC_SUPPRESS_WARNING_WITH_PUSH(5046)
#include <bit>
#include <ratio>
RYTHE_MSVC_SUPPRESS_WARNING_POP

#include "primitives.hpp"

namespace rsl
{
    [[nodiscard]] constexpr bool is_constant_evaluated() noexcept
    {
        return std::is_constant_evaluated();
    }

    template <typename T>
    struct construct_type_signal_type {};

    template <typename T>
    constexpr construct_type_signal_type construct_type_signal = construct_type_signal_type<T>{};

    template <typename T>
    struct in_place_type_signal_type {};

    template <typename T>
    inline constexpr in_place_type_signal_type<T> in_place_type_signal = in_place_type_signal_type<T>{};

    struct in_place_signal_type {};

    constexpr in_place_signal_type in_place_signal = in_place_signal_type{};

    template <typename T, T Val>
    struct integral_constant
    {
        static constexpr T value = Val;

        using value_type = T;
        using type = integral_constant;

        constexpr operator value_type() const noexcept { return value; }

        [[nodiscard]] constexpr value_type operator()() const noexcept { return value; }
    };

    template <typename>
    inline constexpr bool always_false_v = false; // False value attached to a dependent name (for static_assert).

    template <bool Val>
    using bool_constant = integral_constant<bool, Val>;

    using true_type = bool_constant<true>;
    using false_type = bool_constant<false>;

    template <typename T>
    struct alignment_of : integral_constant<size_type, alignof(T)> {}; // determine alignment of T

    template <typename T>
    constexpr size_type alignment_of_v = alignof(T);

    template <typename T>
    inline constexpr bool is_empty_v = ::std::is_empty_v<T>; // Compiler magic behind the scenes.

    template <typename T>
    struct is_empty : bool_constant<is_empty_v<T>> {};

    template <typename>
    inline constexpr bool is_pointer_v = false;

    template <typename T>
    inline constexpr bool is_pointer_v<T*> = true;

    template <typename T>
    inline constexpr bool is_pointer_v<T* const> = true;

    template <typename T>
    inline constexpr bool is_pointer_v<T* volatile> = true;

    template <typename T>
    inline constexpr bool is_pointer_v<T* const volatile> = true;

    template <typename T>
    struct is_pointer : bool_constant<is_pointer_v<T>> {};

    template <typename, typename>
    inline constexpr bool is_same_v = false;

    template <typename T>
    inline constexpr bool is_same_v<T, T> = true;

    template <typename LHS, typename RHS>
    struct is_same : bool_constant<is_same_v<LHS, RHS>> {};

    template <typename T>
    constexpr bool is_enum_v = ::std::is_enum_v<T>; // Compiler magic behind the scenes.

    template <typename T>
    struct is_enum : bool_constant<is_enum_v<T>> {};

    template <typename EnumType>
        requires (is_enum_v<EnumType>)
    using underlying_type_t = __underlying_type(EnumType);

    template <typename EnumType>
        requires (is_enum_v<EnumType>)
    struct underlying_type
    {
        using type = underlying_type_t<EnumType>;
    };

    template <bool Test, typename TrueType, typename FalseType>
    struct conditional
    {
        using type = TrueType;
    };

    template <typename TrueType, typename FalseType>
    struct conditional<false, TrueType, FalseType>
    {
        using type = FalseType;
    };

    template <bool Test, typename TrueType, typename FalseType>
    using conditional_t = conditional<Test, TrueType, FalseType>::type;

    // Implementation details of conjunction<...> and disjunction<...>
    namespace internal
    {
        template <bool FirstValue, typename First, typename... Rest>
        struct conjunction_impl
        {
            // Handle false trait or last trait.
            using type = First;
        };

        template <typename True, typename Next, typename... Rest>
        struct conjunction_impl<true, True, Next, Rest...>
        {
            // The first trait is true, try the next one.
            using type = conjunction_impl<Next::value, Next, Rest...>::type;
        };

        template <bool FirstValue, typename First, typename... Rest>
        struct disjunction_impl
        {
            // Handle true trait or last trait.
            using type = First;
        };

        template <typename False, typename Next, typename... Rest>
        struct disjunction_impl<false, False, Next, Rest...>
        {
            // First trait is false, try the next trait.
            using type = disjunction_impl<Next::value, Next, Rest...>::type;
        };
    } // namespace internal

    template <typename... Traits>
    struct conjunction : true_type {}; // If Traits is empty, true_type.

    template <typename First, typename... Rest>
    struct conjunction<First, Rest...> : internal::conjunction_impl<First::value, First, Rest...>::type
    {
        // The first false trait in Traits, or the last trait if none are false.
    };

    template <typename... Traits>
    inline constexpr bool conjunction_v = conjunction<Traits...>::value;

    template <typename Trait>
    struct negation : bool_constant<!static_cast<bool>(Trait::value)> {}; // The negated result of Trait

    template <typename Trait>
    inline constexpr bool negation_v = negation<Trait>::value;

    template <typename... Traits>
    struct disjunction : false_type {}; // If Traits is empty, false_type.

    template <typename First, typename... Rest>
    struct disjunction<First, Rest...> : internal::disjunction_impl<First::value, First, Rest...>::type
    {
        // The first true trait in Traits, or the last trait if none are true.
    };

    template <typename... Traits>
    inline constexpr bool disjunction_v = disjunction<Traits...>::value;

    template <typename T, typename... Types>
    inline constexpr bool is_any_of_v = (is_same_v<T, Types> || ...);
    // True if and only if T is in Types. NOLINT(misc-redundant-expression)

    template <typename T>
    struct remove_reference
    {
        using type = T;
    };

    template <typename T>
    struct remove_reference<const T&>
    {
        using type = const T;
    };

    template <typename T>
    struct remove_reference<T&>
    {
        using type = T;
    };

    template <typename T>
    struct remove_reference<T&&>
    {
        using type = T;
    };

    template <typename T>
    using remove_reference_t = remove_reference<T>::type;

    template <typename T>
    struct remove_pointer
    {
        using type = T;
    };

    template <typename T>
    struct remove_pointer<const T*>
    {
        using type = const T;
    };

    template <typename T>
    struct remove_pointer<T*>
    {
        using type = T;
    };

    template <typename T>
    using remove_pointer_t = remove_pointer<T>::type;

    template <typename T>
    struct remove_const
    {
        using type = T;
    };

    template <typename T>
    struct remove_const<const T>
    {
        using type = T;
    };

    template <typename T>
    using remove_const_t = remove_const<T>::type;

    template <typename T>
    struct remove_cv
    {
        using type = T;
    };

    template <typename T>
    struct remove_cv<const T>
    {
        using type = T;
    };

    template <typename T>
    struct remove_cv<volatile T>
    {
        using type = T;
    };

    template <typename T>
    struct remove_cv<const volatile T>
    {
        using type = T;
    };

    template <typename T>
    using remove_cv_t = remove_cv<T>::type;

    template <typename T>
    using remove_cvr = remove_cv<remove_reference_t<T>>;

    template <typename T>
    using remove_cvr_t = remove_cvr<T>::type;

    template <typename T>
    inline constexpr bool is_void_v = is_same_v<remove_cv_t<T>, void>;

    template <typename T>
    struct is_void : bool_constant<is_void_v<T>> {};

    template <typename... Types>
    using void_t = void;

    template <typename... Types>
    using bool_t = bool;

    template <typename T>
    inline constexpr bool is_integral_v = is_any_of_v<
        remove_cv_t<T>, bool, char, signed char, unsigned char, wchar_t, char8_t, char16_t, char32_t, short,
        unsigned short, int, unsigned int, long, unsigned long, long long, unsigned long long>;

    template <typename T>
    struct is_integral : bool_constant<is_integral_v<T>> {};

    template <typename T>
    inline constexpr bool is_floating_point_v = is_any_of_v<remove_cv_t<T>, float, double, long double>;

    template <typename T>
    struct is_floating_point : bool_constant<is_floating_point_v<T>> {};

    template <typename T>
    inline constexpr bool is_arithmetic_v = // determine whether T is an arithmetic type
            is_integral_v<T> || is_floating_point_v<T>;

    template <typename T>
    struct is_arithmetic : bool_constant<is_arithmetic_v<T>> {};

    template <typename T>
    inline constexpr bool is_char_v = is_any_of_v<
        remove_cv_t<T>, char, signed char, unsigned char, wchar_t, char8_t, char16_t, char32_t>;

    template <typename T>
    struct is_char : bool_constant<is_char_v<T>> {};

    namespace internal
    {
        template <typename T, bool = is_integral_v<T>>
        struct sign_info
        {
            static constexpr bool is_signed = static_cast<remove_cv_t<T>>(-1) < static_cast<remove_cv_t<T>>(0);
            static constexpr bool is_unsigned = !is_signed;
        };

        template <typename T>
        struct sign_info<T, false>
        {
            static constexpr bool is_signed = is_floating_point_v<T>;
            static constexpr bool is_unsigned = false;
        };
    } // namespace internal

    template <typename T>
    inline constexpr bool is_unsigned_v = internal::sign_info<T>::is_unsigned;

    template <typename T>
    struct is_unsigned : bool_constant<is_unsigned_v<T>> {};

    template <typename T>
    inline constexpr bool is_signed_v = internal::sign_info<T>::is_signed;

    template <typename T>
    struct is_signed : bool_constant<is_signed_v<T>> {};

    template <typename T>
    constexpr bool is_nonbool_integral_v = is_integral_v<T> && !is_same_v<remove_cv_t<T>, bool>;

    template <typename T>
    struct is_nonbool_integral : bool_constant<is_nonbool_integral_v<T>> {};

    template <typename>
    inline constexpr bool is_const_v = false;

    template <typename T>
    inline constexpr bool is_const_v<const T> = true;

    template <typename T>
    struct is_const : bool_constant<is_const_v<T>> {};

    template <typename>
    inline constexpr bool is_lvalue_reference_v = false; // determine whether type argument is an lvalue reference

    template <typename T>
    inline constexpr bool is_lvalue_reference_v<T&> = true;

    template <typename T>
    struct is_lvalue_reference : bool_constant<is_lvalue_reference_v<T>> {};

    template <typename>
    inline constexpr bool is_rvalue_reference_v = false; // determine whether type argument is an rvalue reference

    template <typename T>
    inline constexpr bool is_rvalue_reference_v<T&&> = true;

    template <typename T>
    struct is_rvalue_reference : bool_constant<is_rvalue_reference_v<T>> {};

    template <typename>
    inline constexpr bool is_reference_v = false;

    template <typename T>
    inline constexpr bool is_reference_v<T&> = true;

    template <typename T>
    inline constexpr bool is_reference_v<T&&> = true;

    template <typename T>
    struct is_reference : bool_constant<is_reference_v<T>> {};

    // Implementation details of add_XXXX<...>
    namespace internal
    {
        // pointer type cannot be formed
        template <typename T, typename = void>
        struct add_pointer_impl
        {
            using type = T;
        };

        // pointer type can be formed
        template <typename T>
        struct add_pointer_impl<T, void_t<remove_reference_t<T>*>>
        {
            using type = remove_reference<T>::type*;
        };

        template <typename T>
        struct add_const_impl
        {
            using type = const T;
        };

        template <typename T>
        struct add_const_impl<T&>
        {
            using type = const T&;
        };

        template <typename T>
        struct add_const_impl<T&&>
        {
            using type = T&&;
        };

        template <typename T>
        struct add_const_impl<T*>
        {
            using type = const T*;
        };

        template <typename T>
        struct add_const_impl<const T>
        {
            using type = const T;
        };
    } // namespace internal

    template <typename T>
    struct add_pointer
    {
        using type = internal::add_pointer_impl<T>::type;
    };

    template <typename T>
    using add_pointer_t = internal::add_pointer_impl<T>::type;

    template <typename T>
    struct add_const
    {
        using type = internal::add_const_impl<T>::type;
    };

    template <typename T>
    using add_const_t = add_const<T>::type;

    template <typename T, typename = void>
    struct add_ref
    {
        using lvalue = T;
        using rvalue = T;
    };

    template <typename T>
    struct add_ref<T, void_t<T&>>
    {
        using lvalue = T&;
        using rvalue = T&&;
    };

    template <typename T>
    struct add_ref<const T, void_t<const T&>>
    {
        using lvalue = const T&;
        using rvalue = const T&&;
    };

    template <typename T>
    struct add_lval_ref
    {
        using type = add_ref<T>::lvalue;
    };

    template <typename T>
    using add_lval_ref_t = add_lval_ref<T>::type;

    template <typename T>
    struct add_rval_ref
    {
        using type = add_ref<T>::rvalue;
    };

    template <typename T>
    using add_rval_ref_t = add_rval_ref<T>::type;

    template <typename T>
    [[nodiscard]] [[rythe_always_inline]] constexpr T* addressof(T& val) noexcept
    {
        return __builtin_addressof(val); // Compiler magic.
    }

    template <typename T>
    const T* addressof(const T&&) = delete;

    template <typename T>
    [[nodiscard]] [[rythe_always_inline]] constexpr T&& forward(remove_reference_t<T>& val) noexcept
    {
        return static_cast<T&&>(val);
    }

    template <typename T>
    [[nodiscard]] [[rythe_always_inline]] constexpr T&& forward(remove_reference_t<T>&& val) noexcept // NOLINT(cppcoreguidelines*)
    {
        static_assert(!is_lvalue_reference_v<T>, "bad forward call");
        return static_cast<T&&>(val);
    }

    template <typename T>
    [[nodiscard]] [[rythe_always_inline]] constexpr remove_reference_t<T>&& move(T&& val) noexcept // NOLINT(cppcoreguidelines*)
    {
        return static_cast<remove_reference_t<T>&&>(val);
    }

    // Only function types and reference types can't be const qualified.
    template <typename T>
    inline constexpr bool is_function_v = !is_const_v<const T> && !is_reference_v<T>;

    template <typename T>
    struct is_function : bool_constant<is_function_v<T>> {};

    // Only function types and reference types can't be const qualified.
    template <typename T>
    inline constexpr bool is_object_v = is_const_v<const T> && !is_void_v<T>;

    template <typename T>
    struct is_object : bool_constant<is_object_v<T>> {};

    template <typename T, typename... Args>
    inline constexpr bool is_constructible_v = requires(Args... args)
        {
            { T(args...) };
        };

    template <typename T, typename... Args>
    struct is_constructible : bool_constant<is_constructible_v<T, Args...>> {};

    template <typename T>
    struct is_copy_constructible : is_constructible<T, typename add_lval_ref<typename add_const<T>::type>::type> {};

    template <typename T>
    inline constexpr bool is_copy_constructible_v = is_copy_constructible<T>::value;

    template <typename T>
    struct is_move_constructible : is_constructible<T, typename add_rval_ref<T>::type> {};

    template <typename T>
    inline constexpr bool is_move_constructible_v = is_move_constructible<T>::value;

    template <typename T, typename... Args>
    inline constexpr bool is_nothrow_constructible_v = requires(Args... args)
        {
            { T(args...) } noexcept;
        };

    template <typename T, typename... Args>
    struct is_nothrow_constructible : bool_constant<is_nothrow_constructible_v<T, Args...>> {};

    template <typename T>
    inline constexpr bool is_nothrow_copy_constructible_v = requires(const T& src)
        {
            { T(src) } noexcept;
        };

    template <typename T>
    struct is_nothrow_copy_constructible : bool_constant<is_nothrow_copy_constructible_v<T>> {};

    template <typename T>
    inline constexpr bool is_nothrow_move_constructible_v = requires(T&& src)
        {
            { T(move(src)) } noexcept;
        };

    template <typename T>
    struct is_nothrow_move_constructible : bool_constant<is_nothrow_move_constructible_v<T>> {};

    template <typename To, typename From>
    constexpr bool is_assignable_v = requires(add_lval_ref_t<To> to, add_lval_ref_t<From> from)
        {
            { to = from };
        };

    template <typename To, typename From>
    struct is_assignable : bool_constant<is_assignable_v<To, From>> {};

    template <typename T>
    constexpr bool is_copy_assignable_v = requires(add_lval_ref_t<T> dst, const T& src)
        {
            { dst = src };
        };

    template <typename T>
    struct is_copy_assignable : bool_constant<is_copy_assignable_v<T>> {};

    template <typename T>
    constexpr bool is_move_assignable_v = requires(add_lval_ref_t<T> dst, T&& src)
        {
            { dst = move(src) };
        };

    template <typename T>
    struct is_move_assignable : bool_constant<is_move_assignable_v<T>> {};

    template <typename To, typename From>
    constexpr bool is_nothrow_assignable_v = requires(add_lval_ref_t<To> to, add_lval_ref_t<From> from)
        {
            { to = from } noexcept;
        };

    template <typename To, typename From>
    struct is_nothrow_assignable : bool_constant<is_nothrow_assignable_v<To, From>> {};

    template <typename T>
    constexpr bool is_nothrow_copy_assignable_v = requires(add_lval_ref_t<T> dst, const T& src)
        {
            { dst = src } noexcept;
        };

    template <typename T>
    struct is_nothrow_copy_assignable : bool_constant<is_nothrow_copy_assignable_v<T>> {};

    template <typename T>
    constexpr bool is_nothrow_move_assignable_v = requires(add_lval_ref_t<T> dst, T&& src)
        {
            { dst = move(src) } noexcept;
        };

    template <typename T>
    struct is_nothrow_move_assignable : bool_constant<is_nothrow_move_assignable_v<T>> {};

    template <typename T, typename... Args>
    inline constexpr bool is_trivially_constructible_v =
            ::std::is_trivially_constructible_v<T, Args...>; // Uses compiler magic behind the scenes.

    template <typename T>
    struct is_trivially_constructible : bool_constant<is_trivially_constructible_v<T>> {};

    template <typename T>
    inline constexpr bool is_trivially_default_constructible_v =
            ::std::is_trivially_default_constructible_v<T>; // Uses compiler magic behind the scenes.

    template <typename T>
    struct is_trivially_default_constructible : bool_constant<is_trivially_default_constructible_v<T>> {};

    template <typename T>
    inline constexpr bool is_trivially_destructible_v =
            ::std::is_trivially_destructible_v<T>; // Uses compiler magic behind the scenes.

    template <typename T>
    struct is_trivially_destructible : bool_constant<is_trivially_destructible_v<T>> {};

    template <typename T>
    inline constexpr bool is_trivially_copy_constructible_v =
            ::std::is_trivially_copy_constructible_v<T>; // Uses compiler magic behind the scenes.

    template <typename T>
    struct is_trivially_copy_constructible : bool_constant<is_trivially_copy_constructible_v<T>> {};

    template <typename T>
    constexpr bool is_trivial_v = __is_trivial(T); // Compiler magic.

    template <typename T>
    struct is_trivial : bool_constant<is_trivial_v<T>> {};

    template <typename T>
    constexpr bool is_trivially_copyable_v = __is_trivially_copyable(T); // Compiler magic.

    template <typename T>
    struct is_trivially_copyable : bool_constant<is_trivially_copyable_v<T>> {};

    namespace internal
    {
        // Requires that the size of To is equal or larger to the size of From
        // Might still break on some compilers if From is byte and To is unsigned long long
        template <typename To, typename From>
            requires is_trivially_copyable_v<To> && is_trivially_constructible_v<To> && is_trivially_copyable_v<From> &&
            (sizeof(To) >= sizeof(From))
        constexpr void constexpr_memcpy_impl(To* dst, const From* src, const size_type count)
        {
            const size_type itemCount = count / sizeof(To);
            constexpr size_type stepSize = sizeof(To) / sizeof(From);

            struct converter
            {
                From from[stepSize];
            };

            for (size_type i = 0; i < itemCount; i++)
            {
                converter conv{};
                for (size_type j = 0; j < stepSize; j++)
                {
                    conv.from[j] = src[i * stepSize + j];
                }

                dst[i] = ::std::bit_cast<To>(conv);
            }
        }

        // Has no size requirements, but not every compiler is happy with this implementation.
        // Causes an internal compiler on MSVC
        // TODO(Glyn): Needs more research
        template <typename To, typename From>
            requires is_trivially_copyable_v<To> && is_trivially_constructible_v<To> && is_trivially_copyable_v<From>
        constexpr void constexpr_memcpy_impl_unrestricted(To* dst, const From* src, const size_type count)
        {
            size_type srcCount = count / sizeof(From);
            if (srcCount * sizeof(From) != count)
            {
                ++srcCount;
            }

            struct src_converter
            {
                byte from[sizeof(From)];
            };

            src_converter* srcData = new src_converter[srcCount];

            for (size_type i = 0; i < srcCount; ++i)
            {
                srcData[i] = ::std::bit_cast<src_converter>(src[i]);
            }

            size_type dstCount = count / sizeof(From);
            if (dstCount * sizeof(From) != count)
            {
                ++dstCount;
            }

            struct dst_converter
            {
                byte to[sizeof(To)];
            };

            dst_converter* dstData = new dst_converter[dstCount];

            for (size_type i = 0; i < count; ++i)
            {
                size_type srcIdx = i / sizeof(From);
                size_type dstIdx = i / sizeof(To);
                size_type srcByte = i % sizeof(From);
                size_type dstByte = i % sizeof(To);

                dstData[dstIdx].to[dstByte] = srcData[srcIdx].from[srcByte];
            }

            for (size_type i = 0; i < dstCount; ++i)
            {
                // This line is the one that causes troubles.
                // on MSVC bit_cast from struct{From[]} is fine, but from struct{byte[]} is not...
                dst[i] = ::std::bit_cast<To>(dstData[i]);
            }

            delete[] srcData;
            delete[] dstData;
        }

        // Might still break on some compilers if T is unsigned long long
        template <typename T>
            requires is_trivially_constructible_v<T>
        constexpr void constexpr_memset_impl(T* dst, const byte val, const size_type count)
        {
            const size_type itemCount = count / sizeof(T);
            constexpr size_type stepSize = sizeof(T);

            struct converter
            {
                byte from[stepSize];
            };

            for (size_type i = 0; i < itemCount; i++)
            {
                converter conv{};
                for (size_type j = 0; j < stepSize; j++)
                {
                    conv.from[j] = val;
                }

                dst[i] = ::std::bit_cast<T>(conv);
            }
        }
    } // namespace internal

    template <typename To, typename From>
    constexpr void* constexpr_memcpy(To* dst, const From* src, const size_type count) noexcept;

    // Assumes buffer size of src is larger or equal to the size of "To"
    template <typename To, typename From>
        requires is_trivially_copyable_v<To> && is_trivially_copyable_v<From> && is_trivially_constructible_v<To>
    [[nodiscard]] constexpr To unaligned_load(const From* src) noexcept
    {
        To dst{};
        constexpr_memcpy(&dst, src, sizeof(To));
        return dst;
    }

    // Requires the size of "To" to be equal to the size of "From"
    template <typename To, typename From>
        requires is_trivially_copyable_v<To> && is_trivially_copyable_v<From> && (sizeof(To) == sizeof(From))
    [[nodiscard]] constexpr To bit_cast(const From& value) noexcept
    {
        if (is_constant_evaluated())
        {
            To dst{};
            constexpr_memcpy(&dst, &value, sizeof(From));
            return dst;
        }
        else
        {
            return __builtin_bit_cast(To, value);
        }
    }

    template <typename To, typename From>
        requires is_trivially_copyable_v<To> && is_trivially_copyable_v<From> && (sizeof(To) >= sizeof(From))
    [[nodiscard]] constexpr To insert_cast(const From& value) noexcept
    {
        To dst{};
        constexpr_memcpy(&dst, &value, sizeof(From));
        return dst;
    }

    template <typename To, typename From>
    constexpr void* constexpr_memcpy(To* dst, const From* src, const size_type count) noexcept
    {
        if (is_constant_evaluated())
        {
            if constexpr (is_void_v<To>)
            {
                return constexpr_memcpy(bit_cast<byte*>(dst), src, count);
            }
            else if constexpr (is_void_v<From>)
            {
                return constexpr_memcpy(dst, bit_cast<const byte*>(src), count);
            }
            else
            {
                if constexpr (sizeof(To) >= sizeof(From))
                {
                    internal::constexpr_memcpy_impl(dst, src, count);
                }
                else
                {
                    internal::constexpr_memcpy_impl_unrestricted(dst, src, count);
                }

                return dst;
            }
        }
        else
        {
            return memcpy(dst, src, count);
        }
    }

    template <typename T>
    constexpr void* constexpr_memset(T* dst, const byte value, const size_type count)
    {
        if (is_constant_evaluated())
        {
            if constexpr (is_void_v<T>)
            {
                return constexpr_memset(bit_cast<byte*>(dst), value, count);
            }
            else
            {
                internal::constexpr_memset_impl(dst, value, count);
                return dst;
            }
        }
        else
        {
            return memset(dst, value, count);
        }
    }

    template <typename T>
    [[nodiscard]] [[rythe_always_inline]] constexpr T* to_address(T* val) noexcept
    {
        static_assert(!is_function_v<T>);
        return val;
    }

    template <typename T>
        requires(!is_pointer_v<T>)
    [[nodiscard]] [[rythe_always_inline]] constexpr auto to_address(const T& val) noexcept
    {
        return to_address(val.operator->());
    }

    template <typename>
    inline constexpr bool is_array_v = false;

    template <typename T, size_type I>
    inline constexpr bool is_array_v<T[I]> = true;

    template <typename T>
    inline constexpr bool is_array_v<T[]> = true;

    template <typename T>
    struct is_array : bool_constant<is_array_v<T>> {};

    template <typename T>
    struct array_value;

    template <typename T, size_type I>
    struct array_value<T[I]>
    {
        using type = T;
    };

    template <typename T>
    struct array_value<T[]>
    {
        using type = T;
    };

    template <typename T>
    using array_value_t = typename array_value<T>::type;

    template <typename T, unsigned int I = 0>
    inline constexpr size_type extent_v = 0; // Determine extent of dimension I of array T.

    template <typename T, size_type N>
    inline constexpr size_type extent_v<T[N], 0> = N;

    template <typename T, unsigned int I, size_type N>
    inline constexpr size_type extent_v<T[N], I> = extent_v<T, I - 1>;

    template <typename T, unsigned int I>
    inline constexpr size_type extent_v<T[], I> = extent_v<T, I - 1>;

    template <typename T, unsigned int I = 0>
    struct extent : integral_constant<size_type, extent_v<T, I>> {};

    // remove array extent
    template <typename T>
    struct remove_extent
    {
        using type = T;
    };

    template <typename T, size_type I>
    struct remove_extent<T[I]>
    {
        using type = T;
    };

    template <typename T>
    struct remove_extent<T[]>
    {
        using type = T;
    };

    template <typename T>
    using remove_extent_t = remove_extent<T>::type;

    // Implementation details of decay_t<...>
    namespace internal
    {
        template <typename T>
        struct decay_impl
        {
            using T1 = remove_reference_t<T>;
            using T2 = conditional_t<is_function_v<T1>, add_pointer<T1>, remove_cv<T1>>;
            using type = conditional_t<is_array_v<T1>, add_pointer<remove_extent_t<T1>>, T2>::type;
        };
    } // namespace internal


    template <typename T>
    T returns_exactly() noexcept; // not defined NOLINT

    template <typename T>
    [[nodiscard]] T fake_copy_init(T) noexcept; // NOLINT
    // fake_copy_init<T>(E):
    // (1) has type T [decay_t<decltype((E))> if T is deduced],
    // (2) is well-formed if and only if E is implicitly convertible to T and T is destructible, and
    // (3) is non-throwing if and only if both conversion from decltype((E)) to T and destruction of T are non-throwing.

    template <typename T>
    add_rval_ref_t<T> declval() noexcept // NOLINT
    {
        static_assert(always_false_v<T>, "Calling declval is ill-formed.");
    }

    template <typename T>
    struct decay
    {
        using type = internal::decay_impl<T>::type;
    };

    template <typename T>
    using decay_t = internal::decay_impl<T>::type;

    template <typename T, typename... Types>
    concept in_place_constructable =
            requires(T* location, Types&&... args) { ::new(static_cast<void*>(location)) T(forward<Types>(args)...); };

    template <typename T, typename... Types>
    constexpr bool is_nothrow_in_place_constructable_v =
            noexcept(::new(declval<void*>()) T(forward<Types>(declval<Types>())...));

    template <typename T, typename... Types>
    struct is_nothrow_in_place_constructable
    {
        constexpr static bool value = is_nothrow_in_place_constructable_v<T, Types...>;
    };

    template <typename T, typename... Types>
        requires in_place_constructable<T, Types...>
    constexpr T* construct_at(
            T* const location,
            Types&&... args
            )
        noexcept(is_nothrow_in_place_constructable_v<T, Types...>)
    {
        return ::new(static_cast<void*>(location)) T(forward<Types>(args)...);
    }

    template <typename T>
    using is_union = ::std::is_union<T>; // Compiler magic behind the scenes.

    template <typename T>
    inline constexpr bool is_union_v = is_union<T>::value;

    // Implementation details of is_class<...>
    namespace internal
    {
        // Test if member object pointers of this type are valid.
        template <typename T>
        bool_constant<!is_union<T>::value> test_is_class(int T::*); // NOLINT

        template <typename>
        false_type test_is_class(...); // NOLINT
    }                                  // namespace internal

    template <typename T>
    struct is_class : decltype(internal::test_is_class<T>(nullptr)) {};

    template <typename T>
    inline constexpr bool is_class_v = is_class<T>::value;

    // Implementation details of is_base_of<...>
    namespace internal
    {
        template <typename Base>
        true_type test_ptr_conv(const volatile Base*); // NOLINT
        template <typename>
        false_type test_ptr_conv(const volatile void*); // NOLINT

        template <typename Base, typename Derived>
        auto test_is_base_of(int) -> decltype(test_ptr_conv<Base>(static_cast<Derived*>(nullptr))); // NOLINT
        template <typename, typename>
        auto test_is_base_of(...) -> true_type; // Private or ambiguous base. NOLINT
    }                                           // namespace internal

    template <typename Base, typename Derived>
    struct is_base_of
            : bool_constant<
                is_class_v<Base> && is_class_v<Derived> && decltype(internal::test_is_base_of<Base, Derived>(0))::value> {};

    template <typename Base, typename Derived>
    inline constexpr bool is_base_of_v = is_base_of<Base, Derived>::value;

    // Implementation details of is_convertible<...>
    namespace internal
    {
        template <typename T>
        auto test_returnable(int) -> decltype(void(static_cast<T (*)()>(nullptr)), true_type{}); // NOLINT
        template <typename>
        auto test_returnable(...) -> false_type; // NOLINT

        template <typename From, typename To>
        auto test_implicitly_convertible( /* NOLINT */
                int
                )
            -> decltype((void)(::rsl::declval<void (&)(To)>()(::rsl::declval<From>())), true_type{});
        template <typename, typename>
        auto test_implicitly_convertible(...) -> false_type; // NOLINT
    }                                                        // namespace internal

    template <typename From, typename To>
    struct is_convertible
            : bool_constant<
                (decltype(internal::test_returnable<To>(0))::value &&
                    decltype(internal::test_implicitly_convertible<From, To>(0))::value) ||
                (is_void_v<From> && is_void_v<To>)> {};

    template <typename From, typename To>
    inline constexpr bool is_convertible_v = is_convertible<From, To>::value;

    // Implementation details of is_nothrow_convertible<...>
    namespace internal
    {
        template <typename From, typename To, bool = is_convertible_v<From, To>, bool = is_void_v<To>>
        constexpr bool is_nothrow_convertible_impl_v = noexcept(fake_copy_init<To>(declval<From>()));

        template <typename From, typename To, bool IsVoid>
        constexpr bool is_nothrow_convertible_impl_v<From, To, false, IsVoid> = false;

        template <typename From, typename To>
        constexpr bool is_nothrow_convertible_impl_v<From, To, true, true> = true;
    }

    template <typename From, typename To>
    constexpr bool is_nothrow_convertible_v = internal::is_nothrow_convertible_impl_v<From, To>;

    template <typename From, typename To>
    struct is_nothrow_convertible : bool_constant<internal::is_nothrow_convertible_impl_v<From, To>> {};

    template <typename... T>
    struct common_type;

    template <typename... T>
    using common_type_t = common_type<T...>::type;

    template <>
    struct common_type<> {};

    template <typename T1>
    struct common_type<T1> : common_type<T1, T1> {};

    // Implementation details of common_type<...>
    namespace internal
    {
        // Type of the outcome of a ternary statement if a ternary statement is valid.
        template <typename T1, typename T2>
        using ternary_result_test = decltype(false ? ::rsl::declval<T1>() : ::rsl::declval<T2>());

        template <typename T1, typename T2, typename = void>
        struct ternary_result_type
        {
            // No common type can be found, ternary statement isn't valid.
        };

        template <typename T1, typename T2>
        struct ternary_result_type<T1, T2, void_t<ternary_result_test<const T1&, const T2&>>>
        {
            // Common type if valid.
            using type = remove_cvr_t<ternary_result_test<const T1&, const T2&>>;
        };

        template <typename T1, typename T2, typename = void>
        struct decayed_ternary_result_type : ternary_result_type<T1, T2> {};

        template <typename T1, typename T2>
        struct decayed_ternary_result_type<T1, T2, void_t<ternary_result_test<T1, T2>>>
        {
            // Decayed common type.
            using type = decay_t<ternary_result_test<T1, T2>>;
        };

        template <typename T1, typename T2, typename Decayed1 = decay_t<T1>, typename Decayed2 = decay_t<T2>>
        struct common_type_2_types : common_type<Decayed1, Decayed2> {};

        template <typename T1, typename T2>
        struct common_type_2_types<T1, T2, T1, T2> : decayed_ternary_result_type<T1, T2> {};

        template <typename Void, typename T1, typename T2, typename... Rest>
        struct common_type_multi {};

        template <typename T1, typename T2, typename... Rest>
        struct common_type_multi<void_t<common_type_t<T1, T2>>, T1, T2, Rest...> : common_type<common_type_t<T1, T2>, Rest...> {};
    } // namespace internal

    template <typename T1, typename T2>
    struct common_type<T1, T2> : internal::common_type_2_types<T1, T2> {};

    template <typename T1, typename T2, typename... Rest>
    struct common_type<T1, T2, Rest...> : internal::common_type_multi<void, T1, T2, Rest...> {};

    // Implementation details of copy_qualifiers<...>
    namespace internal
    {
        template <typename From>
        struct copy_qualifiers_impl
        {
            template <typename To>
            using result = To;
        };

        template <typename From>
        struct copy_qualifiers_impl<const From>
        {
            template <typename To>
            using result = const To;
        };

        template <typename From>
        struct copy_qualifiers_impl<volatile From>
        {
            template <typename To>
            using result = volatile To;
        };

        template <typename From>
        struct copy_qualifiers_impl<const volatile From>
        {
            template <typename To>
            using result = const volatile To;
        };
    } // namespace internal

    template <typename From, typename To>
    using copy_qualifiers_t = internal::copy_qualifiers_impl<From>::template result<To>;

    template <typename From, typename To>
    struct copy_qualifiers
    {
        using type = internal::copy_qualifiers_impl<From>::template result<To>;
    };

    template <typename, typename, template <typename> typename, template <typename> typename>
    struct basic_common_reference {};

    template <typename...>
    struct common_reference;

    template <typename... Types>
    using common_reference_t = common_reference<Types...>::type;

    template <>
    struct common_reference<> {};

    template <typename T>
    struct common_reference<T>
    {
        using type = T;
    };

    template <typename T>
    concept lvalue_reference_type = is_lvalue_reference_v<T>;

    namespace internal
    {
        template <typename T1>
        struct add_qualifiers
        {
            template <typename T2>
            using apply = copy_qualifiers_t<T1, T2>;
        };

        template <typename T1>
        struct add_qualifiers<T1&>
        {
            template <typename T2>
            using apply = add_lval_ref_t<copy_qualifiers_t<T1, T2>>;
        };

        template <typename T1>
        struct add_qualifiers<T1&&>
        {
            template <typename T2>
            using apply = add_rval_ref_t<copy_qualifiers_t<T1, T2>>;
        };

        template <typename T1, typename T2>
        using cond_res = decltype(false ? ::rsl::returns_exactly<T1>() : ::rsl::returns_exactly<T2>());

        template <typename T1, typename T2, typename = void>
        struct common_reference2_c : common_type<T1, T2> {};

        template <typename T1, typename T2>
        struct common_reference2_c<T1, T2, void_t<cond_res<T1, T2>>>
        {
            using type = cond_res<T1, T2>;
        };

        template <typename T1, typename T2>
        using basic_specialization = basic_common_reference<
            remove_cvr_t<T1>, remove_cvr_t<T2>, add_qualifiers<T1>::template apply,
            add_qualifiers<T2>::template apply>::type;

        template <typename T1, typename T2, typename = void>
        struct common_reference2_b : common_reference2_c<T1, T2> {};

        template <typename T1, typename T2>
        struct common_reference2_b<T1, T2, void_t<basic_specialization<T1, T2>>>
        {
            using type = basic_specialization<T1, T2>;
        };

        template <typename T1, typename T2>
        struct common_reference2_a : common_reference2_b<T1, T2> {};

        template <typename T1, typename T2>
        concept lvalue_common_reference_valid =
                lvalue_reference_type<cond_res<copy_qualifiers_t<T1, T2>&, copy_qualifiers_t<T1, T2>&>>;

        template <typename T1, typename T2>
            requires lvalue_common_reference_valid<T1, T2>
        using lvalue_common_ref = cond_res<copy_qualifiers_t<T1, T2>&, copy_qualifiers_t<T2, T1>&>;

        template <typename T1, typename T2>
        struct common_reference2_ax {};

        template <typename T1, typename T2>
            requires lvalue_common_reference_valid<T1, T2>
        struct common_reference2_ax<T1&, T2&>
        {
            using type = lvalue_common_ref<T1, T2>;
        };

        template <typename T1, typename T2>
        concept rvalue_ref_convertible = is_convertible_v<T1&&, lvalue_common_ref<const T1, T2>>;

        template <typename T1, typename T2>
            requires rvalue_ref_convertible<T1, T2>
        struct common_reference2_ax<T1&&, T2&>
        {
            using type = lvalue_common_ref<const T1, T2>;
        };

        template <typename T1, typename T2>
            requires rvalue_ref_convertible<T2, T1>
        struct common_reference2_ax<T1&, T2&&>
        {
            using type = lvalue_common_ref<const T2, T1>;
        };

        template <typename T1, typename T2>
        using rvalue_common_ref = remove_reference_t<lvalue_common_ref<T1, T2>>&&;

        template <typename T1, typename T2>
            requires is_convertible_v<T1&&, rvalue_common_ref<T1, T2>> &&
            is_convertible_v<T1&&, rvalue_common_ref<T1, T2>>
        struct common_reference2_ax<T1&&, T2&&>
        {
            using type = rvalue_common_ref<T1, T2>;
        };

        template <typename T1, typename T2>
        using common_ref_2_ax_t = common_reference2_ax<T1, T2>::type;

        template <typename T1, typename T2>
            requires is_convertible_v<add_pointer_t<T1>, add_pointer_t<common_ref_2_ax_t<T1, T2>>> &&
            is_convertible_v<add_pointer_t<T2>, add_pointer_t<common_ref_2_ax_t<T1, T2>>>
        struct common_reference2_a<T1, T2>
        {
            using type = common_ref_2_ax_t<T1, T2>;
        };

        template <typename Void, typename T1, typename T2, typename... Types>
        struct fold_common_reference {};

        template <typename T1, typename T2, typename... Types>
        struct fold_common_reference<void_t<common_reference_t<T1, T2>>, T1, T2, Types...>
                : common_reference<common_reference_t<T1, T2>, Types...> {};
    } // namespace internal

    template <typename T1, typename T2>
    struct common_reference<T1, T2> : internal::common_reference2_a<T1, T2> {};

    template <typename T1, typename T2, typename T3, typename... Rest>
    struct common_reference<T1, T2, T3, Rest...> : internal::fold_common_reference<void, T1, T2, T3, Rest...> {};

    template <typename Type, template <typename...> typename Template>
    constexpr bool is_specialization_v = false; // true if and only if Type is a specialization of Template

    template <template <typename...> typename Template, typename... Types>
    constexpr bool is_specialization_v<Template<Types...>, Template> = true;

    template <typename Type, template <typename...> typename Template>
    struct is_specialization : bool_constant<is_specialization_v<Type, Template>> {};

    template <typename T>
    [[nodiscard]] [[rythe_always_inline]] constexpr conditional_t<
        !is_nothrow_move_constructible_v<T> && is_copy_constructible_v<T>, const T&, T&&> move_if_noexcept(T& val) noexcept
    {
        return ::rsl::move(val);
    }

    template <typename T>
    concept swappable = is_move_constructible_v<T> && is_move_assignable_v<T>;

    template <typename T>
        requires swappable<T>
    [[rythe_always_inline]] constexpr void swap(
            T& lhs,
            T& rhs
            )
        noexcept(is_nothrow_move_constructible_v<T> && is_nothrow_move_assignable_v<T>)
    {
        T tmp = move(lhs);
        lhs = move(rhs);
        rhs = move(tmp);
    }

    template <typename T>
    [[nodiscard]] [[rythe_always_inline]] constexpr add_const_t<T>& as_const(T& val) noexcept
    {
        return val;
    }

    template <typename T>
    void as_const(const T&&) = delete;

    template <typename T>
    [[nodiscard]] [[rythe_always_inline]] constexpr const T* as_const_ptr(T* ptr) noexcept
    {
        return ptr;
    }

    template <typename T>
    [[nodiscard]] [[rythe_always_inline]] constexpr const T* as_const_ptr(const T* ptr) noexcept
    {
        return ptr;
    }

    template <template <typename...> typename T, typename U, size_type I, typename... Args>
    struct make_sequence : make_sequence<T, U, I - 1, Args..., U> {};

    template <template <typename...> typename T, typename U, typename... Args>
    struct make_sequence<T, U, 0, Args...>
    {
        using type = T<Args...>;
    };

    template <template <typename...> typename T, typename U, size_type I, typename... Args>
    using make_sequence_t = make_sequence<T, U, I, Args...>::type;

    template <rsl::size_type I, typename Type, typename... Types>
    struct element_at : element_at<I - 1, Types...> {};

    template <typename Type, typename... Types>
    struct element_at<0, Type, Types...>
    {
        using type = Type;
    };

    template <rsl::size_type I, typename... Types>
    using element_at_t = element_at<I, Types...>::type;

    namespace internal
    {
        template <size_type I, typename T, typename Type, typename... Types>
        struct index_of_element_impl
                : conditional_t<
                    is_same_v<T, Type>, integral_constant<size_type, I>, index_of_element_impl<I + 1, T, Types...>> {};
    } // namespace internal

    // Gets index of first occurrence of T in type list.
    template <typename T, typename Type, typename... Types>
    struct index_of_element : internal::index_of_element_impl<0, T, Type, Types...> {};

    // Gets index of first occurrence of T in type list.
    template <typename T, typename Type, typename... Types>
    constexpr size_type index_of_element_v = index_of_element<T, Type, Types...>::value;

    template <typename... Types>
    struct type_sequence
    {
        using tuple_type = ::std::tuple<Types...>; // TODO: Make our own tuple type.
        constexpr static size_type size = sizeof...(Types);

        template <typename T>
        constexpr static bool contains = disjunction<is_same<T, Types>...>::value;

        template <typename T>
        constexpr static size_type index_of_type = index_of_element_v<T, Types...>;

        template <size_type I>
        using type_at = element_at_t<I, Types...>;
    };

    template <typename T>
    concept type_sequence_c = is_specialization_v<T, type_sequence>;

    template <typename...>
    struct concat_sequence;

    template <typename... A, typename... B, type_sequence_c C, type_sequence_c... Rest>
    struct concat_sequence<type_sequence<A...>, type_sequence<B...>, C, Rest...>
    {
        using type = concat_sequence<type_sequence<A..., B...>, C, Rest...>::type;
    };

    template <typename... A, typename... B>
    struct concat_sequence<type_sequence<A...>, type_sequence<B...>>
    {
        using type = type_sequence<A..., B...>;
    };

    template <type_sequence_c... Sequences>
    using concat_sequence_t = concat_sequence<Sequences...>::type;

    template <type_sequence_c Sequence, typename T>
    constexpr bool type_sequence_contains_v = Sequence::template contains<T>;

    template <type_sequence_c Sequence, typename T>
    struct type_sequence_contains;

    template <typename... Types, typename T>
    struct type_sequence_contains<type_sequence<Types...>, T> : bool_constant<type_sequence_contains_v<type_sequence<Types...>, T>> {};

    template <type_sequence_c SequenceA, type_sequence_c SequenceB>
    struct type_sequence_conjunction;

    template <typename... TypesA, typename... TypesB>
    struct type_sequence_conjunction<type_sequence<TypesA...>, type_sequence<TypesB...>>
            : conjunction<
                conjunction<type_sequence_contains<type_sequence<TypesA...>, TypesB>...>,
                conjunction<type_sequence_contains<type_sequence<TypesB...>, TypesA>...>> {};

    template <typename T, T... Vals>
    struct integer_sequence
    {
        // sequence of integer parameters
        static_assert(is_integral_v<T>, "integer_sequence<T, I...> requires T to be an integral type.");

        using value_type = T;

        constexpr static size_type size = sizeof...(Vals);

        template <T Val>
        constexpr static bool contains = ((Val == Vals) || ...);
    };

    namespace internal
    {
        template <class T, T I, T N, T... Integers>
        struct make_integer_seq_impl
        {
            using type = make_integer_seq_impl<T, I + 1, N, Integers..., I>::type;
        };

        template <class T, T N, T... Integers>
        struct make_integer_seq_impl<T, N, N, Integers...>
        {
            using type = integer_sequence<T, Integers...>;
        };
    } // namespace internal

    template <typename T, T Size>
    using make_integer_sequence = internal::make_integer_seq_impl<T, 0, Size>::type;

    template <size_type... Vals>
    using index_sequence = integer_sequence<size_type, Vals...>;

    template <size_type Size>
    using make_index_sequence = make_integer_sequence<size_type, Size>;

    template <typename... Types>
    using index_sequence_for = make_index_sequence<sizeof...(Types)>;

    namespace internal
    {
        template <typename From, typename To, typename = void>
        struct invoke_convertible : false_type {};

        template <typename From, typename To>
        struct invoke_convertible<From, To, void_t<decltype(fake_copy_init<To>(returns_exactly<From>()))>> : true_type {};

        template <typename From, typename To>
        struct invoke_nothrow_convertible : bool_constant<noexcept(fake_copy_init<To>(returns_exactly<From>()))> {};

        template <typename ReturnType, bool Nothrow>
        struct invoke_traits
        {
            using type = ReturnType;
            using is_invocable = true_type;
            using is_nothrow_invocable = bool_constant<Nothrow>;

            template <typename RetType>
            using is_invocable_ret = bool_constant<disjunction_v<is_void<RetType>, invoke_convertible<type, RetType>>>;

            template <typename RetType>
            using is_nothrow_invocable_ret = bool_constant<conjunction_v<
                is_nothrow_invocable,
                disjunction<
                    is_void<RetType>,
                    conjunction<invoke_convertible<type, RetType>, invoke_nothrow_convertible<type, RetType>>>>>;
        };

        template <typename Void, typename Callable>
        struct invoke_traits_zero
        {
            // Selected when Callable isn't callable with zero Args.
            using is_invocable = false_type;
            using is_nothrow_invocable = false_type;

            template <typename RetType>
            using is_invocable_ret = false_type;

            template <typename RetType>
            using is_nothrow_invocable_ret = false_type;
        };

        template <typename Callable>
        using decltype_invoke_zero = decltype(declval<Callable>()());

        template <typename Callable>
        struct invoke_traits_zero<void_t<decltype_invoke_zero<Callable>>, Callable>
                : invoke_traits<decltype_invoke_zero<Callable>, noexcept(declval<Callable>()())> {};

        template <typename Void, typename... Types>
        struct invoke_traits_nonzero
        {
            // Selected when Callable isn't callable with nonzero Args.
            using is_invocable = false_type;
            using is_nothrow_invocable = false_type;

            template <typename RetType>
            using is_invocable_ret = false_type;

            template <typename RetType>
            using is_nothrow_invocable_ret = false_type;
        };

        template <typename Callable, typename T1, typename... Types2>
        using decltype_invoke_nonzero =
        decltype(::std::invoke(declval<Callable>(), declval<T1>(), declval<Types2>()...));

        template <typename Callable, typename T1, typename... Types2>
        struct invoke_traits_nonzero<
                    void_t<decltype_invoke_nonzero<Callable, T1, Types2...>>, Callable, T1, Types2...>
                : invoke_traits<
                    decltype_invoke_nonzero<Callable, T1, Types2...>,
                    noexcept(::std::invoke(declval<Callable>(), declval<T1>(), declval<Types2>()...))> {};

        template <typename Callable, typename... Args>
        using select_invoke_traits = conditional_t<
            sizeof...(Args) == 0, invoke_traits_zero<void, Callable>, invoke_traits_nonzero<void, Callable, Args...>>;

        template <typename ReturnType, typename Callable, typename... Args>
        using is_invocable_ret_impl =
        select_invoke_traits<Callable, Args...>::template is_invocable_ret<ReturnType>;
    } // namespace internal

    template <typename Callable, typename... Args>
    using invoke_result_t = internal::select_invoke_traits<Callable, Args...>::type;

    template <typename Callable, typename... Args>
    struct invoke_result
    {
        using type = invoke_result_t<Callable, Args...>;
    };

    template <typename Callable, typename... Args>
    struct is_invocable : internal::select_invoke_traits<Callable, Args...>::is_invocable
    {
        // Determines whether Callable is callable with Args
    };

    template <typename Callable, typename... Args>
    inline constexpr bool is_invocable_v = internal::select_invoke_traits<Callable, Args...>::is_invocable::value;

    template <typename Callable, typename... Args>
    struct is_nothrow_invocable : internal::select_invoke_traits<Callable, Args...>::is_nothrow_invocable
    {
        // Determines whether Callable is nothrow-callable with Args
    };

    template <typename Callable, typename... Args>
    inline constexpr bool is_nothrow_invocable_v =
            internal::select_invoke_traits<Callable, Args...>::is_nothrow_invocable::value;

    template <typename ReturnType, typename Callable, typename... Args>
    struct is_invocable_ret : internal::is_invocable_ret_impl<ReturnType, Callable, Args...>
    {
        // Determines whether Callable is callable with Args and return type ReturnType
    };

    template <typename ReturnType, typename Callable, typename... Args>
    inline constexpr bool is_invocable_ret_v = internal::is_invocable_ret_impl<ReturnType, Callable, Args...>::value;

    template <typename ReturnType, typename Callable, typename... Args>
    struct is_nothrow_invocable_ret : internal::select_invoke_traits<Callable, Args...>::template is_nothrow_invocable_ret<ReturnType>
    {
        // Determines whether Callable is nothrow-callable with Args and return type ReturnType
    };

    template <typename ReturnType, typename Callable, typename... Args>
    inline constexpr bool is_nothrow_invocable_ret_v =
            internal::select_invoke_traits<Callable, Args...>::template is_nothrow_invocable_ret<ReturnType>::value;

    struct any_type
    {
        #if defined(RYTHE_MSVC)
        template <typename T>
        constexpr operator T&(); // implicit conversion to any type. NOLINT
        #else
        template <typename T>
        constexpr operator T(); // implicit conversion to any type. NOLINT
        #endif
        template <typename T>
        constexpr any_type(const T&); // implicit conversion from any type. NOLINT
    };

    namespace internal
    {
        template <typename Func, size_type... ParamCounts>
        consteval bool test_invocable_impl([[maybe_unused]] integer_sequence<size_type, ParamCounts...> intSeq)
        {
            return ((make_sequence_t<is_invocable, any_type, ParamCounts, Func>::value) || ...);
        }

        template <typename T, size_type... ParamCounts>
        consteval bool test_constructible_impl([[maybe_unused]] integer_sequence<size_type, ParamCounts...> intSeq)
        {
            return ((make_sequence_t<is_constructible, any_type, ParamCounts, T>::value) || ...);
        }
    } // namespace internal

    template <typename Func, size_type MaxParams = 32>
    struct is_invocable_any
    {
        static constexpr bool value = internal::test_invocable_impl<Func>(make_index_sequence<MaxParams>{});
    };

    template <typename Func, size_type MaxParams = 32>
    constexpr bool is_invocable_any_v = is_invocable_any<Func, MaxParams>::value;

    template <typename Func, size_type MaxParams = 32>
    constexpr bool is_function_ptr_v = (is_empty_v<Func> || is_pointer_v<Func>) && is_invocable_any_v<Func, MaxParams>;

    template <typename Func, size_type MaxParams = 32>
    constexpr bool is_functor_v = requires { &Func::operator(); } && is_invocable_any_v<Func, MaxParams>;

    template <typename T, size_type MaxParams = 32>
    struct is_constructible_any
    {
        static constexpr bool value = internal::test_constructible_impl<T>(make_index_sequence<MaxParams>{});
    };

    template <typename T, size_type MaxParams = 32>
    constexpr bool is_constructible_any_v = is_constructible_any<T, MaxParams>::value;

    template <typename T>
    inline constexpr bool is_abstract_v = !is_void_v<T> && !is_constructible_any_v<T>;

    template <typename T>
    struct is_abstract
    {
        static constexpr bool value = is_abstract_v<T>;
    };

    // TODO: Make our own ratio type.
    template <typename Type>
    constexpr bool is_ratio_v = false; // test for ratio type

    template <int_max Numerator, int_max Denominator>
    constexpr bool is_ratio_v<::std::ratio<Numerator, Denominator>> = true;

    template <typename Type>
    struct is_ratio : bool_constant<is_ratio_v<Type>> {};

    [[rythe_always_inline]] constexpr const void* advance(const void* ptr, const size_type count) noexcept
    {
        return bit_cast<const byte*>(ptr) + count;
    }

    [[rythe_always_inline]] constexpr void* advance(void* ptr, const size_type count) noexcept
    {
        return bit_cast<byte*>(ptr) + count;
    }

    [[rythe_always_inline]] constexpr const void* advance(const void* ptr, const diff_type count) noexcept
    {
        return bit_cast<const byte*>(ptr) + count;
    }

    [[rythe_always_inline]] constexpr void* advance(void* ptr, const diff_type count) noexcept
    {
        return bit_cast<byte*>(ptr) + count;
    }

    template <typename T>
    [[rythe_always_inline]] constexpr const T* advance(const T* ptr, const size_type count) noexcept
    {
        return bit_cast<const T*>(advance(static_cast<const void*>(ptr), count));
    }

    template <typename T>
    [[rythe_always_inline]] constexpr T* advance(T* ptr, const size_type count) noexcept
    {
        return bit_cast<T*>(advance(static_cast<void*>(ptr), count));
    }

    template <typename T>
    [[rythe_always_inline]] constexpr const T* advance(const T* ptr, const diff_type count) noexcept
    {
        return bit_cast<const T*>(advance(static_cast<const void*>(ptr), count));
    }

    template <typename T>
    [[rythe_always_inline]] constexpr T* advance(T* ptr, const diff_type count) noexcept
    {
        return bit_cast<T*>(advance(static_cast<void*>(ptr), count));
    }

    template <typename LHS, typename RHS>
    constexpr bool is_pointer_assignable_v = requires(LHS* lhs, RHS* rhs) { lhs = rhs; };

    struct lval_ref_signal {};

    using ref_signal = lval_ref_signal;

    struct rval_ref_signal {};

    using move_signal = rval_ref_signal;

    struct const_signal {};

    struct pointer_signal {};

    template <typename T, typename... DecorationSignals>
    struct decorate_type;

    template <typename T, typename DecorationSignal>
    struct decorate_type<T, DecorationSignal>
    {
        static_assert(false, "Unknown signal."); // NOLINT
    };

    template <typename T>
    struct decorate_type<T>
    {
        using type = T;
    };

    template <typename T, typename DecorationSignal, typename... Rest>
    struct decorate_type<T, DecorationSignal, Rest...>
    {
        using type = decorate_type<typename decorate_type<T, DecorationSignal>::type, Rest...>::type;
    };

    template <typename T>
    struct decorate_type<T, lval_ref_signal>
    {
        using type = add_lval_ref<T>::type;
    };

    template <typename T>
    struct decorate_type<T, rval_ref_signal>
    {
        using type = add_rval_ref<T>::type;
    };

    template <typename T>
    struct decorate_type<T, const_signal>
    {
        using type = add_const<T>::type;
    };

    template <typename T>
    struct decorate_type<T, pointer_signal>
    {
        using type = add_pointer<T>::type;
    };

    template <typename T, typename... DecorationSignals>
    using decorate_type_t = decorate_type<T, DecorationSignals...>::type;

    namespace internal
    {
        [[nodiscard]] void* get_global_empty_type_placeholder_ptr();
    }

    template <typename T>
        requires (is_empty_v<T>)
    [[nodiscard]] [[rythe_always_inline]] constexpr T* address_of_empty() noexcept
    {
        if (is_constant_evaluated())
        {
            static constexpr T tmp;
            return const_cast<T*>(&tmp);
        }
        else
        {
            return static_cast<T*>(internal::get_global_empty_type_placeholder_ptr());
        }
    }
} // namespace rsl
