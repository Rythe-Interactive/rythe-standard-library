#pragma once
#include "hash_map.hpp"
#include "../../math/util/limits.hpp"

namespace rsl
{
    template <typename MapInfo>
    constexpr hash_map_base<MapInfo>::hash_map_base() noexcept(MapInfo::nothrow_constructible)
        : m_values(),
          m_buckets(),
          m_lastValueBucketIndex(0),
          m_minPsl(0),
          m_maxPsl(0),
          m_hasher(),
          m_keyComparer(),
          m_alloc(),
          m_factory(),
          m_memoryPool() {}

    template <typename MapInfo>
    constexpr hash_map_base<MapInfo>::hash_map_base(
            const hasher_type& h,
            const key_comparer_type& equal
            )
        noexcept(MapInfo::nothrow_copy_constructible)
        : m_values(),
          m_buckets(),
          m_lastValueBucketIndex(0),
          m_minPsl(0),
          m_maxPsl(0),
          m_hasher(h),
          m_keyComparer(equal),
          m_alloc(),
          m_factory(),
          m_memoryPool() {}

    template <typename MapInfo>
    constexpr hash_map_base<MapInfo>::hash_map_base(
            const hasher_type& h
            )
        noexcept(MapInfo::nothrow_hasher_copy_constructible)
        : m_values(),
          m_buckets(),
          m_lastValueBucketIndex(0),
          m_minPsl(0),
          m_maxPsl(0),
          m_hasher(h),
          m_keyComparer(),
          m_alloc(),
          m_factory(),
          m_memoryPool() {}

    template <typename MapInfo>
    constexpr hash_map_base<MapInfo>::hash_map_base(
            const key_comparer_type& equal
            )
        noexcept(MapInfo::nothrow_comparer_copy_constructible)
        : m_values(),
          m_buckets(),
          m_lastValueBucketIndex(0),
          m_minPsl(0),
          m_maxPsl(0),
          m_hasher(),
          m_keyComparer(equal),
          m_alloc(),
          m_factory(),
          m_memoryPool() {}

    template <typename MapInfo>
    constexpr hash_map_base<MapInfo>::hash_map_base(
            const allocator_storage_type& allocStorage
            )
        noexcept(nothrow_constructible_alloc)
        : m_values(allocStorage),
          m_buckets(allocStorage),
          m_lastValueBucketIndex(0),
          m_minPsl(0),
          m_maxPsl(0),
          m_hasher(),
          m_keyComparer(),
          m_alloc(allocStorage),
          m_factory(),
          m_memoryPool(allocStorage) {}

    template <typename MapInfo>
    constexpr hash_map_base<MapInfo>::hash_map_base(
            const factory_storage_type& factoryStorage
            )
        noexcept(nothrow_constructible_fact)
        : m_values(factoryStorage),
          m_buckets(factoryStorage),
          m_lastValueBucketIndex(0),
          m_minPsl(0),
          m_maxPsl(0),
          m_hasher(),
          m_keyComparer(),
          m_alloc(),
          m_factory(factoryStorage),
          m_memoryPool() {}

    template <typename MapInfo>
    constexpr hash_map_base<MapInfo>::hash_map_base(
            const allocator_storage_type& allocStorage,
            const factory_storage_type& factoryStorage
            ) noexcept(nothrow_constructible_alloc_fact)
        : m_values(allocStorage, factoryStorage),
          m_buckets(allocStorage, factoryStorage),
          m_lastValueBucketIndex(0),
          m_minPsl(0),
          m_maxPsl(0),
          m_hasher(),
          m_keyComparer(),
          m_alloc(allocStorage),
          m_factory(factoryStorage),
          m_memoryPool(allocStorage) {}

    template <typename MapInfo>
    template <typename Iter, typename ConstIter>
    constexpr hash_map_base<MapInfo> hash_map_base<MapInfo>::from_view(const iterator_view<Iter, ConstIter> src)
    {
        hash_map_base result;

        for (auto& [key, value] : src)
        {
            result.emplace(key, value);
        }

        return result;
    }

    template <typename MapInfo>
    template <typename Iter, typename ConstIter>
    constexpr hash_map_base<MapInfo> hash_map_base<MapInfo>::move_from_view(iterator_view<Iter, ConstIter> src)
    {
        hash_map_base result;

        for (auto& [key, value] : src)
        {
            result.emplace(rsl::move(key), rsl::move(value));
        }

        return result;
    }

