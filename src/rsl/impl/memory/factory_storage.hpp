#pragma once
#include "factory.hpp"

namespace rsl
{
    template <factory_type Factory>
    struct factory_storage;

    template <factory_type Factory>
        requires (!is_empty_v<Factory>)
    struct factory_storage<Factory> final
    {
        [[rythe_always_inline]] constexpr factory_storage() noexcept(is_nothrow_constructible_v<Factory>) = default;

        [[rythe_always_inline]] constexpr factory_storage(const Factory& factory)
            noexcept(is_nothrow_copy_constructible_v<Factory>) : value(factory) {}
        [[rythe_always_inline]] constexpr factory_storage(Factory&& factory)
            noexcept(is_nothrow_move_constructible_v<Factory>) : value(rsl::move(factory)) {}

        template <factory_type Other>
        [[rythe_always_inline]] constexpr factory_storage(const factory_storage<Other>& other)
            noexcept(is_nothrow_constructible_v<Factory, const Other&>) : value(other.value) {}
        template <factory_type Other>
        [[rythe_always_inline]] constexpr factory_storage(factory_storage<Other>&& other)
            noexcept(is_nothrow_constructible_v<Factory, Other&&>) : value(rsl::move(other.value)) {}

        [[rythe_always_inline]] constexpr Factory& operator*() noexcept { return value; }
        [[rythe_always_inline]] constexpr const Factory& operator*() const noexcept { return value; }
        [[rythe_always_inline]] constexpr Factory* operator->() noexcept { return &value; }
        [[rythe_always_inline]] constexpr const Factory* operator->() const noexcept { return &value; }

        [[rythe_always_inline]] constexpr operator bool() const noexcept { return value.is_valid(); }

        Factory value;
    };

    template <factory_type Factory>
        requires (is_empty_v<Factory>)
    struct factory_storage<Factory> final
    {
        [[rythe_always_inline]] constexpr factory_storage() noexcept = default;
        [[rythe_always_inline]] constexpr factory_storage(const Factory&) noexcept {}
        [[rythe_always_inline]] constexpr factory_storage(Factory&&) noexcept {}

        template <factory_type Other>
        [[rythe_always_inline]] constexpr factory_storage(const factory_storage<Other>&) noexcept {}
        template <factory_type Other>
        [[rythe_always_inline]] constexpr factory_storage(factory_storage<Other>&&) noexcept {}

        [[rythe_always_inline]] constexpr Factory& operator*() noexcept { return *address_of_empty<Factory>(); }
        [[rythe_always_inline]] constexpr const Factory& operator*() const noexcept { return *address_of_empty<Factory>(); }
        [[rythe_always_inline]] constexpr Factory* operator->() noexcept { return address_of_empty<Factory>(); }
        [[rythe_always_inline]] constexpr const Factory* operator->() const noexcept { return address_of_empty<Factory>(); }

        [[rythe_always_inline]] constexpr operator bool() const noexcept { return Factory{}.is_valid(); }
    };

    template <>
    struct factory_storage<polymorphic_factory> final
    {
        [[rythe_always_inline]] constexpr factory_storage() noexcept = default;

        [[rythe_always_inline]] constexpr factory_storage(polymorphic_factory& factory) noexcept : value(&factory) {}

        [[rythe_always_inline]] constexpr polymorphic_factory& operator*() noexcept { return *value; }
        [[rythe_always_inline]] constexpr const polymorphic_factory& operator*() const noexcept { return *value; }
        [[rythe_always_inline]] constexpr polymorphic_factory* operator->() noexcept { return value; }
        [[rythe_always_inline]] constexpr const polymorphic_factory* operator->() const noexcept { return value; }

        [[rythe_always_inline]] constexpr operator bool() const noexcept { return value && value->is_valid(); }

        mutable polymorphic_factory* value = nullptr;
    };
} // namespace rsl
