#pragma once

#include "../util/concepts.hpp"

#include "delegate_base.hpp"

namespace rsl
{
    template <typename FuncSig>
    class delegate;

    template <typename FuncSig>
    class multicast_delegate;

    template <typename ReturnType, typename... ParamTypes>
    class delegate<ReturnType(ParamTypes...)> final :
        private delegate_base<ReturnType(ParamTypes...)>
    {
        friend class multicast_delegate<ReturnType(ParamTypes...)>;

        using base = delegate_base<ReturnType(ParamTypes...)>;
        using typed_alloc_type = typename base::typed_alloc_type;
        using stub_type = typename base::stub_type;

    public:
        using return_type = ReturnType;
        using param_types = type_sequence<ParamTypes...>;
        using invocation_element = typename base::invocation_element;

        [[rythe_always_inline]] constexpr delegate();

        [[rythe_always_inline]] explicit constexpr delegate(allocator_storage allocator);

        template <invocable<ReturnType(ParamTypes...)> Functor>
        [[rythe_always_inline]] constexpr delegate(const Functor& instance);

        template <invocable<ReturnType(ParamTypes...)> Functor>
        [[rythe_always_inline]] constexpr delegate(allocator_storage allocator, const Functor& instance);

        template <functor Functor>
            requires invocable<Functor, ReturnType(ParamTypes...)>
        [[rythe_always_inline]] constexpr delegate(const Functor& instance);

        template <functor Functor>
            requires invocable<Functor, ReturnType(ParamTypes...)>
        [[rythe_always_inline]] constexpr delegate(allocator_storage allocator, const Functor& instance);

        template <typename T, ReturnType (T::*TMethod)(ParamTypes...)>
        [[nodiscard]] [[rythe_always_inline]] constexpr static delegate create(T& instance);

        template <typename T, ReturnType (T::*TMethod)(ParamTypes...)>
        [[nodiscard]] [[rythe_always_inline]] constexpr static delegate
        create(allocator_storage allocator, T& instance);

        template <typename T, ReturnType (T::*TMethod)(ParamTypes...) const>
        [[nodiscard]] [[rythe_always_inline]] constexpr static delegate create(const T& instance);

        template <typename T, ReturnType (T::*TMethod)(ParamTypes...) const>
        [[nodiscard]] [[rythe_always_inline]] constexpr static delegate
        create(allocator_storage allocator, const T& instance);

        template <ReturnType (*TMethod)(ParamTypes...)>
        [[nodiscard]] [[rythe_always_inline]] constexpr static delegate create();

        template <ReturnType (*TMethod)(ParamTypes...)>
        [[nodiscard]] [[rythe_always_inline]] constexpr static delegate create(allocator_storage allocator);

        template <functor Functor>
            requires invocable<Functor, ReturnType(ParamTypes...)>
        [[nodiscard]] [[rythe_always_inline]] constexpr static delegate create(const Functor& instance);

        template <functor Functor>
            requires invocable<Functor, ReturnType(ParamTypes...)>
        [[nodiscard]] [[rythe_always_inline]] constexpr static delegate
        create(allocator_storage allocator, const Functor& instance);

        [[rythe_always_inline]] constexpr void set_allocator(allocator_storage allocator)
            noexcept(is_nothrow_copy_assignable_v<allocator_storage>);

        [[nodiscard]] [[rythe_always_inline]] constexpr memory_allocator& get_allocator() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const memory_allocator& get_allocator() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr bool empty() const noexcept;
        [[rythe_always_inline]] constexpr void clear() noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr bool operator==(nullptr_type) const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr bool operator!=(nullptr_type) const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr bool operator==(const delegate&) const noexcept = default;
        [[nodiscard]] [[rythe_always_inline]] constexpr bool operator!=(const delegate&) const noexcept = default;

        [[nodiscard]] [[rythe_always_inline]] constexpr bool
        operator==(const multicast_delegate<ReturnType(ParamTypes...)>& other) const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr bool
        operator!=(const multicast_delegate<ReturnType(ParamTypes...)>& other) const noexcept;

        template <typename T, ReturnType (T::*TMethod)(ParamTypes...)>
        [[rythe_always_inline]] constexpr delegate& assign(T& instance);

        template <typename T, ReturnType (T::*TMethod)(ParamTypes...) const>
        [[rythe_always_inline]] constexpr delegate& assign(const T& instance);

        template <ReturnType (*TMethod)(ParamTypes...)>
        [[rythe_always_inline]] constexpr delegate& assign();

        template <invocable<ReturnType(ParamTypes...)> Functor>
        [[rythe_always_inline]] constexpr delegate& operator=(const Functor& instance);
        [[rythe_always_inline]] constexpr delegate& operator=(const delegate&) = default;

        [[rythe_always_inline]] constexpr ReturnType operator()(ParamTypes... args) const;
        [[rythe_always_inline]] constexpr ReturnType invoke(ParamTypes... args) const;

    private:
        [[rythe_always_inline]] constexpr delegate(allocator_storage allocator, invocation_element&& e);

        allocator_storage m_alloc;
        invocation_element m_invocation;
    };
} // namespace rsl

#include "delegate.inl"
