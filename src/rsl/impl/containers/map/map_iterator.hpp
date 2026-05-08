#pragma once
#include "../iterators.hpp"

namespace rsl
{
    struct move_to_next_tag_type
    {
    };

    [[maybe_unused]] constexpr move_to_next_tag_type move_to_next_tag{};

    template <typename MapInfo, weak_input_or_output_iterator NodeIter>
    class hash_map_iterator
    {
    public:
        static constexpr bool is_map = MapInfo::is_map;
        static constexpr bool is_set = MapInfo::is_set;
        static constexpr bool is_transparent = MapInfo::is_transparent;

        using key_type = typename MapInfo::key_type;
        using mapped_type = typename MapInfo::mapped_type;
        using value_type = typename MapInfo::value_type;
        using node_type = remove_reference_t<iter_reference_t<NodeIter>>;
        using node_iter = NodeIter;

        using difference_type = iter_difference_t<node_iter>;

        using ref_type = iter_reference_t<node_type>;
        using ptr_type = iter_pointer_t<node_type>;

        constexpr hash_map_iterator() noexcept = default;
        constexpr hash_map_iterator(nullptr_type) noexcept{};

        constexpr explicit hash_map_iterator(const node_iter& iter) noexcept : m_node(iter) {}
        constexpr explicit hash_map_iterator(node_iter&& iter) noexcept : m_node(rsl::move(iter)) {}

        template <weak_input_or_output_iterator OtherNodeIter>
        constexpr hash_map_iterator(const hash_map_iterator<MapInfo, OtherNodeIter>& other) noexcept
            requires constructible_from<node_iter, OtherNodeIter> && not_same_as<node_iter, OtherNodeIter>
            : m_node(other.m_node)
        {
        }

        template <weak_input_or_output_iterator OtherNodeIter>
        constexpr hash_map_iterator(hash_map_iterator<MapInfo, OtherNodeIter>&& other) noexcept
            requires constructible_from<node_iter, OtherNodeIter&&> && not_same_as<node_iter, OtherNodeIter>
            : m_node(rsl::move(other.m_node))
        {
        }

        template <weak_input_or_output_iterator OtherNodeIter>
        constexpr hash_map_iterator& operator=(const hash_map_iterator<MapInfo, OtherNodeIter>& other) noexcept
            requires assignable_from<node_iter, OtherNodeIter> && not_same_as<node_iter, OtherNodeIter>
        {
            m_node = other.m_node;
            return *this;
        }

        template <weak_input_or_output_iterator OtherNodeIter>
        constexpr hash_map_iterator& operator=(hash_map_iterator<MapInfo, OtherNodeIter>&& other) noexcept
            requires assignable_from<node_iter, OtherNodeIter&&> && not_same_as<node_iter, OtherNodeIter>
        {
            m_node = rsl::move(other.m_node);
            return *this;
        }

        constexpr hash_map_iterator& operator+=(const difference_type offset) noexcept
            requires random_access_iterator<node_iter>
        {
            m_node += offset;
            return *this;
        }

        constexpr hash_map_iterator operator+(const difference_type offset) const noexcept
            requires random_access_iterator<node_iter>
        {
            return reverse_iterator(m_node + offset);
        }

        constexpr hash_map_iterator& operator++() noexcept
            requires forward_iterator<node_iter>
        {
            ++m_node;
            return *this;
        }

        constexpr hash_map_iterator operator++(int) noexcept
            requires forward_iterator<node_iter>
        {
            hash_map_iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        constexpr hash_map_iterator& operator-=(const difference_type offset) noexcept
            requires random_access_iterator<node_iter>
        {
            m_node -= offset;
            return *this;
        }

        constexpr hash_map_iterator operator-(const difference_type offset) const noexcept
            requires random_access_iterator<node_iter>
        {
            return reverse_iterator(m_node - offset);
        }

        constexpr hash_map_iterator& operator--() noexcept
            requires bidirectional_iterator<node_iter>
        {
            --m_node;
            return *this;
        }

        constexpr hash_map_iterator operator--(int) noexcept
            requires bidirectional_iterator<node_iter>
        {
            hash_map_iterator tmp = *this;
            --(*this);
            return tmp;
        }

        constexpr ref_type operator[](const difference_type n) const noexcept
            requires random_access_iterator<node_iter>
        {
            return *m_node[n];
        }

        constexpr ref_type operator*() const noexcept { return **m_node; }
        constexpr ptr_type operator->() const noexcept { return &**m_node; }

        template <weak_input_or_output_iterator OtherNodeIter>
            requires sentinel_for<node_iter, OtherNodeIter>
        constexpr bool operator==(const hash_map_iterator<MapInfo, OtherNodeIter>& other) const noexcept
        {
            return m_node == other.m_node;
        }

        template <weak_input_or_output_iterator OtherNodeIter>
            requires sentinel_for<node_iter, OtherNodeIter>
        constexpr bool operator!=(const hash_map_iterator<MapInfo, OtherNodeIter>& other) const noexcept
        {
            return m_node != other.m_node;
        }

    private:
        node_iter m_node{nullptr};
    };
} // namespace rsl
