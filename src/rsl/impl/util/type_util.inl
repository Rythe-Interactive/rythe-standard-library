namespace rsl
{
    template <typename T, size_type Size>
    [[nodiscard]] constexpr T* begin(T (&arr)[Size]) noexcept;

    template <typename T, size_type Size>
    [[nodiscard]] constexpr T* end(T (&arr)[Size]) noexcept;

    template <template <typename> typename Compare, typename T, T A, T B>
    struct do_compare
    {
        static constexpr Compare<T> comp{};
        static constexpr bool value = comp(A, B);
    };

    template <template <typename> typename Compare, typename T, T A, T B>
    inline constexpr bool do_compare_v = do_compare<Compare, T, A, B>::value;

    template <typename T, typename... Args>
    struct is_brace_constructible
    {
        static constexpr bool value = requires(Args&&... args) { T{ ::rsl::forward<Args>(args)... }; };
    };

    template <typename T, typename... Args>
    inline constexpr bool is_brace_constructible_v = is_brace_constructible<T, Args...>::value;


    template <rsl::size_type I, typename Check, typename...>
    struct element_at_is_same_as;

    template <rsl::size_type I, typename Check, typename Type, typename... Types>
    struct element_at_is_same_as<I, Check, Type, Types...> : element_at_is_same_as<I - 1, Check, Type, Types...> {};

    template <typename Check, typename Type, typename... Types>
    struct element_at_is_same_as<0, Check, Type, Types...>
    {
        static constexpr bool value = is_same_v<Check, Type>;
    };

    template <rsl::size_type I, typename Check>
    struct element_at_is_same_as<I, Check>
    {
        static constexpr bool value = false;
    };

    template <rsl::size_type I, typename Check, typename... Types>
    inline constexpr bool element_at_is_same_as_v = element_at_is_same_as<I, Check, Types...>::value;

    template <typename T, typename = void>
    struct is_always_equal
    {
        using type = bool_constant<::rsl::is_empty_v<T>>;
    };

    template <typename T>
    struct is_always_equal<T, void_t<typename T::is_always_equal>>
    {
        using type = typename T::is_always_equal;
    };

    template <typename T>
    using is_always_equal_t = typename is_always_equal<T>::type;

    namespace internal
    {
        template <typename T, size_type>
        struct make_signed_impl;

        template <typename T>
        struct make_signed_impl<T, 1>
        {
            using type = signed char;
        };

        template <typename T>
        struct make_signed_impl<T, 2>
        {
            using type = short;
        };

        template <typename T>
        struct make_signed_impl<T, 4>
        {
            using type = conditional_t<is_same_v<T, long> || is_same_v<T, unsigned long>, long, int>;
        };

        template <typename T>
        struct make_signed_impl<T, 8>
        {
            using type = long long;
        };
    } // namespace internal

    template <typename T>
    struct make_signed : internal::make_signed_impl<T, sizeof(T)>
    {
        static_assert(is_nonbool_integral_v<T> || is_enum_v<T>);
    };

    template <>
    struct make_signed<float>
    {
        using type = float;
    };

    template <>
    struct make_signed<double>
    {
        using type = double;
    };

    template <>
    struct make_signed<long double>
    {
        using type = long double;
    };

    template <typename T>
    using make_signed_t = typename make_signed<T>::type;

    namespace internal
    {
        template <typename T, size_type>
        struct make_unsigned_impl;

        template <typename T>
        struct make_unsigned_impl<T, 1>
        {
            using type = unsigned char;
        };

        template <typename T>
        struct make_unsigned_impl<T, 2>
        {
            using type = unsigned short;
        };

        template <typename T>
        struct make_unsigned_impl<T, 4>
        {
            using type = conditional_t<is_same_v<T, long> || is_same_v<T, unsigned long>, unsigned long, unsigned int>;
        };

        template <typename T>
        struct make_unsigned_impl<T, 8>
        {
            using type = unsigned long long;
        };
    } // namespace internal

    template <typename T>
    struct make_unsigned : internal::make_unsigned_impl<T, sizeof(T)>
    {
        static_assert(is_nonbool_integral_v<T> || is_enum_v<T>);
    };

    template <>
    struct make_unsigned<float>
    {
        using type = float;
    };

    template <>
    struct make_unsigned<double>
    {
        using type = double;
    };

    template <>
    struct make_unsigned<long double>
    {
        using type = long double;
    };

    template <typename T>
    using make_unsigned_t = typename make_unsigned<T>::type;

    template <size_type N>
    struct unsigned_integer_of_size : internal::make_unsigned_impl<void, N> {};

    template <size_type N>
    using unsigned_integer_of_size_t = typename unsigned_integer_of_size<N>::type;

    template <size_type N>
    struct signed_integer_of_size : internal::make_signed_impl<void, N> {};

    template <size_type N>
    using signed_integer_of_size_t = typename signed_integer_of_size<N>::type;

    template <typename A, typename B>
    struct largest : conditional<(sizeof(A) > sizeof(B)), A, B> {};

    template <typename A, typename B>
    using largest_t = typename largest<A, B>::type;

    template <typename A, typename B>
    struct smallest : conditional<(sizeof(A) < sizeof(B)), A, B> {};

    template <typename A, typename B>
    using smallest_t = typename smallest<A, B>::type;

#define RYTHE_HAS_FUNC(x)                                                                                                             \
    namespace internal                                                                                                                \
    {                                                                                                                                 \
        template <typename, typename T>                                                                                               \
        struct RYTHE_CONCAT(has_, RYTHE_CONCAT(x, _impl))                                                                             \
        {                                                                                                                             \
            static_assert(                                                                                                            \
                    ::rsl::integral_constant<::rsl::bool_t<T>, false>::value, "Second template param needs to be of function type."); \
        };                                                                                                                            \
                                                                                                                                      \
        template <typename C, typename Ret, typename... Args>                                                                         \
        struct RYTHE_CONCAT(has_, RYTHE_CONCAT(x, _impl))<C, Ret(Args...)>                                                            \
        {                                                                                                                             \
            static constexpr bool value = requires(C& val, Args&&... args) {                                                          \
                { val.x(::rsl::forward<Args>(args)...) } -> ::rsl::convertible_to<Ret>;                                               \
            };                                                                                                                        \
        };                                                                                                                            \
    }                                                                                                                                 \
                                                                                                                                      \
    template <typename C, typename F>                                                                                                 \
    constexpr bool RYTHE_CONCAT(has_, RYTHE_CONCAT(x, _v)) = RYTHE_CONCAT(internal::has_, RYTHE_CONCAT(x, _impl))<C, F>::value;       \
                                                                                                                                      \
    template <typename C, typename F>                                                                                                 \
    concept RYTHE_CONCAT(has_, x) = RYTHE_CONCAT(has_, RYTHE_CONCAT(x, _v))<C, F>;                                                    \
                                                                                                                                      \
    namespace internal                                                                                                                \
    {                                                                                                                                 \
        template <typename, typename T>                                                                                               \
        struct RYTHE_CONCAT(has_static_, RYTHE_CONCAT(x, _impl))                                                                      \
        {                                                                                                                             \
            static_assert(                                                                                                            \
                    ::rsl::integral_constant<::rsl::bool_t<T>, false>::value, "Second template param needs to be of function type."); \
        };                                                                                                                            \
                                                                                                                                      \
        template <typename C, typename Ret, typename... Args>                                                                         \
        struct RYTHE_CONCAT(has_static_, RYTHE_CONCAT(x, _impl))<C, Ret(Args...)>                                                     \
        {                                                                                                                             \
            static constexpr bool value = requires(Args&&... args) {                                                                  \
                { C::x(::rsl::forward<Args>(args)...) } -> ::rsl::convertible_to<Ret>;                                                \
            };                                                                                                                        \
        };                                                                                                                            \
    }                                                                                                                                 \
                                                                                                                                      \
    template <typename C, typename F>                                                                                                 \
    constexpr bool RYTHE_CONCAT(has_static_, RYTHE_CONCAT(x, _v)) =                                                                   \
            RYTHE_CONCAT(internal::has_static_, RYTHE_CONCAT(x, _impl))<C, F>::value;                                                 \
                                                                                                                                      \
    template <typename C, typename F>                                                                                                 \
    concept RYTHE_CONCAT(has_static_, x) = RYTHE_CONCAT(has_static_, RYTHE_CONCAT(x, _v))<C, F>;
} // namespace rsl
