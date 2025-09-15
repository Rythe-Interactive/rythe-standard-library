#pragma once

#define RYTHE_EXPAND(x) x

#define _RYTHE_CONCAT_IMPL_(A, B) A##B

#define RYTHE_CONCAT(A, B) _RYTHE_CONCAT_IMPL_(A, B)

#define RYTHE_ANONYMOUS_NAME(x) CONCAT(x, __LINE__)

#define RYTHE_ANON_VAR(Type, Category) inline static Type RYTHE_EXPAND(RYTHE_ANONYMOUS_NAME(Category))

#define RYTHE_STRINGIFY(x) #x

#define RYTHE_DEBUG_VALUE 1
#define RYTHE_RELEASE_VALUE 2

#if defined(_DEBUG) || defined(DEBUG)
    /**@def RYTHE_DEBUG
     * @brief Defined in debug mode.
     */
    #define RYTHE_DEBUG
    #define RYTHE_CONFIGURATION RYTHE_DEBUG_VALUE

    #if !defined(RYTHE_VALIDATE)
        #define RYTHE_VALIDATE
    #endif
#else
    /**@def RYTHE_RELEASE
     * @brief Defined in release mode.
     */
    #define RYTHE_RELEASE
    #define RYTHE_CONFIGURATION RYTHE_RELEASE_VALUE
#endif

#if !defined(RYTHE_HIGH_IMPACT_VALIDATION_LEVEL)
    #define RYTHE_HIGH_IMPACT_VALIDATION_LEVEL 3
#endif

#if !defined(RYTHE_MEDIUM_IMPACT_VALIDATION_LEVEL)
    #define RYTHE_MEDIUM_IMPACT_VALIDATION_LEVEL 2
#endif

#if !defined(RYTHE_LOW_IMPACT_VALIDATION_LEVEL)
    #define RYTHE_LOW_IMPACT_VALIDATION_LEVEL 1
#endif

#define RYTHE_VALIDATION_LEVEL_NONE 0

#if !defined(RYTHE_VALIDATION_LEVEL)
    #if defined(RYTHE_VALIDATE)
        #define RYTHE_VALIDATION_LEVEL RYTHE_HIGH_IMPACT_VALIDATION_LEVEL
    #else
        #define RYTHE_VALIDATION_LEVEL RYTHE_VALIDATION_LEVEL_NONE
    #endif
#elif RYTHE_VALIDATION_LEVEL != RYTHE_VALIDATION_LEVEL_NONE
    #if !defined(RYTHE_VALIDATE)
        #define RYTHE_VALIDATE
    #endif
#elif defined(RYTHE_VALIDATE)
    #undef RYTHE_VALIDATE
#endif

namespace rsl
{
#if defined(RYTHE_VALIDATE)
    constexpr bool RYTHE_VALIDATE_ON = true;
#else
    constexpr bool RYTHE_VALIDATE_ON = false;
#endif
} // namespace rsl


#pragma region /////////////////////////////////// Operating system /////////////////////////////////////

#define RYTHE_WINDOWS_INDEX 0
#define RYTHE_LINUX_INDEX 1
#define RYTHE_MAX_PLATFORMS 2

#if defined(_WIN64)
    #define RYTHE_PLATFORM_INDEX RYTHE_WINDOWS_INDEX
#elif defined(__linux__)
    #define RYTHE_PLATFORM_INDEX RYTHE_LINUX_INDEX
#endif

#define RYTHE_PLATFORM_WINDOWS (RYTHE_PLATFORM_INDEX == RYTHE_WINDOWS_INDEX)
#define RYTHE_PLATFORM_LINUX (RYTHE_PLATFORM_INDEX == RYTHE_LINUX_INDEX)

#if RYTHE_PLATFORM_WINDOWS
    #define RYTHE_PLATFORM_LITTLE_ENDIAN
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    #define RYTHE_PLATFORM_LITTLE_ENDIAN
#else
    #define RYTHE_PLATFORM_BIG_ENDIAN
#endif


#pragma endregion

#pragma region ////////////////////////////////// Target architecture ///////////////////////////////////

#if defined(_M_AMD64) || defined(_M_X64) || defined(__x86_64__) || defined(__amd64__)
    #define RYTHE_ARCH_X86_64
#endif

#if defined(__i386__) || defined(__i386) || defined(_M_IX86) || defined(_X86_)
    #define RYTHE_ARCH_X86_32