    template <typename MapInfo>
    constexpr hash_map_base<MapInfo> hash_map_base<MapInfo>::create_reserved(
            const size_type capacity
            )
        noexcept(noexcept(declval<hash_map_base>().reserve(0)))
    {
        hash_map_base result;
        result.reserve(capacity);

        return result;
    }

    template <typename MapInfo>
    constexpr size_type hash_map_base<MapInfo>::size() const noexcept
    {
        return m_values.size();
    }

    template <typename MapInfo>
    constexpr bool hash_map_base<MapInfo>::empty() const noexcept
    {
        return size() == 0;
    }

    template <typename MapInfo>
    constexpr size_type hash_map_base<MapInfo>::capacity() const noexcept
    {
        return m_buckets.size();
    }

    template <typename MapInfo>
    constexpr typename hash_map_base<MapInfo>::const_view_type hash_map_base<MapInfo>::view() const noexcept
    {
        return const_view_type(begin(), end());
    }

    template <typename MapInfo>
    constexpr typename hash_map_base<MapInfo>::view_type hash_map_base<MapInfo>::view() noexcept
    {
        return view_type(begin(), end());
    }

    template <typename MapInfo>
    constexpr void hash_map_base<MapInfo>::erase(const key_type& key) noexcept
    {
        erase_impl(key);
    }

    template <typename MapInfo>
    constexpr void hash_map_base<MapInfo>::erase(key_view_alternative key) noexcept
        requires (has_key_view_alternative)
    {
        erase_impl(key);
    }

    template <typename MapInfo>
    constexpr memory_pool<typename hash_map_base<MapInfo>::value_type>& hash_map_base<
        MapInfo>::get_memory_pool() noexcept
        requires (!is_flat)
    {
        return *m_memoryPool;
    }

    template <typename MapInfo>
    constexpr const memory_pool<typename hash_map_base<MapInfo>::value_type>& hash_map_base<
        MapInfo>::get_memory_pool() const noexcept
        requires (!is_flat)
    {
        return *m_memoryPool;
    }

    template <typename MapInfo>
    constexpr typename hash_map_base<MapInfo>::allocator_t& hash_map_base<MapInfo>::get_allocator() noexcept
    {
        return *m_alloc;
    }

    template <typename MapInfo>
    constexpr const typename hash_map_base<MapInfo>::allocator_t& hash_map_base<MapInfo>::get_allocator() const noexcept
    {
        return *m_alloc;
    }

    template <typename MapInfo>
    constexpr typename hash_map_base<MapInfo>::factory_t& hash_map_base<MapInfo>::get_factory() noexcept
    {
        return *m_factory;
    }

    template <typename MapInfo>
    constexpr const typename hash_map_base<MapInfo>::factory_t& hash_map_base<MapInfo>::get_factory() const noexcept
    {
        return *m_factory;
    }

    template <typename MapInfo>
    constexpr typename hash_map_base<MapInfo>::iterator_type hash_map_base<MapInfo>::begin() noexcept
    {
        return iterator_type(m_values.begin());
    }

    template <typename MapInfo>
    constexpr typename hash_map_base<MapInfo>::const_iterator_type hash_map_base<MapInfo>::begin() const noexcept
    {
        return const_iterator_type(m_values.cbegin());
    }

    template <typename MapInfo>
    constexpr typename hash_map_base<MapInfo>::const_iterator_type hash_map_base<MapInfo>::cbegin() const noexcept
    {
        return const_iterator_type(m_values.cbegin());
    }

    template <typename MapInfo>
    constexpr typename hash_map_base<MapInfo>::iterator_type hash_map_base<MapInfo>::end() noexcept
    {
        return iterator_type(m_values.end());
    }

    template <typename MapInfo>
    constexpr typename hash_map_base<MapInfo>::const_iterator_type hash_map_base<MapInfo>::end() const noexcept
    {
        return const_iterator_type(m_values.cend());
    }

    template <typename MapInfo>
    constexpr typename hash_map_base<MapInfo>::const_iterator_type hash_map_base<MapInfo>::cend() const noexcept
    {
        return const_iterator_type(m_values.cend());
    }

