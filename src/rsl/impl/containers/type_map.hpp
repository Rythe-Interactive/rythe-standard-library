#pragma once

#include "../memory/allocator.hpp"
#include "../util/assert.hpp"
#include "../util/common.hpp"

#include "map/dynamic_map.hpp"

namespace rsl
{
    class type_map
    {
    private:
        struct entry_item;

    public:
        template <typename T>
        using alloc_type = typed_allocator<T>;

        constexpr type_map() noexcept
            : type_map(allocator_context::globalAllocator)
        {}

        explicit constexpr type_map(allocator_storage allocator) noexcept
            : m_allocator(allocator),
              m_storage(allocator) {}

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
        [[rythe_always_inline]] constexpr pair<T&, bool> try_emplace(Args&&... args) noexcept
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

        [[rythe_always_inline]] constexpr allocator_storage get_allocator() const noexcept { return m_allocator.get_allocator(); }

    private:
        template <typename T>
        static void deallocate_entry(type_map& map, entry_item& entry)
        {
            alloc_type<T>(map.m_allocator.get_allocator()).destroy_and_deallocate(entry.template cast<T>());
            entry.data = nullptr;
            entry.map = nullptr;
            entry.deallocateEntry = nullptr;
        }

        struct entry_item
        {
            using deallocate_func = void (*)(type_map&, entry_item&);
            void* data = nullptr;
            type_map* map = nullptr;
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
            void construct(type_map* mapPtr, Args&&... args)
            {
                data = alloc_type<T>(mapPtr->m_allocator.get_allocator()).allocate_and_construct(1, forward<Args>(args)...);
                map = mapPtr;
                deallocateEntry = &type_map::deallocate_entry<T>;
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
        dynamic_map<id_type, entry_item> m_storage;
    };
} // namespace rsl
