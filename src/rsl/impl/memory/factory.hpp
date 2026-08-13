#pragma once

#include "../util/assert.hpp"
#include "../util/concepts.hpp"
#include "../util/type_traits.hpp"

namespace rsl
{
    RYTHE_HAS_FUNC(from_view)

    template <typename T>
    struct factory;

    template <constructible_at_all T>
    struct factory<T>
    {
    public:
        constexpr static bool valid_factory = false;
        [[rythe_always_inline]] constexpr static bool is_valid() noexcept { return valid_factory; }

        template <typename... Args>
        constexpr static T construct_single_inline(Args&&... args) noexcept(is_nothrow_constructible_v<T, Args...>);
        template <typename... Args>
        constexpr static T* construct(void* ptr, size_type count, Args&&... args) noexcept(is_nothrow_constructible_v<T, Args...>);
        constexpr static T* copy(void* dst, const T* src, size_type count) noexcept(is_nothrow_copy_constructible_v<T>);
        constexpr static T* move(void* dst, T* src, size_type count) noexcept(is_nothrow_move_constructible_v<T>);
        constexpr static void destroy(T* ptr, size_type count) noexcept;

        constexpr static size_type type_size() noexcept { return sizeof(T); }
        constexpr static bool trivial_copy() noexcept { return is_trivially_copyable_v<T>; }
        constexpr static id_type type_id() noexcept { return rsl::type_id<T>(); }
    };

    template<>
    struct factory<void>
    {
    public:
        constexpr static bool valid_factory = false;
        [[rythe_always_inline]] constexpr static bool is_valid() noexcept { return valid_factory; }

        template <typename... Args>
        constexpr static void construct_single_inline(Args&&...) { rsl_assert_unreachable(); }
        template <typename... Args>
        constexpr static void* construct(void*, size_type, Args&&...) { rsl_assert_unreachable(); return nullptr; }
        constexpr static void* copy(void*, const void*, size_type) { rsl_assert_unreachable(); return nullptr; }
        constexpr static void* move(void*, void*, size_type) { rsl_assert_unreachable(); return nullptr; }
        constexpr static void destroy(void*, size_type) noexcept { rsl_assert_unreachable(); }

        constexpr static size_type type_size() noexcept { rsl_assert_unreachable(); return 0; }
        constexpr static bool trivial_copy() noexcept { rsl_assert_unreachable(); return false; }
        constexpr static id_type type_id() noexcept { rsl_assert_unreachable(); return 0; }
    };

    template <not_constructible T>
    struct factory<T> : public factory<void> {};

    class type_erased_factory
    {
    public:
        using ptr_type = void*;

        using construct_func = void* (*)(void* ptr, size_type count);
        using copy_func = void* (*)(void* dst, const void* src, size_type count);
        using move_func = void* (*)(void* dst, void* src, size_type count);
        using destroy_func = void (*)(void* ptr, size_type count);

        [[rythe_always_inline]] bool is_valid() const noexcept;

        [[rythe_always_inline]] constexpr type_erased_factory() noexcept = default;
        [[nodiscard]] [[rythe_always_inline]] bool operator==(const type_erased_factory&) const noexcept = default;
        [[nodiscard]] [[rythe_always_inline]] bool operator!=(const type_erased_factory&) const noexcept = default;

        template <typename T>
        [[rythe_always_inline]] constexpr type_erased_factory(construct_type_signal_type<T>) noexcept; // NOLINT(*-explicit-constructor)

        void* construct(void* ptr, size_type count) const;
        void* copy(void* dst, const void* src, size_type count) const;
        void* move(void* dst, void* src, size_type count) const;
        void destroy(void* ptr, size_type count) const noexcept;
        [[nodiscard]] size_type type_size() const noexcept;
        [[nodiscard]] bool trivial_copy() const noexcept;
        [[nodiscard]] id_type type_id() const noexcept;

    private:
        construct_func m_constructFunc = nullptr;
        copy_func m_copyFunc = nullptr;
        move_func m_moveFunc = nullptr;
        destroy_func m_destroyFunc = nullptr;
        size_type m_typeSize = 0;
        bool m_triviallyCopyable = false;
        id_type m_typeId = invalid_id;
    };
} // namespace rsl

#include "factory.inl"
