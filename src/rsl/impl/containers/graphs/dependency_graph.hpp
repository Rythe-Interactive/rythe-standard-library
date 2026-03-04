#pragma once
#include "graph_error.hpp"
#include "../array.hpp"
// ReSharper disable once CppUnusedIncludeDirective
#include "../util/sort.hpp" // used in .inl

namespace rsl
{
    enum struct [[rythe_closed_enum]] dependency_type : uint8
    {
        shared,
        exclusive
    };

    struct builder_dependency
    {
        id_type id;
        dependency_type type;
    };

    struct dependency_graph_builder_node
    {
        id_type id;
        dynamic_array<builder_dependency> dependencies;
    };

    struct dependency_graph_node;

    struct dependency
    {
        dependency_graph_node* node;
        dependency_type type;
    };

    struct dependency_graph_node
    {
        id_type id;
        array_view<const dependency> dependencies;
        array_view<const dependency> dependents;
    };

    class dependency_graph_builder;

    class dependency_graph
    {
        friend class dependency_graph_builder;
    public:
        using graph_node = dependency_graph_node;

        array_view<const graph_node> root_nodes();

        const graph_node* find_node(id_type id) const;

        template<typename Visitor>
        void visit_children_first(Visitor&& visitor);

    private:
        dynamic_array<graph_node> m_nodes;
        dynamic_array<dependency> m_relationships;
    };

    class dependency_graph_builder final
    {
    public:
        using builder_node = dependency_graph_builder_node;
        using graph = dependency_graph;

        bool add_node(builder_node&& node) noexcept;
        bool add_node(const builder_node& node) noexcept;

        bool add_dependency(id_type nodeId, builder_dependency dep);

        result<dependency_graph> build();

    private:
        dynamic_map<id_type, builder_node> m_nodes;
    };
}

#include "dependency_graph.inl"
