#pragma once
#include "../containers/optional.hpp"

#include "allocator.hpp"
#include "memory_resource_base.hpp"

namespace rsl
{
    namespace internal
    {
        struct unique_payload_base
        {
            virtual ~unique_payload_base() = default;
            virtual void destroy(void*) noexcept { rsl_assert_unreachable(); }
        };

        template <typename Deleter, typename T>
        concept unique_deleter_type = requires(Deleter del, T& val) { del(val); };

        template <typename T, unique_deleter_type<T> Deleter>
        struct unique_payload final : public unique_payload_base
        {
            Deleter deleter;

            void destroy(void* value) noexcept override;
        };
    }

    template <typename T, typed_factory_type Factory = default_factory<T>>
    class unique_resource : public internal::select_memory_resource<
            internal::unique_payload_base, type_erased_factory>::type
    {
    public:
        constexpr static bool untyped_memory_resource =
            internal::select_memory_resource<internal::unique_payload_base, type_erased_factory>::is_untyped;
        using mem_rsc = typename internal::select_memory_resource<
            internal::unique_payload_base, type_erased_factory>::type;
        using factory_storage_type = factory_storage<Factory>;
        using factory_t = Factory;

        [[rythe_always_inline]] constexpr unique_resource(nullptr_type)
            noexcept(is_nothrow_constructible_v<mem_rsc>);

        [[rythe_always_inline]] constexpr explicit unique_resource(allocator_storage allocator)
            noexcept(is_nothrow_constructible_v<mem_rsc, allocator_storage>);

        [[rythe_always_inline]] constexpr explicit unique_resource(const type_erased_factory& factory)
            noexcept(is_nothrow_constructible_v<mem_rsc>);

        [[rythe_always_inline]] constexpr unique_resource(
                allocator_storage allocator,
                const type_erased_factory& factory
            ) noexcept(is_nothrow_constructible_v<mem_rsc, allocator_storage>);

        template <typename OtherT, typed_factory_type OtherFactory>
        [[rythe_always_inline]] constexpr unique_resource(
                internal::alloc_and_factory_only_signal_type,
                const unique_resource<OtherT, OtherFactory>& other
            ) noexcept(is_nothrow_constructible_v<mem_rsc, internal::alloc_and_factory_only_signal_type, const mem_rsc&>);

        template <internal::unique_deleter_type<T> Deleter, typename... Args>
        [[rythe_always_inline]] constexpr explicit unique_resource(
                Deleter deleter,
                Args&&... args
            ) noexcept(is_nothrow_constructible_v<mem_rsc> && is_nothrow_constructible_v<T, Args...>);

        template <internal::unique_deleter_type<T> Deleter, typename... Args>
        [[rythe_always_inline]] constexpr unique_resource(
                allocator_storage allocator,
                Deleter deleter,
                Args&&... args
            ) noexcept(is_nothrow_constructible_v<mem_rsc, allocator_storage> && is_nothrow_constructible_v<T, Args...>);

        [[rythe_always_inline]] constexpr unique_resource() noexcept = default;
        unique_resource(const unique_resource&) = delete;
        [[rythe_always_inline]] constexpr unique_resource(unique_resource&& other) noexcept;

        [[rythe_always_inline]] constexpr ~unique_resource() noexcept;

        [[rythe_always_inline]] constexpr unique_resource& operator=(unique_resource&& other) noexcept;

        [[rythe_always_inline]] constexpr void set_factory(const type_erased_factory& factory)
            noexcept(is_nothrow_copy_assignable_v<factory_storage_type>);

        [[nodiscard]] [[rythe_always_inline]] constexpr factory_t& get_factory() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const factory_t& get_factory() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr factory_storage_type& get_factory_storage() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const factory_storage_type&
        get_factory_storage() const noexcept;

        template <internal::unique_deleter_type<T> Deleter, typename... Args>
        [[rythe_always_inline]] constexpr void arm(Deleter deleter, Args&&... args)
            noexcept(is_nothrow_constructible_v<T, Args...>);

        [[nodiscard]] [[rythe_always_inline]] constexpr bool is_armed() const noexcept;

        [[rythe_always_inline]] constexpr void disarm() noexcept;

        [[rythe_always_inline]] constexpr T* get() noexcept { return &*m_value; }
        [[rythe_always_inline]] constexpr const T* get() const noexcept { return &*m_value; }

        [[rythe_always_inline]] constexpr T& operator*() noexcept { return *m_value; }
        [[rythe_always_inline]] constexpr const T& operator*() const noexcept { return *m_value; }
        [[rythe_always_inline]] constexpr T* operator->() noexcept { return &*m_value; }
        [[rythe_always_inline]] constexpr const T* operator->() const noexcept { return &*m_value; }

    protected:
        optional<T> m_value;
    };
} // namespace rsl

#include "unique_resource.inl"
