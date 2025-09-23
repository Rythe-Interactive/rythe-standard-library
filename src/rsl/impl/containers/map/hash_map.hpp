#pragma once

#include "../any.hpp"
#include "../array.hpp"
#include "../optional.hpp"
#include "../pair.hpp"
#include "../reference_wrapper.hpp"
#include "../views.hpp"
#include "../../memory/memory_pool.hpp"
#include "../util/comparers.hpp"

#include "map_iterator.hpp"
#include "map_node.hpp"

namespace rsl
{
	// Default implementation of a hash table using robin hood hashing
	template <typename MapInfo>
	class hash_map_base
	{
	public:
		static constexpr bool is_flat = MapInfo::is_flat;
		static constexpr bool is_large = MapInfo::is_large;

		using key_type = typename MapInfo::key_type;
		using mapped_type = typename MapInfo::mapped_type;
		using hasher_type = typename MapInfo::hasher_type;
		using key_comparer_type = typename MapInfo::key_comparer_type;

		static constexpr bool is_map = MapInfo::is_map;
		static constexpr bool is_set = MapInfo::is_set;
		static constexpr bool is_transparent = MapInfo::is_transparent;

		static constexpr float32 max_load_factor = MapInfo::max_load_factor;

		using value_type = typename MapInfo::value_type;
		using node_type = internal::map_node<MapInfo>;
		using bucket_type = typename MapInfo::bucket_type;
		using psl_type = typename MapInfo::psl_type;
		using storage_type = typename MapInfo::storage_type;

		using allocator_t = typename MapInfo::allocator_t;
		using allocator_storage_type = allocator_storage<allocator_t>;

		using factory_t = typename MapInfo::template factory_t<mapped_type>;
		using factory_storage_type = factory_storage<factory_t>;

		using bucket_factory_t = typename MapInfo::template factory_t<bucket_type>;
		using node_factory_t = typename MapInfo::template factory_t<node_type>;

	    constexpr static bool view_hash_identical = true;

	private:
		using data_pool = conditional_storage<!is_flat, memory_pool<value_type, allocator_t>>;
		using value_container = dynamic_array<node_type, allocator_t, node_factory_t>;
		using bucket_container = dynamic_array<bucket_type, allocator_t, bucket_factory_t>;

		constexpr static bool nothrow_constructible_alloc =
			is_nothrow_constructible_v<data_pool, const allocator_storage_type&> &&
			is_nothrow_constructible_v<value_container, const allocator_storage_type&> &&
			is_nothrow_constructible_v<bucket_container, const allocator_storage_type&>;

		constexpr static bool nothrow_constructible_fact =
			is_nothrow_constructible_v<value_container, const factory_storage_type&> &&
			is_nothrow_constructible_v<bucket_container, const factory_storage<bucket_factory_t>&>;

		constexpr static bool nothrow_constructible_alloc_fact =
			nothrow_constructible_alloc && nothrow_constructible_fact;

	    using key_view_alternative = MapInfo::key_view_alternative;
	    constexpr static bool has_key_view_alternative = MapInfo::has_key_view_alternative;

	public:
		using iterator_type = hash_map_iterator<hash_map_base, typename value_container::iterator_type>;
		using const_iterator_type = hash_map_iterator<hash_map_base, typename value_container::const_iterator_type>;
		using reverse_iterator_type = hash_map_iterator<hash_map_base, typename value_container::reverse_iterator_type>;
		using const_reverse_iterator_type = hash_map_iterator<hash_map_base, typename value_container::const_reverse_iterator_type>;

		using view_type = iterator_view<iterator_type, const_iterator_type>;
		using const_view_type = typename view_type::const_view_type;

		[[rythe_always_inline]] constexpr hash_map_base() noexcept(MapInfo::nothrow_constructible);

		[[rythe_always_inline]] constexpr hash_map_base(const hasher_type& h, const key_comparer_type& equal)
			noexcept(MapInfo::nothrow_copy_constructible);