#endif

#if defined(__arm__) || defined(__thumb__) || defined(_ARM) || defined(_M_ARM) || defined(_M_ARMT) || defined(__TARGET_ARCH_ARM) ||   \
        defined(__TARGET_ARCH_THUMB)
    #if defined(__aarch64__) || defined(_M_ARM64)
        #define RYTHE_ARCH_ARM64
    #else
        #define RYTHE_ARCH_ARM
    #endif
#endif


#if defined(__riscv)
    #if __riscv_xlen == 32
        #define RYTHE_ARCH_RISCV32
    #elif __riscv_xlen == 64
        #define RYTHE_ARCH_RISCV32
    #else
        #define RYTHE_ARCH_RISCV_UNKNOWN
    #endif
#endif

#pragma endregion

#pragma region //////////////////////////////////// Detect compiler /////////////////////////////////////

#if defined(__clang__)
    // clang
    #define RYTHE_CLANG
    #if defined(__GNUG__) || (defined(__GNUC__) && defined(__cplusplus))
        #define RYTHE_CLANG_GCC
    #elif defined(_MSC_VER)
        #define RYTHE_CLANG_MSVC
    #endif
#elif defined(__GNUG__) || (defined(__GNUC__) && defined(__cplusplus))
  // gcc
    #define RYTHE_GCC
#elif defined(_MSC_VER)
  // msvc
    #define RYTHE_MSVC
#endif

#pragma endregion

#pragma region ////////////////////////////////// Compiler specifics ////////////////////////////////////

#if !defined(rythe_nullop)
    #if defined(RYTHE_CLANG) || defined(RYTHE_GCC)
        #define rythe_nullop() asm volatile("nop");
    #elif defined(RYTHE_MSVC)
        #include <intrin.h>
        #define rythe_nullop() __nop();
    #else
        #define rythe_nullop() ;
    #endif
#endif

#if !defined(rythe_pause_instruction)
    #if defined(RYTHE_CLANG) || defined(RYTHE_GCC)
        #define rythe_pause_instruction __builtin_ia32_pause
    #elif defined(RYTHE_MSVC)
        #define rythe_pause_instruction _mm_pause
    #else
        #define rythe_pause_instruction nullop
    #endif
#endif

#if !defined(rythe_debugbreak_instruction)
    #if defined(RYTHE_MSVC) || defined(RYTHE_CLANG_MSVC)
        #define rythe_debugbreak_instruction __debugbreak
    #elif defined(RYTHE_CLANG) && __has_builtin(__builtin_debugtrap)
        #define rythe_debugbreak_instruction __builtin_debugtrap
    #else
        #include <signal.h>
        #if defined(SIGTRAP)
            #define rythe_debugbreak_instruction() raise(SIGTRAP)
        #else
            #define rythe_debugbreak_instruction() raise(SIGABRT)
        #endif
    #endif
#endif

#if !defined(__RYTHE_FULL_FUNC__)
    #if defined(RYTHE_CLANG) || defined(RYTHE_GCC)
        #define __RYTHE_FULL_FUNC__ __PRETTY_FUNCTION__
    #elif defined(RYTHE_MSVC)
        #define __RYTHE_FULL_FUNC__ __FUNCSIG__
    #else
        #define __RYTHE_FULL_FUNC__ __func__
    #endif
#endif

#if defined(RYTHE_CLANG)
    #define RYTHE_PRAGMA_TO_STR(x) _Pragma(#x)
    #define RYTHE_CLANG_SUPPRESS_WARNING_PUSH _Pragma("clang diagnostic push")
    #define RYTHE_CLANG_SUPPRESS_WARNING(w) RYTHE_PRAGMA_TO_STR(clang diagnostic ignored w)
    #define RYTHE_CLANG_SUPPRESS_WARNING_POP _Pragma("clang diagnostic pop")
    #define RYTHE_CLANG_SUPPRESS_WARNING_WITH_PUSH(w) RYTHE_CLANG_SUPPRESS_WARNING_PUSH RYTHE_CLANG_SUPPRESS_WARNING(w)
#else
    #define RYTHE_CLANG_SUPPRESS_WARNING_PUSH
    #define RYTHE_CLANG_SUPPRESS_WARNING(w)
    #define RYTHE_CLANG_SUPPRESS_WARNING_POP
    #define RYTHE_CLANG_SUPPRESS_WARNING_WITH_PUSH(w)
