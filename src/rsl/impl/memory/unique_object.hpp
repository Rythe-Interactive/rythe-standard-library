#pragma once
#include "unique_resource.hpp"

namespace rsl
{
    template <typename T>
    class unique_object : private unique_resource<T*>
    {
    public:
        using unique_rsc = unique_resource<T*>;
        using mem_rsc = typename unique_rsc::mem_rsc;

        [[rythe_always_inline]] constexpr unique_object(nullptr_type) noexcept;

        [[rythe_always_inline]] explicit unique_object(allocator_storage allocator) noexcept;

        [[rythe_always_inline]] explicit unique_object(const type_erased_factory& factory) noexcept;

        [[rythe_always_inline]] explicit unique_object(allocator_storage allocator, const type_erased_factory& factory) noexcept;

        template <typename... Args>
        [[rythe_always_inline]] constexpr static unique_object create_in_place(Args&&... args)
                noexcept(is_nothrow_constructible_v<T, Args...>);

        template <typename... Args>
        [[rythe_always_inline]] static unique_object create_in_place_with_allocator(allocator_storage allocator, Args&&... args)
                noexcept(is_nothrow_constructible_v<T, Args...>);

        [[rythe_always_inline]] constexpr unique_object() noexcept = default;
        unique_object(const unique_object&) = delete;
        [[rythe_always_inline]] constexpr unique_object(unique_object&& other) noexcept;

        template <typename OtherType>
            requires(is_pointer_assignable_v<T, OtherType>)
        [[rythe_always_inline]] constexpr unique_object(unique_object<OtherType>&& other) noexcept; // NOLINT(*-explicit-constructor)

        [[rythe_always_inline]] constexpr unique_object& operator=(unique_object&& other) noexcept;

        template <typename OtherType>
            requires(is_pointer_assignable_v<T, OtherType>)
        [[rythe_always_inline]] constexpr unique_object& operator=(unique_object<OtherType>&& other) noexcept;


        [[nodiscard]] [[rythe_always_inline]] constexpr bool is_armed() const noexcept { return unique_rsc::is_armed(); }

        [[rythe_always_inline]] constexpr void disarm() noexcept { unique_rsc::disarm(); }

        [[rythe_always_inline]] constexpr T* get() noexcept { return *unique_rsc::get(); }
        [[rythe_always_inline]] constexpr const T* get() const noexcept { return *unique_rsc::get(); }

        [[rythe_always_inline]] constexpr T& operator*() noexcept { return **unique_rsc::get(); }
        [[rythe_always_inline]] constexpr const T& operator*() const noexcept { return **unique_rsc::get(); }
        [[rythe_always_inline]] constexpr T* operator->() noexcept { return *unique_rsc::get(); }
        [[rythe_always_inline]] constexpr const T* operator->() const noexcept { return *unique_rsc::get(); }

    private:
        template <typename FriendT>
        friend class unique_object;

        struct deleter
        {
            allocator_storage allocator;
            void operator()(T* mem) noexcept;

            operator bool() const noexcept { return allocator; }
        };

        template <typename... Args>
        [[rythe_always_inline]] constexpr void arm(Args&&... args) noexcept(is_nothrow_constructible_v<T, Args...>);
    };

    // TODO(Glyn): Create `temporary_object` that when moved from will invalidate itself.
    // Effectively the same as `unique_object&&` but with clearer ownership transfer.
    // Crucially allows for `view<temporary_object>` to be used to move `unique_object`s into containers.
    template <typename T>
    using temporary_object = unique_object<T>;
} // namespace rsl

#include "unique_object.inl"
