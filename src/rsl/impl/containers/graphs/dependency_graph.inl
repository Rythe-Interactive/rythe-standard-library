#pragma once

namespace  rsl
{
    namespace internal
    {
        template <typename Visitor>
        void recursive_visitor(dependency_graph_node& node, Visitor&& visitor)
        {
            for (auto& dependency : node.dependencies)
            {
                recursive_visitor(dependency, rsl::forward<Visitor>(visitor));
            }

            visitor(node);
        }
    }

    template <typename Visitor>
    void dependency_graph::visit_children_first(Visitor&& visitor)
    {
        for (auto& root : root_nodes())
        {
            recursive_visitor(root, visitor);
        }
    }
}