    template <typename MapInfo>
    constexpr typename hash_map_base<MapInfo>::reverse_iterator_type hash_map_base<MapInfo>::rbegin() noexcept
    {
        return reverse_iterator_type(m_values.rbegin());
    }

    template <typename MapInfo>
    constexpr typename hash_map_base<MapInfo>::const_reverse_iterator_type hash_map_base<MapInfo>::rbegin() const
        noexcept
    {
        return const_reverse_iterator_type(m_values.crbegin());
    }

    template <typename MapInfo>
    constexpr typename hash_map_base<MapInfo>::const_reverse_iterator_type hash_map_base<MapInfo>::crbegin() const
        noexcept
    {
        return const_reverse_iterator_type(m_values.crbegin());
    }

    template <typename MapInfo>
    constexpr typename hash_map_base<MapInfo>::reverse_iterator_type hash_map_base<MapInfo>::rend() noexcept
    {
        return reverse_iterator_type(m_values.rend());
    }

    template <typename MapInfo>
    constexpr typename hash_map_base<MapInfo>::const_reverse_iterator_type hash_map_base<MapInfo>::rend() const
        noexcept
    {
        return const_reverse_iterator_type(m_values.crend());
    }

    template <typename MapInfo>
    constexpr typename hash_map_base<MapInfo>::const_reverse_iterator_type hash_map_base<MapInfo>::crend() const
        noexcept
    {
        return const_reverse_iterator_type(m_values.crend());
    }

    template <typename MapInfo>
    template <typename... Args>
    constexpr typename hash_map_base<MapInfo>::node_type hash_map_base<MapInfo>::create_node(const key_type& key, Args&&... args)
    {
        if constexpr (is_flat)
        {
            return node_type(key, m_factory->construct_single_inline(rsl::forward<Args>(args)...));
        }
        else
        {
            value_type* newValue = m_memoryPool->allocate();

            if constexpr (is_map)
            {
                new(&newValue->first) key_type(key);
                m_factory->construct(&newValue->second, 1, rsl::forward<Args>(args)...);
            }
            else
            {
                *newValue = key;
            }

            return node_type(newValue);
        }
    }

    template <typename MapInfo>
    template <typename ... Args>
    constexpr typename hash_map_base<MapInfo>::node_type hash_map_base<MapInfo>::create_node(key_type&& key, Args&&... args)
    {
        if constexpr (is_flat)
        {
            return node_type(rsl::move(key), m_factory->construct_single_inline(rsl::forward<Args>(args)...));
        }
        else
        {
            value_type* newValue = m_memoryPool->allocate();

            if constexpr (is_map)
            {
                new(&newValue->first) key_type(rsl::move(key));
                m_factory->construct(&newValue->second, 1, rsl::forward<Args>(args)...);
            }
            else
            {
                *newValue = rsl::move(key);
            }

            return node_type(newValue);
        }
    }

    template <typename MapInfo>
    constexpr void hash_map_base<MapInfo>::destroy_node(node_type& node) noexcept
    {
        if constexpr (!is_flat)
        {
            if constexpr (is_map)
            {
                m_factory->destroy(&node->second, 1);
                node->first.~key_type();
            }
            else
            {
                node->~value_type();
            }

            m_memoryPool->deallocate(node.get_ptr());
            node->set_ptr(nullptr);
        }
    }

