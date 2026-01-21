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

    template <typename PayloadType>
    struct dependency_graph_builder_node
    {
        id_type id;
        PayloadType payload;
        dynamic_array<builder_dependency> dependencies;
    };

    template <typename PayloadType>
    struct dependency_graph_node;

    template <typename PayloadType>
    struct dependency
    {
        dependency_graph_node<PayloadType>* node;
        dependency_type type;
    };

    template <typename PayloadType>
    struct dependency_graph_node
    {
        id_type id;
        PayloadType payload;
        array_view<const dependency<PayloadType>> dependencies;
        array_view<const dependency<PayloadType>> dependents;
    };

    template <typename PayloadType>
    class dependency_graph;

    template <typename PayloadType>
    class dependency_graph_builder
    {
    public:
        using builder_node = dependency_graph_builder_node<PayloadType>;
        using graph = dependency_graph<PayloadType>;

        bool add_node(builder_node&& node) noexcept(nothrow_move_constructible<PayloadType>);
        bool add_node(const builder_node& node) noexcept(nothrow_copy_constructible<PayloadType>);

        bool add_dependency(id_type nodeId, builder_dependency dep);

        result<graph> build();

    private:
        dynamic_map<id_type, builder_node> m_nodes;
    };

    template <typename PayloadType>
    class dependency_graph
    {
        friend class dependency_graph_builder<PayloadType>;
    public:
        using graph_node = dependency_graph_node<PayloadType>;

        array_view<const graph_node> root_nodes();

        const graph_node* find_node(id_type id) const;

        template<typename Visitor>
        void visit_children_first(Visitor&& visitor);

    private:
        dynamic_array<graph_node> m_nodes;
        dynamic_array<dependency<PayloadType>> m_relationships;
    };
}

#include "dependency_graph.inl"