#endif

#if defined(RYTHE_GCC)
    #define RYTHE_PRAGMA_TO_STR(x) _Pragma(#x)
    #define RYTHE_GCC_SUPPRESS_WARNING_PUSH _Pragma("GCC diagnostic push")
    #define RYTHE_GCC_SUPPRESS_WARNING(w) RYTHE_PRAGMA_TO_STR(GCC diagnostic ignored w)
    #define RYTHE_GCC_SUPPRESS_WARNING_POP _Pragma("GCC diagnostic pop")
    #define RYTHE_GCC_SUPPRESS_WARNING_WITH_PUSH(w) RYTHE_GCC_SUPPRESS_WARNING_PUSH RYTHE_GCC_SUPPRESS_WARNING(w)
#else
    #define RYTHE_GCC_SUPPRESS_WARNING_PUSH
    #define RYTHE_GCC_SUPPRESS_WARNING(w)
    #define RYTHE_GCC_SUPPRESS_WARNING_POP
    #define RYTHE_GCC_SUPPRESS_WARNING_WITH_PUSH(w)
#endif

#if defined(RYTHE_MSVC)
    #define RYTHE_MSVC_SUPPRESS_WARNING_PUSH __pragma(warning(push))
    #define RYTHE_MSVC_SUPPRESS_WARNING(w) __pragma(warning(disable : w))
    #define RYTHE_MSVC_SUPPRESS_WARNING_POP __pragma(warning(pop))
    #define RYTHE_MSVC_SUPPRESS_WARNING_WITH_PUSH(w) RYTHE_MSVC_SUPPRESS_WARNING_PUSH RYTHE_MSVC_SUPPRESS_WARNING(w)
#else
    #define RYTHE_MSVC_SUPPRESS_WARNING_PUSH
    #define RYTHE_MSVC_SUPPRESS_WARNING(w)
    #define RYTHE_MSVC_SUPPRESS_WARNING_POP
    #define RYTHE_MSVC_SUPPRESS_WARNING_WITH_PUSH(w)
#endif

#if defined(RYTHE_MSVC)
    #define RYTHE_COMPILER_WARNING(desc)                                                                                              \
        __pragma(message(__FILE__ "(" RYTHE_STRINGIFY(__LINE__) ") : warning: " RYTHE_STRINGIFY(desc)))
    #define RYTHE_COMPILER_ERROR(desc) __pragma(message(__FILE__ "(" RYTHE_STRINGIFY(__LINE__) ") : error: " RYTHE_STRINGIFY(desc)))
#elif defined(RYTHE_GCC) || defined(RYTHE_CLANG)
    #define RYTHE_COMPILER_WARNING(desc) _Pragma(RYTHE_STRINGIFY(GCC warning desc))
    #define RYTHE_COMPILER_ERROR(desc) _Pragma(RYTHE_STRINGIFY(GCC error desc))
#endif

#if defined(__FMA__) || defined(__AVX2__) || defined(__AVX__) || defined(__FMA4__)
    #define RYTHE_FMA_ENABLED
#endif

#pragma endregion

#pragma region ////////////////////////////////// Language convention ///////////////////////////////////

#if defined(RYTHE_WINDOWS) && !defined(RYTHE_WINDOWS_USE_CDECL)
/**@def RYTHE_CCONV
 * @brief the calling convention exported functions will use in the args engine
 */
    #define RYTHE_CCONV __fastcall
#elif defined(RYTHE_MSVC)
    #define RYTHE_CCONV __cdecl
#else
    #define RYTHE_CCONV
#endif

/**@def RYTHE_CPP17V
 * @brief the version number of c++17 as long
 */
#define RYTHE_CPP17V 201703L

/**@def NO_MANGLING
 * @brief exports functions with C style names instead of C++ mangled names
 */
#define RYTHE_NO_MANGLING extern "C"

/**@def RYTHE_FUNC
 * @brief export setting + calling convention used by the engine
 */
#define RYTHE_FUNC RYTHE_CCONV

/**@def RYTHE_INTERFACE
 * @brief un-mangled function name +  export setting + calling convention used by the engine
 */
#define RYTHE_INTERFACE RYTHE_NO_MANGLING RYTHE_CCONV

#if !defined(__FUNC__)
    #define __FUNC__ __func__
#endif

#pragma endregion