    template <typename MapInfo>
    constexpr void hash_map_base<MapInfo>::rehash(const bucket_container& oldBuckets) noexcept
    {
        for (const bucket_type& bucket : oldBuckets)
        {
            size_type oldIndex = bucket.index;
            key_type key = m_values[oldIndex].key();

            const hash_result hash = get_hash_result(key);

            bucket_search_result searchResult = find_next_available(hash.homeIndex, 0, hash.fingerprint, key, false);

            const insert_result result{
                        .index = hash.homeIndex + searchResult.unpackedPsl.psl,
                        .type =
                        (searchResult.type == search_result_type::existing_item ?
                            insert_result_type::existing_item :
                            insert_result_type::new_insertion)
                    };

            bucket_type insertBucket{
                        .pslAndFingerprint = pack_bucket_psl(searchResult.unpackedPsl),
                        .index = oldIndex
                    };

            index_type currentIndex = result.index;
            while (searchResult.type == search_result_type::swap)
            {
                rsl::swap(m_buckets[currentIndex], insertBucket);
                psl_type insertPsl = unpack_bucket_psl(insertBucket);

                index_type homeIndex = currentIndex - insertPsl.psl;

                searchResult = find_next_available(
                        homeIndex,
                        insertPsl.psl + 1,
                        insertPsl.fingerprint,
                        m_values[insertBucket.index].key(),
                        false
                        );
                currentIndex = homeIndex + searchResult.unpackedPsl.psl;

                rsl_assert_frequent(searchResult.type != search_result_type::existing_item);
            }

            rsl_assert_invalid_object(searchResult.type == search_result_type::new_insertion);
            rsl_assert_invalid_object(currentIndex < m_buckets.size());
            m_buckets[currentIndex] = insertBucket;
        }

        m_maxPsl = 0;
        m_minPsl = math::limits<storage_type>::max;
        m_lastValueBucketIndex = 0;
        for (size_type i = 0; i < m_buckets.size(); ++i)
        {
            psl_type unpackedPsl = unpack_bucket_psl(m_buckets[i]);
            if (unpackedPsl.psl < m_minPsl)
            {
                m_minPsl = unpackedPsl.psl;
            }

            if (unpackedPsl.psl > m_maxPsl)
            {
                m_maxPsl = unpackedPsl.psl;
            }

            if (m_buckets[i].index > m_buckets[m_lastValueBucketIndex].index)
            {
                m_lastValueBucketIndex = i;
            }
        }
    }

    template <typename MapInfo>
    constexpr void hash_map_base<MapInfo>::maybe_grow() noexcept(noexcept(reserve(0)))
    {
        const size_type currentCapacity = capacity();
        if (currentCapacity == 0)
        {
            if constexpr (max_load_factor >= 0.5f)
            {
                reserve(4);
            }
            else if constexpr (max_load_factor >= 0.25f)
            {
                reserve(8);
            }
            else
            {
                reserve(16);
            }
            return;
        }

        float32 currentLoadFactor = m_values.size() / static_cast<float32>(m_buckets.size());

        if (currentLoadFactor >= max_load_factor)
        {
            reserve(currentCapacity * 2);
        }
    }

    template <typename MapInfo>
    constexpr typename hash_map_base<MapInfo>::storage_type hash_map_base<MapInfo>::pack_bucket_psl(
            const psl_type& unpackedPsl
            ) noexcept
    {
        return (unpackedPsl.psl << bucket_type::fingerprint_size) | unpackedPsl.fingerprint;
    }

    template <typename MapInfo>
    constexpr typename hash_map_base<MapInfo>::psl_type hash_map_base<MapInfo>::unpack_bucket_psl(
            const bucket_type& bucket
            ) noexcept
    {
        psl_type result{};
        result.psl = (bucket.pslAndFingerprint & bucket_type::psl_mask) >> bucket_type::fingerprint_size;
        result.fingerprint = bucket.pslAndFingerprint & bucket_type::fingerprint_mask;

        return result;
    }

    template <typename MapInfo>
    template <typename KeyType>
    constexpr typename hash_map_base<MapInfo>::hash_result hash_map_base<MapInfo>::get_hash_result(
            const KeyType& key
            ) const noexcept
    {
        id_type hash = m_hasher.hash(key);

        hash_result result{};

        result.fingerprint = hash & bucket_type::fingerprint_mask;
        result.homeIndex = static_cast<index_type>(hash % m_buckets.size());

        return result;
    }

