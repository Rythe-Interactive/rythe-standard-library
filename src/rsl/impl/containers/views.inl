#pragma once

namespace rsl
{
    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
    constexpr array_view<T, Iter, ConstIter>::operator array_view<const T, ConstIter>() const noexcept
        requires(!is_const_v<T>)
    {
        return array_view<const T, ConstIter>::from_buffer(m_src, m_count);
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
    constexpr array_view<T, Iter, ConstIter> array_view<T, Iter, ConstIter>::from_value(
            T& src
            ) noexcept
    {
        return from_buffer(&src, 1ull);
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
    template <size_type N>
    constexpr array_view<T, Iter, ConstIter>::array_view(
            T (& arr)[N]
            ) noexcept
        : m_src(arr), m_count(N)
    {
        if constexpr (char_type<value_type>)
        {
            if (m_src[m_count - 1] == static_cast<value_type>('\0'))
            {
                --m_count;
            }
        }
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
    constexpr array_view<T, Iter, ConstIter> array_view<T, Iter, ConstIter>::from_buffer(
            T* ptr,
            size_type count
            ) noexcept
    {
        array_view result;
        result.m_src = ptr;
        result.m_count = count;
        return result;
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
    template <not_same_as<T> Other>
    constexpr array_view<T, Iter, ConstIter> array_view<T, Iter, ConstIter>::from_buffer(
            Other* ptr,
            const size_type count
            ) noexcept
        requires same_as<byte, remove_cvr_t<T>> && (is_const_v<T> || !is_const_v<Other>)
    {
        array_view result;
        result.m_src = bit_cast<T*>(ptr);
        result.m_count = count * sizeof(Other);
        return result;
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
    template <contiguous_iterator It>
    constexpr array_view<T, Iter, ConstIter> array_view<T, Iter, ConstIter>::from_iterator_pair(
            It first,
            It last
            ) noexcept(iter_noexcept_deref<It> && iter_noexcept_diff<It>)
        requires same_as<iter_pointer_t<It>, T*>
    {
        return from_buffer(&(*first), last - first);
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
    constexpr array_view<T, Iter, ConstIter> array_view<T, Iter, ConstIter>::from_string_length(T* str, const T terminator) noexcept
        requires char_type<T>
    {
        return from_buffer(str, string_length(str, terminator));
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
    constexpr bool array_view<T, Iter, ConstIter>::operator==(const array_view& rhs) const noexcept
    {
        for (size_type i = 0; i < this->size(); ++i)
        {
            if (this->at(i) != rhs.at(i)) { return false; }
        }

        return true;
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
    constexpr bool array_view<T, Iter, ConstIter>::operator!=(const array_view& rhs) const noexcept
    {
        return !(*this == rhs);
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
    template <size_type N>
    constexpr bool array_view<T, Iter, ConstIter>::operator==(const value_type (& rhs)[N]) const noexcept
    {
        bool result = true;
        for (size_type i = 0; i < this->size(); ++i) { result &= this->at(i) == rhs[i]; }

        return result;
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
    template <size_type N>
    constexpr bool array_view<T, Iter, ConstIter>::operator!=(const value_type (& rhs)[N]) const noexcept
    {
        return !(*this == rhs);
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
    constexpr typename array_view<T, Iter, ConstIter>::iterator_type array_view<T, Iter, ConstIter>::begin() noexcept
    {
        return iterator_type(m_src);
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
    constexpr typename array_view<T, Iter, ConstIter>::const_iterator_type array_view<T, Iter, ConstIter>::begin() const noexcept
    {
        return cbegin();
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
    constexpr typename array_view<T, Iter, ConstIter>::const_iterator_type array_view<T, Iter, ConstIter>::cbegin() const noexcept
    {
        return const_iterator_type(m_src);
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
    constexpr typename array_view<T, Iter, ConstIter>::iterator_type array_view<T, Iter, ConstIter>::end() noexcept
    {
        return iterator_type(m_src + m_count);
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
    constexpr typename array_view<T, Iter, ConstIter>::const_iterator_type array_view<T, Iter, ConstIter>::end() const noexcept
    {
        return cend();
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
    constexpr typename array_view<T, Iter, ConstIter>::const_iterator_type array_view<T, Iter, ConstIter>::cend() const noexcept
    {
        return const_iterator_type(m_src + m_count);
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
    constexpr typename array_view<T, Iter, ConstIter>::reverse_iterator_type array_view<T, Iter, ConstIter>::rbegin() noexcept
    {
        return reverse_iterator(end());
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
    constexpr typename array_view<T, Iter, ConstIter>::const_reverse_iterator_type array_view<
        T, Iter, ConstIter>::rbegin() const noexcept
    {
        return crbegin();
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
    constexpr typename array_view<T, Iter, ConstIter>::const_reverse_iterator_type array_view<
        T, Iter, ConstIter>::crbegin() const noexcept
    {
        return const_reverse_iterator_type(cend());
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
    constexpr typename array_view<T, Iter, ConstIter>::reverse_iterator_type array_view<T, Iter, ConstIter>::rend() noexcept
    {
        return reverse_iterator(begin());
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
    constexpr typename array_view<T, Iter, ConstIter>::const_reverse_iterator_type array_view<
        T, Iter, ConstIter>::rend() const noexcept
    {
        return crend();
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
    constexpr typename array_view<T, Iter, ConstIter>::const_reverse_iterator_type array_view<
        T, Iter, ConstIter>::crend() const noexcept
    {
        return const_reverse_iterator_type(cbegin());
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
    constexpr typename array_view<T, Iter, ConstIter>::reference array_view<T, Iter, ConstIter>::front()
    {
        return at(0);
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
    constexpr typename array_view<T, Iter, ConstIter>::reference array_view<T, Iter, ConstIter>::front() const
    {
        return at(0);
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
    constexpr typename array_view<T, Iter, ConstIter>::reference array_view<T, Iter, ConstIter>::back()
    {
        return at(m_count - 1);
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
    constexpr typename array_view<T, Iter, ConstIter>::reference array_view<T, Iter, ConstIter>::back() const
    {
        return at(m_count - 1);
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
    constexpr typename array_view<T, Iter, ConstIter>::reference array_view<T, Iter, ConstIter>::at(size_type pos)
    {
        return *(begin() + pos);
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
    constexpr typename array_view<T, Iter, ConstIter>::reference array_view<T, Iter, ConstIter>::at(size_type pos) const
    {
        return *(cbegin() + pos);
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
    constexpr typename array_view<T, Iter, ConstIter>::reference array_view<T, Iter, ConstIter>::operator[](const size_type n)
    {
        return at(n);
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
    constexpr typename array_view<T, Iter, ConstIter>::reference array_view<T, Iter, ConstIter>::operator[](const size_type n) const
    {
        return at(n);
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
    constexpr typename array_view<T, Iter, ConstIter>::pointer array_view<T, Iter, ConstIter>::data() noexcept
    {
        return m_src;
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
    constexpr typename array_view<T, Iter, ConstIter>::pointer array_view<T, Iter, ConstIter>::data() const noexcept
    {
        return m_src;
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
    constexpr size_type array_view<T, Iter, ConstIter>::size() const noexcept
    {
        return m_count;
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
    constexpr size_type array_view<T, Iter, ConstIter>::size_bytes() const noexcept
    {
        return m_count * sizeof(value_type);
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
    constexpr bool array_view<T, Iter, ConstIter>::empty() const noexcept
    {
        return m_count == 0 || m_src == nullptr;
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
    constexpr array_view<T, Iter, ConstIter> array_view<T, Iter, ConstIter>::subview(
            const size_type offset,
            diff_type count
            ) const noexcept
    {
        if (offset >= m_count)
        {
            return {};
        }

        if (count < 0ll)
        {
            count = m_count + count;
        }

        const size_type maxCount = m_count - offset;
        if (count > maxCount)
        {
            count = maxCount;
        }

        if (count < 0ll)
        {
            count = 0ull;
        }

        return array_view::from_buffer(m_src + offset, count);
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
    constexpr void array_view<T, Iter, ConstIter>::set_data(const pointer data, const size_type count) noexcept
    {
        m_src = count == 0ull ? nullptr : data;
        m_count = data == nullptr ? 0ull : count;
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
    constexpr void array_view<T, Iter, ConstIter>::reset() noexcept
    {
        m_src = nullptr;
        m_count = 0ull;
    }

    template <contiguous_container_like ArrayType, weakly_equality_comparable_with<container_value_type<ArrayType>> Comparable,
        convertible_to<container_value_type<ArrayType>> Replacement>
    constexpr size_type linear_search_and_replace(
            ArrayType& arr,
            const Comparable& key,
            const Replacement& replacement,
            size_type offset,
            size_type endSearch
            ) noexcept
    {
        size_type count = 0ull;
        size_type index;
        while ((index = linear_search(arr, key, offset, endSearch)) != npos)
        {
            arr[index] = replacement;
            offset = index + 1ull;
            ++count;
        }

        return count;
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter,
        weakly_equality_comparable_with<T> C, contiguous_iterator CIter, contiguous_iterator CConstIter>
    constexpr size_type linear_search_sequence(
            array_view<T, Iter, ConstIter> arr,
            array_view<C, CIter, CConstIter> key,
            size_type offset,
            size_type endSearch
            ) noexcept
    {
        if (key.size() > arr.size())
        {
            return npos;
        }

        if (key.empty())
        {
            return 0ull;
        }

        Iter endIter = endSearch >= arr.size() ? arr.end() : arr.begin() + offset;
        CIter keyBegin = key.begin();
        CIter keyEnd = key.end();

        CIter keyIter = keyBegin;
        for (auto iter = arr.begin() + offset; iter != endIter; ++iter)
        {
            if (*iter == *keyIter)
            {
                ++keyIter;
            }
            else
            {
                keyIter = keyBegin;
            }

            if (keyIter == keyEnd)
            {
                return offset - (key.size() - 1ull);
            }
            ++offset;
        }

        return npos;
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter,
        weakly_equality_comparable_with<T> C, contiguous_iterator CIter, contiguous_iterator CConstIter>
    constexpr size_type reverse_linear_search_sequence(
            array_view<T, Iter, ConstIter> arr,
            array_view<C, CIter, CConstIter> key,
            size_type offset
            ) noexcept
    {
        if (key.size() > arr.size())
        {
            return npos;
        }

        if (key.empty())
        {
            return arr.size() - 1ull;
        }

        using reverse_iter = typename array_view<T, Iter, ConstIter>::reverse_iterator_type;
        reverse_iter endIter = reverse_iter(arr.begin() + offset);
        auto keyBegin = key.rbegin();
        auto keyEnd = key.rend();

        reverse_iter keyIter = keyBegin;
        for (auto iter = arr.rbegin(); iter != endIter; ++iter)
        {
            ++offset;
            if (*iter == *keyIter)
            {
                ++keyIter;
            }
            else
            {
                keyIter = keyBegin;
            }

            if (keyIter == keyEnd)
            {
                return arr.size() - offset;
            }
        }

        return npos;
    }

    // TODO(Rowan): Implement a better search algo
    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter,
        weakly_equality_comparable_with<T> C, contiguous_iterator CIter, contiguous_iterator CConstIter>
    constexpr size_type linear_search_collection(
            array_view<T, Iter, ConstIter> arr,
            array_view<C, CIter, CConstIter> key,
            size_type offset,
            size_type endSearch
            ) noexcept
    {
        if (endSearch >= offset)
        {
            return npos;
        }

        const auto endIter = endSearch >= arr.size() ? arr.end() : arr.begin() + endSearch;
        for (auto iter = arr.begin() + offset; iter != endIter; ++iter)
        {
            for (auto it = key.begin(); it != key.end(); ++it)
            {
                if (*iter == *it) { return offset; }
            }
            ++offset;
        }
        return npos;
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter,
        weakly_equality_comparable_with<T> C, contiguous_iterator CIter, contiguous_iterator CConstIter>
    constexpr size_type linear_search_outside_collection(
            array_view<T, Iter, ConstIter> arr,
            array_view<C, CIter, CConstIter> key,
            size_type offset
            ) noexcept
    {
        for (auto iter = arr.begin() + offset; iter != arr.end(); ++iter)
        {
            bool found = true;
            for (auto it = key.begin(); it != key.end(); ++it)
            {
                if (*iter == *it)
                {
                    found = false;
                    break;
                }
            }

            if (found) { return offset; }

            ++offset;
        }
        return npos;
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter,
        weakly_equality_comparable_with<T> C, contiguous_iterator CIter, contiguous_iterator CConstIter>
    constexpr size_type reverse_linear_search_collection(
            array_view<T, Iter, ConstIter> arr,
            array_view<C, CIter, CConstIter> key,
            size_type offset
            ) noexcept
    {
        using reverse_iter = typename array_view<T, Iter, ConstIter>::reverse_iterator_type;
        reverse_iter endIter = reverse_iter(arr.begin() + offset);
        auto keyBegin = key.begin();
        auto keyEnd = key.end();

        for (auto iter = arr.rbegin(); iter != endIter; ++iter)
        {
            ++offset;
            for (auto it = keyBegin; it != keyEnd; ++it)
            {
                if (*iter == *it) { return arr.size() - offset; }
            }
        }
        return npos;
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter,
        weakly_equality_comparable_with<T> C, contiguous_iterator CIter, contiguous_iterator CConstIter>
    constexpr size_type reverse_linear_search_outside_collection(
            array_view<T, Iter, ConstIter> arr,
            array_view<C, CIter, CConstIter> key,
            [[maybe_unused]] size_type offset
            ) noexcept
    {
        using reverse_iter = typename array_view<T, Iter, ConstIter>::reverse_iterator_type;
        reverse_iter endIter = reverse_iter(arr.begin() + offset);
        auto keyBegin = key.begin();
        auto keyEnd = key.end();

        for (auto iter = arr.rbegin(); iter != endIter; ++iter)
        {
            ++offset;
            bool found = true;
            for (auto it = keyBegin; it != keyEnd; ++it)
            {
                if (*iter == *it)
                {
                    found = false;
                    break;
                }
            }

            if (found) { return arr.size() - offset; }
        }
        return npos;
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter, weakly_equality_comparable_with<T> C>
    constexpr size_type linear_search(array_view<T, Iter, ConstIter> arr, const C& key, size_type offset, size_type endSearch) noexcept
    {
        return linear_search_collection(arr, array_view<const C>::from_value(key), offset, endSearch);
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter, weakly_equality_comparable_with<T> C>
    constexpr size_type linear_search_not_eq(array_view<T, Iter, ConstIter> arr, const C& key, size_type offset) noexcept
    {
        return linear_search_outside_collection(arr, array_view<const C>::from_value(key), offset);
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter, weakly_equality_comparable_with<T> C>
    constexpr size_type reverse_linear_search(array_view<T, Iter, ConstIter> arr, const C& key, size_type offset) noexcept
    {
        return reverse_linear_search_collection(arr, array_view<const C>::from_value(key), offset);
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter, weakly_equality_comparable_with<T> C>
    constexpr size_type reverse_linear_search_not_eq(array_view<T, Iter, ConstIter> arr, const C& key, size_type offset) noexcept
    {
        return reverse_linear_search_outside_collection(arr, array_view<const C>::from_value(key), offset);
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter, typename Func>
    constexpr size_type linear_search_custom(array_view<T, Iter, ConstIter> arr, Func&& comparer, size_type offset) noexcept
    {
        for (auto iter = arr.begin() + offset; iter != arr.end(); ++iter)
        {
            if (comparer(*iter))
            {
                return offset;
            }
            ++offset;
        }
        return npos;
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter, typename Func>
    constexpr size_type reverse_linear_search_custom(array_view<T, Iter, ConstIter> arr, Func&& comparer, size_type offset) noexcept
    {
        using reverse_iter = typename array_view<T, Iter, ConstIter>::reverse_iterator_type;
        reverse_iter endIter = reverse_iter(arr.begin() + offset);

        for (auto iter = arr.rbegin(); iter != endIter; ++iter)
        {
            ++offset;
            if (comparer(*iter))
            {
                return arr.size() - offset;
            }
        }
        return npos;
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter,
        weakly_equality_comparable_with<T> C, contiguous_iterator CIter, contiguous_iterator CConstIter>
    constexpr size_type linear_count_sequence(
            const array_view<T, Iter, ConstIter> arr,
            const array_view<C, CIter, CConstIter> key,
            const size_type offset
            ) noexcept
    {
        const size_type keyLength = key.size();
        const size_type strLength = arr.size();

        size_type count = 0ull;
        size_type i = linear_search_sequence(arr, key, offset) + keyLength;

        while (i <= strLength)
        {
            count++;
            i = linear_search_sequence(arr, key, i) + keyLength;
        }

        return count;
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter,
        weakly_equality_comparable_with<T> C, contiguous_iterator CIter, contiguous_iterator CConstIter>
    constexpr size_type linear_count_collection(
            const array_view<T, Iter, ConstIter> arr,
            const array_view<C, CIter, CConstIter> key,
            const size_type offset
            ) noexcept
    {
        const size_type strLength = arr.size();

        size_type count = 0ull;
        size_type i = linear_search_collection(arr, key, offset);
        while (i < strLength)
        {
            count++;
            i = linear_search_collection(arr, key, i + 1ull);
        }

        return count;
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter,
        weakly_equality_comparable_with<T> C, contiguous_iterator CIter, contiguous_iterator CConstIter>
    constexpr size_type linear_count_outside_collection(
            const array_view<T, Iter, ConstIter> arr,
            const array_view<C, CIter, CConstIter> key,
            const size_type offset
            ) noexcept
    {
        const size_type strLength = arr.size();

        size_type count = 0ull;
        size_type i = linear_search_outside_collection(arr, key, offset);
        while (i < strLength)
        {
            count++;
            i = linear_search_outside_collection(arr, key, i + 1ull);
        }

        return count;
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter, weakly_equality_comparable_with<T> C>
    constexpr size_type linear_count(const array_view<T, Iter, ConstIter> arr, const C& key, const size_type offset) noexcept
    {
        const size_type strLength = arr.size();

        size_type count = 0ull;
        size_type i = linear_search(arr, key, offset);
        while (i < strLength)
        {
            count++;
            i = linear_search(arr, key, i + 1ull);
        }

        return count;
    }

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter, weakly_equality_comparable_with<T> C>
    constexpr size_type linear_count_not_eq(const array_view<T, Iter, ConstIter> arr, const C& key, const size_type offset) noexcept
    {
        const size_type strLength = arr.size();

        size_type count = 0ull;
        size_type i = linear_search_not_eq(arr, key, offset);
        while (i < strLength)
        {
            count++;
            i = linear_search_not_eq(arr, key, i + 1ull);
        }

        return count;
    }

    template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
    constexpr iterator_view<T, Iter, ConstIter>::iterator_view(iterator_type start, iterator_type end) noexcept
        : m_start(start),
          m_end(end) {}

    template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
    constexpr iterator_view<T, Iter, ConstIter>::iterator_view(pointer ptr, size_type count) noexcept
        requires same_as<Iter, T*>
        : m_start(ptr),
          m_end(ptr + count) {}

    template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
    template <size_type N>
    constexpr iterator_view<T, Iter, ConstIter>::iterator_view(value_type (& arr)[N]) noexcept
        requires same_as<Iter, T*>
        : m_start(arr),
          m_end(arr + N) {}

    template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
    constexpr iterator_view<T, Iter, ConstIter>::iterator_view(const value_type& other) noexcept
        requires same_as<Iter, T*>
        : m_start(&other),
          m_end((&other) + 1u) {}

    template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
    constexpr iterator_view<T, Iter, ConstIter>::operator iterator_view<const T, ConstIter>() noexcept
        requires(!is_const_v<T>)
    {
        return iterator_view<const T, const_iterator_type>(m_start, m_end);
    }

    template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
    constexpr iterator_view<T, Iter, ConstIter>::operator contiguous_view_type() noexcept
        requires(contiguous_iterator<Iter> && contiguous_iterator<ConstIter>)
    {
        if constexpr (!is_void_v<contiguous_view_type>) { return contiguous_view_type(&*m_start, size()); }
        else { return contiguous_view_type(); }
    }

    template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
    constexpr iterator_view<T, Iter, ConstIter> iterator_view<T, Iter, ConstIter>::from_string_length(T* str, T terminator) noexcept
        requires(char_type<T> && same_as<Iter, T*>)
    {
        return iterator_view(str, str + string_length(str, terminator));
    }

    template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
    constexpr bool iterator_view<T, Iter, ConstIter>::operator==(const iterator_view& rhs)
    {
        return m_start == rhs.m_start && m_end == rhs.m_end;
    }

    template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
    constexpr bool iterator_view<T, Iter, ConstIter>::operator!=(const iterator_view& rhs)
    {
        return !(*this == rhs);
    }

    template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
    constexpr typename iterator_view<T, Iter, ConstIter>::iterator_type iterator_view<T, Iter, ConstIter>::begin() noexcept
    {
        return m_start;
    }

    template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
    constexpr typename iterator_view<T, Iter, ConstIter>::const_iterator_type iterator_view<T, Iter, ConstIter>::begin() const noexcept
    {
        return m_start;
    }

    template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
    constexpr typename iterator_view<T, Iter, ConstIter>::const_iterator_type iterator_view<
        T, Iter, ConstIter>::cbegin() const noexcept
    {
        return m_start;
    }

    template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
    constexpr typename iterator_view<T, Iter, ConstIter>::iterator_type iterator_view<T, Iter, ConstIter>::end() noexcept
    {
        return m_end;
    }

    template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
    constexpr typename iterator_view<T, Iter, ConstIter>::const_iterator_type iterator_view<T, Iter, ConstIter>::end() const noexcept
    {
        return m_end;
    }

    template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
    constexpr typename iterator_view<T, Iter, ConstIter>::const_iterator_type iterator_view<T, Iter, ConstIter>::cend() const noexcept
    {
        return m_end;
    }

    template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
    constexpr typename iterator_view<T, Iter, ConstIter>::reverse_iterator_type iterator_view<T, Iter, ConstIter>::rbegin() noexcept
    {
        return reverse_iterator(end());
    }

    template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
    constexpr typename iterator_view<T, Iter, ConstIter>::const_reverse_iterator_type iterator_view<
        T, Iter, ConstIter>::rbegin() const noexcept
    {
        return crbegin();
    }

    template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
    constexpr typename iterator_view<T, Iter, ConstIter>::const_reverse_iterator_type iterator_view<
        T, Iter, ConstIter>::crbegin() const noexcept
    {
        return const_reverse_iterator_type(cend());
    }

    template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
    constexpr typename iterator_view<T, Iter, ConstIter>::reverse_iterator_type iterator_view<T, Iter, ConstIter>::rend() noexcept
    {
        return reverse_iterator(begin());
    }

    template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
    constexpr typename iterator_view<T, Iter, ConstIter>::const_reverse_iterator_type iterator_view<
        T, Iter, ConstIter>::rend() const noexcept
    {
        return crend();
    }

    template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
    constexpr typename iterator_view<T, Iter, ConstIter>::const_reverse_iterator_type iterator_view<
        T, Iter, ConstIter>::crend() const noexcept
    {
        return const_reverse_iterator_type(cbegin());
    }

    template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
    constexpr typename iterator_view<T, Iter, ConstIter>::reference iterator_view<T, Iter, ConstIter>::front()
    {
        return *m_start;
    }

    template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
    constexpr typename iterator_view<T, Iter, ConstIter>::reference iterator_view<T, Iter, ConstIter>::front() const
    {
        return *m_start;
    }

    namespace internal
    {
        template <weakly_incrementable Iter>
        constexpr Iter find_last_iter(Iter start, Iter end) noexcept
        {
            if constexpr (bidirectional_iterator<Iter>)
            {
                --end;
                return end;
            }
            else
            {
                while (true)
                {
                    Iter nextIter = start;
                    ++nextIter;
                    if (nextIter == end) // peek ahead
                    {
                        return start;
                    }
                    start = nextIter;
                }
            }
        }
    } // namespace internal

    template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
    constexpr typename iterator_view<T, Iter, ConstIter>::reference iterator_view<T, Iter, ConstIter>::back()
    {
        return internal::find_last_iter(m_start, m_end);
    }

    template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
    constexpr typename iterator_view<T, Iter, ConstIter>::reference iterator_view<T, Iter, ConstIter>::back() const
    {
        return internal::find_last_iter(m_start, m_end);
    }

    template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
    constexpr size_type iterator_view<T, Iter, ConstIter>::size() const noexcept
    {
        return static_cast<size_type>(iterator_diff(m_start, m_end));
    }

    template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
    constexpr bool iterator_view<T, Iter, ConstIter>::empty() const noexcept
    {
        return m_start == m_end;
    }
} // namespace rsl
