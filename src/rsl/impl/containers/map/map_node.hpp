#pragma once

namespace rsl::internal
{
    template <typename MapInfo>
    class hash_map_node
    {
    public:
        static constexpr bool is_map = MapInfo::is_map;
        static constexpr bool is_set = MapInfo::is_set;
        static constexpr bool is_transparent = MapInfo::is_transparent;

        using key_type = typename MapInfo::key_type;
        using mapped_type = typename MapInfo::mapped_type;
        using value_type = typename MapInfo::value_type;

        explicit constexpr hash_map_node(value_type* data) noexcept
            : m_data(data) {}

        constexpr value_type* get_ptr() noexcept { return m_data; }
        constexpr void set_ptr(value_type* data) noexcept { m_data = data; }

        hash_map_node(hash_map_node&& other) noexcept // NOLINT(cppcoreguidelines*)
            : m_data(other.m_data) {}

        ~hash_map_node()
        {
            rsl_assert_invalid_operation(m_data == nullptr);
        }

        value_type* operator->() noexcept { return m_data; }
        const value_type* operator->() const noexcept { return m_data; }

        value_type& operator*() { return *m_data; }
        const value_type& operator*() const { return *m_data; }

        [[nodiscard]] key_type& key() noexcept
            requires is_map
        {
            return m_data->first;
        }

        [[nodiscard]] const key_type& key() const noexcept
            requires is_map
        {
            return m_data->first;
        }

        [[nodiscard]] value_type& key() noexcept
            requires is_set
        {
            return *m_data;
        }

        [[nodiscard]] const value_type& key() const noexcept
            requires is_set
        {
            return *m_data;
        }

        [[nodiscard]] mapped_type& value() noexcept
            requires is_map
        {
            return m_data->second;
        }

        [[nodiscard]] const mapped_type& value() const noexcept
            requires is_map
        {
            return m_data->second;
        }

    private:
        value_type* m_data;
    };

    template <typename MapInfo>
    class flat_hash_map_node
    {
    public:
        static constexpr bool is_map = MapInfo::is_map;
        static constexpr bool is_set = MapInfo::is_set;
        static constexpr bool is_transparent = MapInfo::is_transparent;

        using key_type = typename MapInfo::key_type;
        using mapped_type = typename MapInfo::mapped_type;
        using value_type = typename MapInfo::value_type;

        template <typename... Args>
        explicit flat_hash_map_node(Args&&... args) // NOLINT(cppcoreguidelines*)
            noexcept(is_nothrow_constructible_v<value_type, Args...>)
            : m_data(rsl::forward<Args>(args)...) {}

        flat_hash_map_node(flat_hash_map_node&& other) // NOLINT(cppcoreguidelines*)
            noexcept(is_nothrow_move_constructible_v<value_type>)
            : m_data(rsl::move(other.m_data)) {}

        value_type* operator->() noexcept { return &m_data; }
        const value_type* operator->() const noexcept { return &m_data; }

        value_type& operator*() noexcept { return m_data; }
        const value_type& operator*() const noexcept { return m_data; }

        [[nodiscard]] key_type& key() noexcept
            requires is_map
        {
            return m_data.first;
        }

        [[nodiscard]] const key_type& key() const noexcept
            requires is_map
        {
            return m_data.first;
        }

        [[nodiscard]] value_type& key() noexcept
            requires is_set
        {
            return m_data;
        }

        [[nodiscard]] const value_type& key() const noexcept
            requires is_set
        {
            return m_data;
        }

        [[nodiscard]] mapped_type& value() noexcept
            requires is_map
        {
            return m_data.second;
        }

        [[nodiscard]] const mapped_type& value() const noexcept
            requires is_map
        {
            return m_data.second;
        }

    private:
        value_type m_data;
    };

    template <typename MapInfo, bool IsFlat = false>
    struct select_node_type
    {
        using type = hash_map_node<MapInfo>;
    };

    template <typename MapInfo>
    struct select_node_type<MapInfo, true>
    {
        using type = flat_hash_map_node<MapInfo>;
    };

    template <typename MapInfo>
    using map_node = typename select_node_type<MapInfo, MapInfo::is_flat>::type;


} // namespace rsl::internal