    template <typename MapInfo>
    template <typename KeyType>
    constexpr typename hash_map_base<MapInfo>::bucket_search_result hash_map_base<MapInfo>::find_next_available(
            index_type homeIndex,
            storage_type startPsl,
            storage_type fingerprint,
            const KeyType& key,
            bool earlyOut
            ) const noexcept
    {
        psl_type insertPsl{ .psl = startPsl, .fingerprint = fingerprint };

        index_type searchIndex = homeIndex + insertPsl.psl;
        const size_type bucketCount = m_buckets.size();
        while (searchIndex < bucketCount)
        {
            const bucket_type& bucket = m_buckets[searchIndex];
            if (bucket.pslAndFingerprint == 0u)
            {
                return bucket_search_result{ .unpackedPsl = insertPsl, .type = search_result_type::new_insertion };
            }

            psl_type unpackedPsl = unpack_bucket_psl(bucket);

            if (unpackedPsl.psl < insertPsl.psl)
            {
                return bucket_search_result{ .unpackedPsl = insertPsl, .type = search_result_type::swap };
            }

            if (unpackedPsl.psl == insertPsl.psl && unpackedPsl.fingerprint == insertPsl.fingerprint)
            {
                if (m_keyComparer(m_values[bucket.index].key(), key))
                {
                    return bucket_search_result{ .unpackedPsl = insertPsl, .type = search_result_type::existing_item };
                }
            }

            ++insertPsl.psl;
            searchIndex = homeIndex + insertPsl.psl;

            if (earlyOut && insertPsl.psl > m_maxPsl)
            {
                return bucket_search_result{ .unpackedPsl = psl_type{ .psl = m_maxPsl, .fingerprint = fingerprint },
                                             .type = search_result_type::item_not_found };
            }
        }

        rsl_assert_consistent(insertPsl.psl == (m_buckets.size() - homeIndex));
        return bucket_search_result{ .unpackedPsl = insertPsl, .type = search_result_type::new_insertion };
    }

    template <typename MapInfo>
    constexpr typename hash_map_base<MapInfo>::insert_result hash_map_base<MapInfo>::insert_key_internal(
            const key_type& key,
            const index_type valueIndexHint
            ) noexcept(noexcept(reserve(0)))
    {
        maybe_grow();
        const hash_result hash = get_hash_result(key);

        bucket_search_result searchResult = find_next_available(hash.homeIndex, m_minPsl, hash.fingerprint, key, false);

        const insert_result result{
                    .index = hash.homeIndex + searchResult.unpackedPsl.psl,
                    .type =
                    (searchResult.type == search_result_type::existing_item ?
                        insert_result_type::existing_item :
                        insert_result_type::new_insertion)
                };

        if (searchResult.unpackedPsl.psl < m_minPsl)
        {
            m_minPsl = searchResult.unpackedPsl.psl;
        }

        if (searchResult.unpackedPsl.psl > m_maxPsl)
        {
            m_maxPsl = searchResult.unpackedPsl.psl;
        }

        bucket_type insertBucket{
                    .pslAndFingerprint = pack_bucket_psl(searchResult.unpackedPsl),
                    .index = valueIndexHint
                };

        index_type currentIndex = result.index;

        if (insertBucket.index > m_buckets[m_lastValueBucketIndex].index)
        {
            m_lastValueBucketIndex = currentIndex;
        }

        bool recalcMin = false;
        while (searchResult.type == search_result_type::swap)
        {
            rsl::swap(m_buckets[currentIndex], insertBucket);
            psl_type insertPsl = unpack_bucket_psl(insertBucket);

            index_type homeIndex = currentIndex - insertPsl.psl;

            searchResult = find_next_available(
                    homeIndex,
                    insertPsl.psl + 1,
                    insertPsl.fingerprint,
                    m_values[insertBucket.index].key(),
                    false
                    );

            if (insertPsl.psl == m_minPsl)
            {
                recalcMin = true;
            }

            if (searchResult.unpackedPsl.psl > m_maxPsl)
            {
                m_maxPsl = searchResult.unpackedPsl.psl;
            }

            currentIndex = homeIndex + searchResult.unpackedPsl.psl;

            rsl_assert_frequent(searchResult.type != search_result_type::existing_item);
        }

        if (recalcMin)
        {
            m_minPsl = math::limits<storage_type>::max;
            for (const bucket_type& bucket : m_buckets)
            {
                psl_type unpackedPsl = unpack_bucket_psl(bucket);
                if (unpackedPsl.psl < m_minPsl)
                {
                    m_minPsl = unpackedPsl.psl;
                }
            }
        }

        if (searchResult.type == search_result_type::new_insertion)
        {
            rsl_assert_invalid_object(currentIndex < m_buckets.size());
            m_buckets[currentIndex] = insertBucket;
        }

        return result;
    }

    template <typename MapInfo>
    template <typename KeyType>
    constexpr const typename hash_map_base<MapInfo>::mapped_type* hash_map_base<MapInfo>::find_impl(const KeyType& key) const noexcept
        requires (MapInfo::is_map)
    {
        if (empty())
        {
            return nullptr;
        }

        const hash_result hash = get_hash_result(key);

        bucket_search_result searchResult = find_next_available(hash.homeIndex, m_minPsl, hash.fingerprint, key, true);

        if (searchResult.type != search_result_type::existing_item)
        {
            return nullptr;
        }

        return &m_values[m_buckets[hash.homeIndex + searchResult.unpackedPsl.psl].index].value();
    }