#pragma region /////////////////////////////////////// Attributes ///////////////////////////////////////

#if defined(RYTHE_DISABLE_ALWAYS_INLINE)
    #define rythe_always_inline
#else
    #if defined(RYTHE_GCC)
        // GCC can't inline unless marked with the keyword inline,
        // which in C++ means that the definition is on the same line as the declaration
        // or local to the translation unit.
        // Crucially this is not the same as compiler optimization inlining behaviour.
        #define rythe_always_inline
    #elif defined(RYTHE_CLANG)
        #define rythe_always_inline clang::always_inline
    #elif defined(RYTHE_MSVC)
        #define rythe_always_inline msvc::forceinline
    #else
        #define rythe_always_inline
    #endif
#endif

#if defined(RYTHE_GCC)
    #define rythe_never_inline gnu::noinline
    #define rythe_allocating
    #define rythe_open_enum gnu::enum_extensibility(open)
    #define rythe_closed_enum gnu::enum_extensibility(closed)
    #define rythe_flag_enum gnu::flag_enum
    #define rythe_preferred_name(name) gnu::preferred_name(name)
    #define rythe_no_unique_address no_unique_address
#elif defined(RYTHE_CLANG)
    #define rythe_never_inline clang::noinline
    #define rythe_allocating
    #define rythe_open_enum clang::enum_extensibility(open)
    #define rythe_closed_enum clang::enum_extensibility(closed)
    #define rythe_flag_enum clang::flag_enum
    #define rythe_preferred_name(name) clang::preferred_name(name)
    #if defined(RYTHE_CLANG_MSVC)
        #define rythe_no_unique_address msvc::no_unique_address
    #else
        #define rythe_no_unique_address no_unique_address
    #endif
#elif defined(RYTHE_MSVC)
    #define rythe_never_inline msvc::noinline
    #define rythe_allocating
    #define rythe_open_enum
    #define rythe_closed_enum
    #define rythe_flag_enum
    #define rythe_preferred_name(name)
    #define rythe_no_unique_address msvc::no_unique_address
#else
    #define rythe_never_inline
    #define rythe_allocating
    #define rythe_open_enum
    #define rythe_closed_enum
    #define rythe_flag_enum
    #define rythe_preferred_name(name)
    #define rythe_no_unique_address no_unique_address
#endif

#pragma endregion

#pragma region /////////////////////////////////////// Utilities ////////////////////////////////////////

#define DECLARE_OPAQUE_HANDLE(name)                                                                                                   \
    enum struct [[rythe_open_enum]] name : rsl::ptr_type                                                                              \
    {                                                                                                                                 \
        invalid = 0                                                                                                                   \
    };                                                                                                                                \
    [[maybe_unused]] constexpr name invalid_##name = name::invalid;

#define DECLARE_OPAQUE_HANDLE_INVALID_VALUE(name, invalidValue)                                                                       \
    enum struct [[rythe_open_enum]] name : rsl::ptr_type                                                                              \
    {                                                                                                                                 \
        invalid = invalidValue                                                                                                        \
    };                                                                                                                                \
    [[maybe_unused]] constexpr name invalid_##name = name::invalid;

#define DECLARE_SINGLETON(name)                                                                                                       \
namespace internal                                                                                                                    \
{                                                                                                                                     \
    [[nodiscard]] name& get_default_##name() noexcept;                                                                                \
}                                                                                                                                     \
                                                                                                                                      \
using get_##name##_func = name& (*)();                                                                                                \
extern get_##name##_func get_##name;

#define RULE_OF_5(type)                                                                                                               \
    type() = default;                                                                                                                 \
    type(const type&) = default;                                                                                                      \
    type(type&&) = default;                                                                                                           \
    type& operator=(const type&) = default;                                                                                           \
    type& operator=(type&&) = default;                                                                                                \
    ~type() = default;

#define VIRTUAL_RULE_OF_5(type)                                                                                                       \
    type() = default;                                                                                                                 \
    type(const type&) = default;                                                                                                      \
    type(type&&) = default;                                                                                                           \
    type& operator=(const type&) = default;                                                                                           \
    type& operator=(type&&) = default;                                                                                                \
    virtual ~type() = default;

#define RULE_OF_5_NOEXCEPT(type)                                                                                                      \
    type() noexcept = default;                                                                                                        \
    type(const type&) noexcept = default;                                                                                             \
    type(type&&) noexcept = default;                                                                                                  \
    type& operator=(const type&) noexcept = default;                                                                                  \
    type& operator=(type&&) noexcept = default;                                                                                       \
    ~type() = default;

