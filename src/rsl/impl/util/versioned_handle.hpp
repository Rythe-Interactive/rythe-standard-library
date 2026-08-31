#pragma once

#include "../rsl_core.hpp"

namespace rsl
{
    template <typename T>
    struct invalid_value
    {
        constexpr static T value{};
    };

    template <typename T>
        requires requires {
            { T::invalid } -> same_as<T>;
        }
    struct invalid_value<T>
    {
        constexpr static T value = T::invalid;
    };

    template <typename T>
        requires integer_type<T>
    struct invalid_value<T>
    {
        constexpr static T value = 0;
    };

    template <typename T>
    constexpr T invalid_value_v = invalid_value<T>::value;

    template <typename HandleType, size_type VersionCounterBitCount, size_type HandleBitCount = sizeof(HandleType) * 8ull>
    struct versioned_handle
    {
        constexpr static size_type handle_bits = HandleBitCount;
        constexpr static size_type counter_bits = VersionCounterBitCount;

        using handle_type = HandleType;
        using counter_type = unsigned_integer_of_size_t<align_value(VersionCounterBitCount, 8ull) / 8ull>;

        using value_type = unsigned_integer_of_size_t<align_value(HandleBitCount + VersionCounterBitCount, 8ull) / 8ull>;
        constexpr static value_type handle_mask = static_cast<value_type>((1ull << handle_bits) - 1ull);

        value_type value;
        static const versioned_handle invalid;
    };

    template <typename HandleType, size_type VersionCounterBitCount, size_type HandleBitCount>
    [[nodiscard]] [[rythe_always_inline]] bool operator==(
            versioned_handle<HandleType, VersionCounterBitCount, HandleBitCount> lhs,
            versioned_handle<HandleType, VersionCounterBitCount, HandleBitCount> rhs) noexcept
    {
        return lhs.value == rhs.value;
    }

    template <typename HandleType, size_type VersionCounterBitCount, size_type HandleBitCount>
    [[nodiscard]] [[rythe_always_inline]] bool operator!=(
            versioned_handle<HandleType, VersionCounterBitCount, HandleBitCount> lhs,
            versioned_handle<HandleType, VersionCounterBitCount, HandleBitCount> rhs) noexcept
    {
        return lhs.value != rhs.value;
    }

    template <typename T>
    constexpr bool is_versioned_handle_v = false;

    template <typename HandleType, size_type VersionCounterBitCount, size_type HandleBitCount>
    constexpr bool is_versioned_handle_v<versioned_handle<HandleType, VersionCounterBitCount, HandleBitCount>> = true;

    template <typename T>
    concept versioned_handle_type = is_versioned_handle_v<T>;

    template <versioned_handle_type VersionedHandleType>
    [[nodiscard]] [[rythe_always_inline]] constexpr VersionedHandleType make_versioned_handle(typename VersionedHandleType::handle_type handle, typename VersionedHandleType::counter_type counter) noexcept
    {
        using value_type = typename VersionedHandleType::value_type;
        return { .value = static_cast<value_type>(handle) | static_cast<value_type>((counter << VersionedHandleType::handle_bits)) };
    }

    template <versioned_handle_type VersionedHandleType>
    [[nodiscard]]
    [[rythe_always_inline]] constexpr pair<typename VersionedHandleType::handle_type, typename VersionedHandleType::counter_type>
            decompose_versioned_handle(VersionedHandleType handle) noexcept
    {
        return { static_cast<typename VersionedHandleType::handle_type>(handle.value & VersionedHandleType::handle_mask),
                 static_cast<typename VersionedHandleType::counter_type>(handle.value >> VersionedHandleType::handle_bits) };
    }

    namespace internal
    {
        template <typename VersionedHandleType>
        constexpr VersionedHandleType invalid_versioned_handle_impl = {
            .value = static_cast<typename VersionedHandleType::value_type>(invalid_value_v<typename VersionedHandleType::handle_type>)
        };
    }