    template <typename MapInfo>
    template <typename KeyType>
    constexpr void hash_map_base<MapInfo>::erase_impl(const KeyType& key) noexcept
    {
        if (empty())
        {
            return;
        }

        const hash_result hash = get_hash_result(key);

        bucket_search_result searchResult = find_next_available(hash.homeIndex, m_minPsl, hash.fingerprint, key, true);

        if (searchResult.type != search_result_type::existing_item)
        {
            return;
        }

        size_type index = hash.homeIndex + searchResult.unpackedPsl.psl;
        size_type valueIndex = m_buckets[index].index;
        destroy_node(m_values[valueIndex]);
        m_values.erase_swap(valueIndex);
        m_buckets[m_lastValueBucketIndex].index = valueIndex;

        psl_type currentPsl = unpack_bucket_psl(m_buckets[index + 1]);

        while (currentPsl.psl != 0)
        {
            bucket_type& bucket = m_buckets[index + 1];
            storage_type newPsl = currentPsl.psl - 1;
            bucket.pslAndFingerprint = pack_bucket_psl(psl_type{ .psl = newPsl, .fingerprint = currentPsl.fingerprint });
            m_buckets[index] = bucket;

            if (newPsl < m_minPsl)
            {
                m_minPsl = newPsl;
            }

            ++index;
            currentPsl = unpack_bucket_psl(m_buckets[index + 1]);
        }

        m_maxPsl = 0;
        m_lastValueBucketIndex = 0;
        for (size_type i = 0; i < m_buckets.size(); ++i)
        {
            psl_type unpackedPsl = unpack_bucket_psl(m_buckets[i]);
            if (unpackedPsl.psl > m_maxPsl)
            {
                m_maxPsl = unpackedPsl.psl;
            }

            if (m_buckets[i].index > m_buckets[m_lastValueBucketIndex].index)
            {
                m_lastValueBucketIndex = i;
            }
        }

        m_buckets[index].pslAndFingerprint = 0;
    }

    template <typename MapInfo>
    void hash_map_base<MapInfo>::reserve(
            size_type newCapacity
            )
        noexcept(noexcept(declval<bucket_container>().reserve(0)) && noexcept(declval<value_container>().reserve(0)))
    {
        m_values.reserve(newCapacity);

        if constexpr (!is_flat)
        {
            m_memoryPool->reserve(newCapacity);
        }

        if (newCapacity > m_buckets.size())
        {
            bucket_container oldBuckets = move(m_buckets);
            m_buckets = bucket_container::create_in_place(newCapacity);

            rehash(oldBuckets);
        }
    }

    template <typename MapInfo>
    void hash_map_base<MapInfo>::clear() noexcept
    {
        for (node_type& node : m_values)
        {
            destroy_node(node);
        }

        m_values.clear();
        m_buckets.clear();

        if constexpr (!is_flat)
        {
            m_memoryPool->clear();
        }
    }

    template <typename MapInfo>
    template <typename... Args>
    typename hash_map_base<MapInfo>::mapped_type& hash_map_base<MapInfo>::emplace(const key_type& key, Args&&... args)
    {
        return try_emplace(key, rsl::forward<Args>(args)...).first;
    }

    template <typename MapInfo>
    template <typename... Args>
    typename hash_map_base<MapInfo>::mapped_type& hash_map_base<MapInfo>::emplace_or_replace(
            const key_type& key,
            Args&&... args
            )
    {
        insert_result insertResult = insert_key_internal(key, m_values.size());

        if (insertResult.type == insert_result_type::new_insertion)
        {
            return m_values.emplace_back(create_node(key, forward<Args>(args)...)).value();
        }

        mapped_type& value = m_values[m_buckets[insertResult.index].index].value();
        value = move(mapped_type(forward<Args>(args)...));
        return value;
    }

