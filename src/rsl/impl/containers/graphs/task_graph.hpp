#pragma once
#include "../array.hpp"

namespace rsl
{
    // enum struct dependency_type : uint8
    // {
    //     shared,
    //     exclusive
    // };
    //
    // struct dependency
    // {
    //     id_type id;
    //     dependency_type type;
    // };
    //
    // template <typename PayloadType>
    // struct dependency_graph_builder_node
    // {
    //     id_type id;
    //     PayloadType payload;
    //     dynamic_array<dependency> dependencies;
    // };
    //
    // DECLARE_OPAQUE_HANDLE_UNDERLYING_TYPE_INVALID_VALUE(dependency_id, size_type, math::limits<size_type>::max);
    //
    // template <typename PayloadType>
    // struct dependency_graph_node
    // {
    //     PayloadType payload;
    //     array_view<const dependency_id> dependencies;
    //     array_view<const dependency_id> dependents;
    // };
    //
    // template <typename PayloadType>
    // class dependency_graph;
    //
    // template <typename PayloadType>
    // class dependency_graph_builder
    // {
    // public:
    //     using builder_node = dependency_graph_builder_node<PayloadType>;
    //     using graph = dependency_graph<PayloadType>;
    //
    //     void add_node(builder_node&& node) noexcept(nothrow_move_constructible<PayloadType>);
    //     void add_node(const builder_node& node) noexcept(nothrow_copy_constructible<PayloadType>);
    //
    //     graph build();
    //
    // private:
    //     dynamic_array<builder_node> m_nodes;
    // };
    //
    // template <typename PayloadType>
    // class dependency_graph
    // {
    // public:
    //     using graph_node = dependency_graph_node<PayloadType>;
    //
    //     array_view<const graph_node> root_nodes();
    //
    //     const graph_node& get_node(dependency_id id) const;
    //
    //     template<typename Visitor>
    //     void visit_depth_first(Visitor&& visitor);
    //
    // private:
    //     dynamic_array<graph_node> m_nodes;
    //     dynamic_array<dependency_id> m_relationships;
    // };
}