    template <typename HandleType, size_type VersionCounterBitCount, size_type HandleBitCount>
    const versioned_handle<HandleType, VersionCounterBitCount, HandleBitCount>
            versioned_handle<HandleType, VersionCounterBitCount, HandleBitCount>::invalid =
                    internal::invalid_versioned_handle_impl<versioned_handle>;

    template<size_type bitCount>
    struct sized_counter
    {
        unsigned_integer_of_size_t<align_value(bitCount, 8ull) / 8ull> value : bitCount;
    };

    template<typename T>
    using counter_of_handle = sized_counter<T::counter_bits>;

#define RYTHE_DECLARE_VERSIONED_OPAQUE_HANDLE(name, counterBitCount)                                                                  \
    RYTHE_DECLARE_OPAQUE_HANDLE(name##_impl)                                                                                          \
    using name = rsl::versioned_handle<name##_impl, counterBitCount>;                                                                 \
    [[maybe_unused]] constexpr name invalid_##name = rsl::internal::invalid_versioned_handle_impl<name>;

#define RYTHE_DECLARE_VERSIONED_OPAQUE_HANDLE_INVALID_VALUE(name, invalidValue, counterBitCount)                                      \
    RYTHE_DECLARE_OPAQUE_HANDLE_INVALID_VALUE(name##_impl, invalidValue)                                                              \
    using name = rsl::versioned_handle<name##_impl, counterBitCount>;                                                                 \
    [[maybe_unused]] constexpr name invalid_##name = rsl::internal::invalid_versioned_handle_impl<name>;

#define RYTHE_DECLARE_VERSIONED_OPAQUE_HANDLE_UNDERLYING_TYPE_INVALID_VALUE(name, underlyingType, invalidValue, counterBitCount)      \
    RYTHE_DECLARE_OPAQUE_HANDLE_UNDERLYING_TYPE_INVALID_VALUE(name##_impl, underlyingType, invalidValue)                              \
    using name = rsl::versioned_handle<name##_impl, counterBitCount>;                                                                 \
    [[maybe_unused]] constexpr name invalid_##name = rsl::internal::invalid_versioned_handle_impl<name>;

#define RYTHE_DECLARE_VERSIONED_OPAQUE_HANDLE_BIT_COUNT(name, handleBitCount, counterBitCount)                                        \
    RYTHE_DECLARE_OPAQUE_HANDLE_BIT_COUNT(name##_impl, handleBitCount)                                                                \
    using name = rsl::versioned_handle<name##_impl, counterBitCount, handleBitCount>;                                                 \
    [[maybe_unused]] constexpr name invalid_##name = rsl::internal::invalid_versioned_handle_impl<name>;

#define RYTHE_DECLARE_VERSIONED_OPAQUE_HANDLE_BIT_COUNT_INVALID_VALUE(name, handleBitCount, invalidValue, counterBitCount)            \
    RYTHE_DECLARE_OPAQUE_HANDLE_BIT_COUNT_INVALID_VALUE(name##_impl, handleBitCount, invalidValue)                                    \
    using name = rsl::versioned_handle<name##_impl, counterBitCount, handleBitCount>;                                                 \
    [[maybe_unused]] constexpr name invalid_##name = rsl::internal::invalid_versioned_handle_impl<name>;

#define RYTHE_DECLARE_VERSIONED_HANDLE(name, valueType, counterBitCount)                                                              \
    using name = rsl::versioned_handle<valueType, counterBitCount>;                                                                   \
    [[maybe_unused]] constexpr name invalid_##name = rsl::internal::invalid_versioned_handle_impl<name>;

#define RYTHE_DECLARE_VERSIONED_HANDLE_BIT_COUNT(name, valueType, counterBitCount, handleBitCount)                                    \
    using name = rsl::versioned_handle<valueType, counterBitCount, handleBitCount>;                                                   \
    [[maybe_unused]] constexpr name invalid_##name = rsl::internal::invalid_versioned_handle_impl<name>;

} // namespace rsl