    template <typename MapInfo>
    template <typename... Args>
    pair<typename hash_map_base<MapInfo>::mapped_type&, bool> hash_map_base<MapInfo>::try_emplace(
            const key_type& key,
            Args&&... args
            )
    {
        insert_result insertResult = insert_key_internal(key, m_values.size());

        if (insertResult.type == insert_result_type::new_insertion)
        {
            return { rsl::ref(m_values.emplace_back(create_node(key, rsl::forward<Args>(args)...)).value()), true };
        }

        return { rsl::ref(m_values[m_buckets[insertResult.index].index].value()), false };
    }

    template <typename MapInfo>
    template <typename... Args>
    pair<typename hash_map_base<MapInfo>::mapped_type&, bool> hash_map_base<MapInfo>::try_emplace(key_type&& key, Args&&... args)
    {
        insert_result insertResult = insert_key_internal(key, m_values.size());

        if (insertResult.type == insert_result_type::new_insertion)
        {
            return { rsl::ref(m_values.emplace_back(create_node(rsl::move(key), rsl::forward<Args>(args)...)).value()), true };
        }

        return { rsl::ref(m_values[m_buckets[insertResult.index].index].value()), false };
    }

    template <typename MapInfo>
    bool hash_map_base<MapInfo>::contains(const key_type& key) const noexcept
    {
        if (empty())
        {
            return false;
        }

        const hash_result hash = get_hash_result(key);

        bucket_search_result searchResult = find_next_available(hash.homeIndex, m_minPsl, hash.fingerprint, key, true);

        return searchResult.type == search_result_type::existing_item;
    }

    template <typename MapInfo>
    bool hash_map_base<MapInfo>::contains(key_view_alternative key) const noexcept
        requires (has_key_view_alternative)
    {
        if (empty())
        {
            return false;
        }

        const hash_result hash = get_hash_result(key);

        bucket_search_result searchResult = find_next_available(hash.homeIndex, m_minPsl, hash.fingerprint, key, true);

        return searchResult.type == search_result_type::existing_item;
    }

    template <typename MapInfo>
    const typename hash_map_base<MapInfo>::mapped_type& hash_map_base<MapInfo>::at(const key_type& key) const
        requires (MapInfo::is_map)
    {
        const mapped_type* result = find(key);
        rsl_assert_invalid_access(result != nullptr);
        return *result;
    }

    template <typename MapInfo>
    typename hash_map_base<MapInfo>::mapped_type& hash_map_base<MapInfo>::at(const key_type& key)
        requires (MapInfo::is_map)
    {
        mapped_type* result = find(key);
        rsl_assert_invalid_access(result != nullptr);
        return *result;
    }

    template <typename MapInfo>
    const typename hash_map_base<MapInfo>::mapped_type& hash_map_base<MapInfo>::at(key_view_alternative key) const
        requires (MapInfo::is_map && has_key_view_alternative)
    {
        const mapped_type* result = find(key);
        rsl_assert_invalid_access(result != nullptr);
        return *result;
    }

    template <typename MapInfo>
    typename hash_map_base<MapInfo>::mapped_type& hash_map_base<MapInfo>::at(key_view_alternative key)
        requires (MapInfo::is_map && has_key_view_alternative)
    {
        mapped_type* result = find(key);
        rsl_assert_invalid_access(result != nullptr);
        return *result;
    }

    template <typename MapInfo>
    const typename hash_map_base<MapInfo>::mapped_type* hash_map_base<MapInfo>::find(const key_type& key) const noexcept
        requires (MapInfo::is_map)
    {
        return find_impl(key);
    }

    template <typename MapInfo>
    typename hash_map_base<MapInfo>::mapped_type* hash_map_base<MapInfo>::find(const key_type& key) noexcept
        requires (MapInfo::is_map)
    {
        return const_cast<mapped_type*>(rsl::as_const(*this).find(key));
    }

    template <typename MapInfo>
    const typename hash_map_base<MapInfo>::mapped_type* hash_map_base<MapInfo>::find(key_view_alternative key) const noexcept
        requires (MapInfo::is_map && has_key_view_alternative)
    {
        return find_impl(key);
    }

    template <typename MapInfo>
    typename hash_map_base<MapInfo>::mapped_type* hash_map_base<MapInfo>::find(key_view_alternative key) noexcept
        requires (MapInfo::is_map && has_key_view_alternative)
    {
        return const_cast<mapped_type*>(rsl::as_const(*this).find(key));
    }
}
