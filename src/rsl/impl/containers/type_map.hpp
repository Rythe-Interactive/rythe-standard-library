#pragma once
#include "type_map.hpp"
#include "../memory/allocator.hpp"
#include "../util/assert.hpp"
#include "../util/common.hpp"
#include "../util/type_traits.hpp"
#include "map/dynamic_map.hpp"

namespace rsl
{
	template <
		allocator_type Alloc = default_allocator, factory_type Factory = default_factory<void>>
	class basic_type_map
	{
	private:
		struct entry_item;

	public:
		using allocator_storage_type = allocator_storage<Alloc>;
		using allocator_t = Alloc;
		using factory_storage_type = factory_storage<Factory>;
		using factory_t = Factory;
		template <typename T>
		using alloc_type = typed_allocator<T, Alloc, typename Factory::template retarget<T>>;

		constexpr basic_type_map() = default;

		explicit constexpr basic_type_map(const allocator_storage_type& allocStorage)
			noexcept(is_nothrow_copy_constructible_v<allocator_storage_type>)
			: m_allocator(allocStorage),
			  m_storage(allocStorage) {}

		[[nodiscard]] [[rythe_always_inline]] constexpr size_type size() const noexcept { return m_storage.size(); }
		[[nodiscard]] [[rythe_always_inline]] constexpr bool empty() const noexcept { return m_storage.empty(); }

		template <typename T>
		[[nodiscard]] [[rythe_always_inline]] constexpr bool has() const noexcept
		{
			constexpr id_type typeHash = type_id<T>();
			return m_storage.contains(typeHash);
		}

		template <typename T>
		[[nodiscard]] [[rythe_always_inline]] constexpr const T* try_get() const noexcept
		{
			constexpr id_type typeHash = type_id<T>();
			if (const entry_item* entry = m_storage.find(typeHash); entry != nullptr)
			{
				return entry->template cast<const T>();
			}

			return nullptr;
		}

		template <typename T>
		[[nodiscard]] [[rythe_always_inline]] constexpr T* try_get() noexcept
		{
			return const_cast<T*>(as_const(*this).template try_get<T>());
		}

		template <typename T>
		[[nodiscard]] [[rythe_always_inline]] constexpr const T& get() const
		{
			auto* ptr = try_get<T>();
			rsl_assert_invalid_object(ptr);
			return *ptr;
		}

		template <typename T>
		[[nodiscard]] [[rythe_always_inline]] constexpr T& get()
		{
			return const_cast<T&>(as_const(*this).template get<T>());
		}

		template <typename T, typename... Args>
		[[rythe_always_inline]] constexpr std::pair<T&, bool> try_emplace(Args&&... args) noexcept
		{
			constexpr id_type typeHash = type_id<T>();
			auto result = m_storage.try_emplace(typeHash);
			if (result.second)
			{
				result.first.template construct<T>(this, forward<Args>(args)...);
			}

			return { ref(*(result.first.template cast<T>())), result.second };
		}

		template <typename T, typename... Args>
		[[rythe_always_inline]] constexpr T& emplace(Args&&... args)
		{
			return try_emplace<T>(forward<Args>(args)...).first;
		}

		template <typename T, typename... Args>
		[[nodiscard]] [[rythe_always_inline]] constexpr T& get_or_emplace(Args&&... args) noexcept
		{
			if (T* ptr = try_get<T>(); ptr != nullptr)
			{
				return *ptr;
			}

			return emplace<T>(forward<Args>(args)...);
		}

		template <typename T, typename... Args>
		[[rythe_always_inline]] constexpr T& emplace_or_replace(Args&&... args) noexcept
		{
			erase<T>();
			return emplace<T>(forward<Args>(args)...);
		}

		template <typename T>
		[[rythe_always_inline]] constexpr void erase() noexcept
		{
			constexpr id_type typeHash = type_id<T>();
			m_storage.erase(typeHash);
		}

		[[rythe_always_inline]] constexpr void clear() noexcept { m_storage.clear(); }

		[[rythe_always_inline]] constexpr allocator_t& get_allocator() noexcept { return m_allocator.get_allocator(); }

		[[rythe_always_inline]] constexpr const allocator_t& get_allocator() const noexcept
		{
			return m_allocator.get_allocator();
		}

	private:
		template <typename T>
		static void deallocate_entry(basic_type_map& map, entry_item& entry)
		{
			alloc_type<T>(map.m_allocator).destroy_and_deallocate(entry.template cast<T>());
			entry.data = nullptr;
			entry.map = nullptr;
			entry.deallocateEntry = nullptr;
		}

		struct entry_item
		{
			using deallocate_func = void (*)(basic_type_map&, entry_item&);
			void* data = nullptr;
			basic_type_map* map = nullptr;
			deallocate_func deallocateEntry = nullptr;

			[[rythe_always_inline]] constexpr entry_item() noexcept = default;

			[[rythe_always_inline]] constexpr entry_item(entry_item&& other) noexcept
				: data(other.data),
				  map(other.map),
				  deallocateEntry(other.deallocateEntry)
			{
				other.data = nullptr;
				other.map = nullptr;
				other.deallocateEntry = nullptr;
			}

			[[rythe_always_inline]] constexpr entry_item& operator=(entry_item&& other) noexcept
			{
				data = other.data;
				map = other.map;
				deallocateEntry = other.deallocateEntry;
				other.data = nullptr;
				other.map = nullptr;
				other.deallocateEntry = nullptr;
				return *this;
			}

			template <typename T, typename... Args>
			void construct(basic_type_map* mapPtr, Args&&... args)
			{
				data = alloc_type<T>(mapPtr->m_allocator).allocate_and_construct(1, forward<Args>(args)...);
				map = mapPtr;
				deallocateEntry = &basic_type_map::deallocate_entry<T>;
			}

			template <typename T>
			T* cast() const
			{
				return static_cast<T*>(data);
			}

			~entry_item()
			{
				if (data)
				{
					rsl_assert_rarely(map);
					(*deallocateEntry)(*map, *this);
				}
			}
		};

		alloc_type<pair<id_type, entry_item>> m_allocator;
		dynamic_map<id_type, entry_item, hash_map_flags::default_flags, allocator_t> m_storage;
	};

	using type_map = basic_type_map<>;

	using pmu_alloc_type_map = basic_type_map<polymorphic_allocator>;
} // namespace rsl