		[[rythe_always_inline]] explicit constexpr hash_map_base(const hasher_type& h)
			noexcept(MapInfo::nothrow_hasher_copy_constructible);

		[[rythe_always_inline]] explicit constexpr hash_map_base(const key_comparer_type& equal)
			noexcept(MapInfo::nothrow_comparer_copy_constructible);

		[[rythe_always_inline]] explicit constexpr hash_map_base(const allocator_storage_type& allocStorage)
			noexcept(nothrow_constructible_alloc);

		[[rythe_always_inline]] explicit constexpr hash_map_base(const factory_storage_type& factoryStorage)
			noexcept(nothrow_constructible_fact);

		[[rythe_always_inline]] constexpr hash_map_base(
			const allocator_storage_type& allocStorage,
			const factory_storage_type& factoryStorage
		) noexcept(nothrow_constructible_alloc_fact);

		template <typename Iter, typename ConstIter>
		[[rythe_always_inline]] constexpr static hash_map_base from_view(iterator_view<Iter, ConstIter> src);

		template <typename Iter, typename ConstIter>
		[[rythe_always_inline]] constexpr static hash_map_base move_from_view(iterator_view<Iter, ConstIter> src);

		[[rythe_always_inline]] constexpr static hash_map_base create_reserved(size_type capacity)
			noexcept(noexcept(declval<hash_map_base>().reserve(0)));

		[[nodiscard]] [[rythe_always_inline]] constexpr size_type size() const noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr bool empty() const noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr size_type capacity() const noexcept;

		[[nodiscard]] [[rythe_always_inline]] constexpr const_view_type view() const noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr view_type view() noexcept;

		void reserve(size_type newCapacity)
			noexcept(noexcept(declval<bucket_container>().reserve(0)) && noexcept(declval<value_container>().reserve(0))
			);

		void clear() noexcept;

		[[nodiscard]] [[rythe_always_inline]] bool contains(const key_type& key) const noexcept;
		[[nodiscard]] [[rythe_always_inline]] bool contains(key_view_alternative key) const noexcept requires(has_key_view_alternative);

		[[nodiscard]] [[rythe_always_inline]] const mapped_type* find(const key_type& key) const noexcept
			requires (MapInfo::is_map);
		[[nodiscard]] [[rythe_always_inline]] mapped_type* find(const key_type& key) noexcept
	        requires (MapInfo::is_map);
	    [[nodiscard]] [[rythe_always_inline]] const mapped_type* find(key_view_alternative key) const noexcept
            requires (MapInfo::is_map && has_key_view_alternative);
	    [[nodiscard]] [[rythe_always_inline]] mapped_type* find(key_view_alternative key) noexcept
            requires (MapInfo::is_map && has_key_view_alternative);

		[[nodiscard]] [[rythe_always_inline]] const mapped_type& at(const key_type& key) const
			requires (MapInfo::is_map);
		[[nodiscard]] [[rythe_always_inline]] mapped_type& at(const key_type& key)
	        requires (MapInfo::is_map);
	    [[nodiscard]] [[rythe_always_inline]] const mapped_type& at(key_view_alternative key) const
            requires (MapInfo::is_map && has_key_view_alternative);
	    [[nodiscard]] [[rythe_always_inline]] mapped_type& at(key_view_alternative key)
            requires (MapInfo::is_map && has_key_view_alternative);

		template <typename... Args>
		mapped_type& emplace(const key_type& key, Args&&... args);

		template <typename... Args>
		mapped_type& emplace_or_replace(const key_type& key, Args&&... args);

		template <typename... Args>
	    pair<mapped_type&, bool> try_emplace(const key_type& key, Args&&... args);
	    template <typename... Args>
        pair<mapped_type&, bool> try_emplace(key_type&& key, Args&&... args);

		[[rythe_always_inline]] constexpr void erase(const key_type& key) noexcept;
		[[rythe_always_inline]] constexpr void erase(key_view_alternative key) noexcept requires(has_key_view_alternative);

