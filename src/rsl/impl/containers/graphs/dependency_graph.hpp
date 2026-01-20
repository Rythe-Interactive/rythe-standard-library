#pragma once
#include "../array.hpp"
#include "../../math/util/limits.hpp"
#include "../../util/error_handling.hpp"
// ReSharper disable once CppUnusedIncludeDirective
#include "../util/sort.hpp" // used in .inl

namespace rsl
{
    enum struct dependency_type : uint8
    {
        shared,
        exclusive
    };

    struct dependency
    {
        id_type id;
        dependency_type type;
    };

    template <typename PayloadType>
    struct dependency_graph_builder_node
    {
        id_type id;
        PayloadType payload;
        dynamic_array<dependency> dependencies;
    };

    //DECLARE_OPAQUE_HANDLE_UNDERLYING_TYPE_INVALID_VALUE(dependency_id, size_type, math::limits<size_type>::max);
    enum struct [[rythe_open_enum]] dependency_id : size_type
    {
        invalid = rsl::math::limits<size_type>::max
    };
    [[maybe_unused]] constexpr dependency_id invalid_dependency_id = dependency_id::invalid;

    template <typename PayloadType>
    struct dependency_graph_node
    {
        PayloadType payload;
        array_view<const dependency> dependencies;
        array_view<const dependency> dependents;
    };

    template <typename PayloadType>
    class dependency_graph;

    template <typename PayloadType>
    class dependency_graph_builder
    {
    public:
        using builder_node = dependency_graph_builder_node<PayloadType>;
        using graph = dependency_graph<PayloadType>;

        void add_node(builder_node&& node) noexcept(nothrow_move_constructible<PayloadType>);
        void add_node(const builder_node& node) noexcept(nothrow_copy_constructible<PayloadType>);

        void add_dependency(dependency_id nodeId, dependency_id dependency);

        result<graph> build();

    private:
        dynamic_map<dependency_id, builder_node> m_nodes;
    };

    template <typename PayloadType>
    class dependency_graph
    {
        friend class dependency_graph_builder;
    public:
        using graph_node = dependency_graph_node<PayloadType>;

        array_view<const graph_node> root_nodes();

        const graph_node& get_node(dependency_id id) const;

        template<typename Visitor>
        void visit_depth_first(Visitor&& visitor);

    private:
        dynamic_array<graph_node> m_nodes;
        dynamic_array<dependency> m_relationships;
    };
}

#include "dependency_graph.inl"
