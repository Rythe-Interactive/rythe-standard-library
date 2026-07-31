#pragma once
#include "optional.hpp"

namespace rsl
{
    template <typename T>
    constexpr optional<T>::optional() noexcept // NOLINT
        : m_hasValue(false) {}

    template <typename T>
    constexpr optional<T>::optional(nullptr_type) noexcept // NOLINT
        : m_hasValue(false) {}

    template <typename T>
    constexpr optional<T>::optional(const optional& other) noexcept(is_nothrow_copy_constructible_v<value_type>)
        : m_hasValue(false)
    {
        if (other.m_hasValue)
        {
            emplace(other.m_value);
        }
    }

    template <typename T>
    constexpr optional<T>::optional(optional&& other) noexcept(is_nothrow_move_constructible_v<value_type>)
        : m_hasValue(false)
    {
        if (other.m_hasValue)
        {
            emplace(move(other.m_value));
        }
        other.reset();
    }

    template <typename T>
    template <typename... Args>
    constexpr optional<T> optional<T>::create_in_place(Args&&... args) noexcept(is_nothrow_constructible_v<value_type, Args...>)
    {
        optional ret;
        ret.emplace(forward<Args>(args)...);
        return ret;
    }

    template <typename T>
    constexpr optional<T>::optional(const value_type& value) noexcept(is_nothrow_copy_constructible_v<value_type>)
    {
        emplace(value);
    }

    template <typename T>
    constexpr optional<T>::optional(value_type&& value) noexcept(is_nothrow_move_constructible_v<value_type>)
    {
        emplace(move(value));
    }

    template <typename T>
    constexpr optional<T>::~optional() noexcept
    {
        reset();
    }

    template <typename T>
    constexpr optional<T>& optional<T>::operator=(nullptr_type) noexcept
    {
        reset();
        return *this;
    }

    template <typename T>
    constexpr optional<T>& optional<T>::operator=(const optional& other) noexcept(is_nothrow_copy_constructible_v<value_type>)
    {
        reset();

        if (other.m_hasValue)
        {
            emplace(other.m_value);
        }

        return *this;
    }

    template <typename T>
    constexpr optional<T>& optional<T>::operator=(optional&& other) noexcept(is_nothrow_move_constructible_v<value_type>)
    {
        reset();

        if (other.m_hasValue)
        {
            emplace(move(other.m_value));
        }

        return *this;
    }

    template <typename T>
    constexpr optional<T>& optional<T>::operator=(const value_type& value) noexcept(is_nothrow_copy_constructible_v<value_type>)
    {
        reset();
        emplace(value);
        return *this;
    }

    template <typename T>
    constexpr optional<T>& optional<T>::operator=(value_type&& value) noexcept(is_nothrow_move_constructible_v<value_type>)
    {
        reset();
        emplace(move(value));
        return *this;
    }

    template <typename T>
    constexpr T* optional<T>::operator->() noexcept
    {
        rsl_assert_invalid_access(m_hasValue);
        return &m_value;
    }

    template <typename T>
    constexpr const T* optional<T>::operator->() const noexcept
    {
        rsl_assert_invalid_access(m_hasValue);
        return &m_value;
    }

    template <typename T>
    constexpr T& optional<T>::operator*() & noexcept
    {
        rsl_assert_invalid_access(m_hasValue);
        return m_value;
    }

    template <typename T>
    constexpr const T& optional<T>::operator*() const & noexcept
    {
        rsl_assert_invalid_access(m_hasValue);
        return m_value;
    }

    template <typename T>
    constexpr T&& optional<T>::operator*() && noexcept
    {
        rsl_assert_invalid_access(m_hasValue);
        return move(m_value);
    }

    template <typename T>
    constexpr const T&& optional<T>::operator*() const && noexcept
    {
        rsl_assert_invalid_access(m_hasValue);
        return move(m_value);
    }

    template <typename T>
    constexpr T& optional<T>::value() & noexcept
    {
        rsl_assert_invalid_access(m_hasValue);
        return m_value;
    }

    template <typename T>
    constexpr const T& optional<T>::value() const & noexcept
    {
        rsl_assert_invalid_access(m_hasValue);
        return m_value;
    }

    template <typename T>
    constexpr T&& optional<T>::value() && noexcept
    {
        rsl_assert_invalid_access(m_hasValue);
        return move(m_value);
    }

    template <typename T>
    constexpr const T&& optional<T>::value() const && noexcept
    {
        rsl_assert_invalid_access(m_hasValue);
        return move(m_value);
    }

    template <typename T>
    constexpr bool optional<T>::holds_value() const noexcept
    {
        return m_hasValue;
    }

    template <typename T>
    constexpr optional<T>::operator bool() const noexcept
    {
        return holds_value();
    }

    template <typename T>
    template <typename... Args>
    constexpr T& optional<T>::emplace(Args&&... args) noexcept(is_nothrow_constructible_v<value_type, Args...>)
    {
        reset();
        m_hasValue = true;
        
        if (is_constant_evaluated())
        {
            m_value = T(forward<Args>(args)...);    
        }
        else
        {
            factory<T>::construct(&m_value, 1, forward<Args>(args)...);
        }
        
        return m_value;
    }

    template <typename T>
    constexpr void optional<T>::reset() noexcept
    {
        if (m_hasValue)
        {
            factory<T>::destroy(&m_value, 1);
            m_hasValue = false;
        }
    }
} // namespace rsl