		[[nodiscard]] [[rythe_always_inline]] constexpr memory_pool<value_type>& get_memory_pool() noexcept
			requires(!is_flat);
		[[nodiscard]] [[rythe_always_inline]] constexpr const memory_pool<value_type>& get_memory_pool() const noexcept
			requires(!is_flat);

		[[nodiscard]] [[rythe_always_inline]] constexpr allocator_t& get_allocator() noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const allocator_t& get_allocator() const noexcept;

		[[nodiscard]] [[rythe_always_inline]] constexpr factory_t& get_factory() noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const factory_t& get_factory() const noexcept;

		[[nodiscard]] [[rythe_always_inline]] constexpr iterator_type begin() noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const_iterator_type begin() const noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const_iterator_type cbegin() const noexcept;

		[[nodiscard]] [[rythe_always_inline]] constexpr iterator_type end() noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const_iterator_type end() const noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const_iterator_type cend() const noexcept;

		[[nodiscard]] [[rythe_always_inline]] constexpr reverse_iterator_type rbegin() noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const_reverse_iterator_type rbegin() const noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const_reverse_iterator_type crbegin() const noexcept;

		[[nodiscard]] [[rythe_always_inline]] constexpr reverse_iterator_type rend() noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const_reverse_iterator_type rend() const noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const_reverse_iterator_type crend() const noexcept;

	protected:
		template <typename... Args>
	    [[nodiscard]] [[rythe_always_inline]] constexpr node_type create_node(const key_type& key, Args&&... args);
	    template <typename... Args>
        [[nodiscard]] [[rythe_always_inline]] constexpr node_type create_node(key_type&& key, Args&&... args);

		[[rythe_always_inline]] constexpr void destroy_node(node_type& node) noexcept;

		constexpr void rehash(const bucket_container& oldBuckets) noexcept;

		[[rythe_always_inline]] constexpr void maybe_grow() noexcept(noexcept(reserve(0)));

		[[rythe_always_inline]] constexpr static storage_type pack_bucket_psl(const psl_type& unpackedPsl) noexcept;
		[[rythe_always_inline]] constexpr static psl_type unpack_bucket_psl(const bucket_type& bucket) noexcept;

		struct hash_result
		{
			storage_type fingerprint;
			index_type homeIndex;
		};

	    template<typename KeyType>
		[[rythe_always_inline]] constexpr hash_result get_hash_result(const KeyType& key) const noexcept;

		enum struct [[rythe_closed_enum]] search_result_type : uint8
		{
			new_insertion,
			swap,
			existing_item,
			item_not_found,
		};

		struct bucket_search_result
		{
			psl_type unpackedPsl;
			search_result_type type;
		};

	    template<typename KeyType>
		constexpr bucket_search_result find_next_available(
			index_type homeIndex, storage_type startPsl, storage_type fingerprint, const KeyType& key, bool earlyOut
		) const noexcept;

		enum struct [[rythe_closed_enum]] insert_result_type : uint8
		{
			new_insertion,
			existing_item,
		};

		struct insert_result
		{
			index_type index;
			insert_result_type type;
		};

		constexpr insert_result insert_key_internal(
			const key_type& key, index_type valueIndexHint
		) noexcept(noexcept(reserve(0)));

	private:
	    template<typename KeyType>
        constexpr const mapped_type* find_impl(const KeyType& key) const noexcept requires (MapInfo::is_map);
	    template<typename KeyType>
        constexpr void erase_impl(const KeyType& key) noexcept;

		value_container m_values;
		bucket_container m_buckets;

		size_type m_lastValueBucketIndex;

		// Sacrifice rehash, erase, and add for faster lookups.
		storage_type m_minPsl;
		storage_type m_maxPsl;

		hasher_type m_hasher;
		key_comparer_type m_keyComparer;
		allocator_storage_type m_alloc;
		factory_storage_type m_factory;
		data_pool m_memoryPool;
	};
} // namespace rsl

#include "hash_map.inl"