#define RULE_OF_5_CONSTEXPR_NOEXCEPT(type)                                                                                            \
    constexpr type() noexcept = default;                                                                                              \
    constexpr type(const type&) noexcept = default;                                                                                   \
    constexpr type(type&&) noexcept = default;                                                                                        \
    constexpr type& operator=(const type&) noexcept = default;                                                                        \
    constexpr type& operator=(type&&) noexcept = default;                                                                             \
    constexpr ~type() = default;

#define NO_DEF_CTOR_RULE5(type)                                                                                                       \
    type(const type&) = default;                                                                                                      \
    type(type&&) = default;                                                                                                           \
    type& operator=(const type&) = default;                                                                                           \
    type& operator=(type&&) = default;                                                                                                \
    ~type() = default;

#define NO_DEF_CTOR_RULE5_NOEXCEPT(type)                                                                                              \
    type(const type&) noexcept = default;                                                                                             \
    type(type&&) noexcept = default;                                                                                                  \
    type& operator=(const type&) noexcept = default;                                                                                  \
    type& operator=(type&&) noexcept = default;                                                                                       \
    ~type() = default;

#define NO_DTOR_RULE5(type)                                                                                                           \
    type() = default;                                                                                                                 \
    type(const type&) = default;                                                                                                      \
    type(type&&) = default;                                                                                                           \
    type& operator=(const type&) = default;                                                                                           \
    type& operator=(type&&) = default;

#define NO_DTOR_RULE5_NOEXCEPT(type)                                                                                                  \
    type() noexcept = default;                                                                                                        \
    type(const type&) noexcept = default;                                                                                             \
    type(type&&) noexcept = default;                                                                                                  \
    type& operator=(const type&) noexcept = default;                                                                                  \
    type& operator=(type&&) noexcept = default;

#define NO_DTOR_RULE5_CONSTEXPR_NOEXCEPT(type)                                                                                        \
    constexpr type() noexcept = default;                                                                                              \
    constexpr type(const type&) noexcept = default;                                                                                   \
    constexpr type(type&&) noexcept = default;                                                                                        \
    constexpr type& operator=(const type&) noexcept = default;                                                                        \
    constexpr type& operator=(type&&) noexcept = default;

#define COPY_FUNCS(type)                                                                                                              \
    type(const type&) = default;                                                                                                      \
    type& operator=(const type&) = default;

#define COPY_FUNCS_DECO(pre, type, post)                                                                                              \
    pre type(const type&) post = default;                                                                                             \
    pre type& operator=(const type&) post = default;

#define COPY_FUNCS_NOEXCEPT(type)                                                                                                     \
    type(const type&) noexcept = default;                                                                                             \
    type& operator=(const type&) noexcept = default;

#define COPY_FUNCS_CONSTEXPR_NOEXCEPT(type)                                                                                           \
    constexpr type(const type&) noexcept = default;                                                                                   \
    constexpr type& operator=(const type&) noexcept = default;

#define MOVE_FUNCS(type)                                                                                                              \
    type(type&&) = default;                                                                                                           \
    type& operator=(type&&) = default;

#define MOVE_FUNCS_DECO(pre, type, post)                                                                                              \
    pre type(type&&) post = default;                                                                                                  \
    pre type& operator=(type&&) post = default;

#define MOVE_FUNCS_NOEXCEPT(type)                                                                                                     \
    type(type&&) noexcept = default;                                                                                                  \
    type& operator=(type&&) noexcept = default;

#define RYTHE_BIT_FLAG_OPERATORS(EnumType)                                                                                            \
    constexpr EnumType operator|(EnumType lhs, EnumType rhs) noexcept                                                                 \
    {                                                                                                                                 \
        return static_cast<EnumType>(static_cast<underlying_type_t<EnumType>>(lhs) | static_cast<underlying_type_t<EnumType>>(rhs));  \
    }                                                                                                                                 \
    constexpr EnumType operator&(EnumType lhs, EnumType rhs) noexcept                                                                 \
    {                                                                                                                                 \
        return static_cast<EnumType>(static_cast<underlying_type_t<EnumType>>(lhs) & static_cast<underlying_type_t<EnumType>>(rhs));  \
    }
#pragma endregion
