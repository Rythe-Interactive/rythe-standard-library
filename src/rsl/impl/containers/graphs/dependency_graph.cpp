#include "dependency_graph.hpp"

namespace rsl
{
    array_view<const dependency_graph_node> dependency_graph::root_nodes()
    {
        for (size_type i = 0ull; i < m_nodes.size(); ++i)
        {
            if (!m_nodes[i].dependencies.is_empty())
            {
                return m_nodes.subview(0ull, i);
            }
        }

        return {};
    }

    const dependency_graph_node* dependency_graph::find_node(const id_type id) const
    {
        struct node_searcher
        {
            id_type id;
            bool operator==(const graph_node& node) const noexcept { return node.id == id; }
        };
        const size_type index = linear_search(m_nodes.view(), node_searcher{ .id = id });

        if (index == npos)
        {
            return nullptr;
        }

        return &m_nodes[index];
    }

    bool dependency_graph_builder::add_node(
            dependency_graph_builder_node&& node
            )
        noexcept
    {
        return m_nodes.try_emplace(node.id, rsl::move(node)).second;
    }

    bool dependency_graph_builder::add_node(
            const dependency_graph_builder_node& node
            )
        noexcept
    {
        return m_nodes.try_emplace(node.id, node).second;
    }

    bool dependency_graph_builder::add_dependency(const id_type nodeId, builder_dependency dep)
    {
        auto* node = m_nodes.find(nodeId);
        if (!node)
        {
            return false;
        }

        // TODO: Validation might not always be needed.
        // TODO: Master/release builds could just ignore the issue as long as they are caught in debug.
        struct dependency_searcher
        {
            id_type dependencyId;
            bool operator==(const builder_dependency& dep) const noexcept { return dep.id == dependencyId; }
        };

        if (linear_search(node->dependencies.view(), dependency_searcher{ .dependencyId = dep.id }) != npos)
        {
            return false;
        }

        node->dependencies.emplace_back(dep);
        return true;
    }

    result<dependency_graph> dependency_graph_builder::build()
    {
        dynamic_array<const builder_node*> sortedNodes;
        for (const auto& [id, node] : m_nodes)
        {
            sortedNodes.push_back(&node);
        }

        hybrid_sort(
                sortedNodes,
                [](const builder_node* lhs, const builder_node* rhs)
                {
                    return lhs->dependencies.size() < rhs->dependencies.size();
                }
                );

        if (!sortedNodes.front()->dependencies.is_empty())
        {
            return make_error(graph_error::no_root);
        }

        struct node_info
        {
            index_type index;
            dynamic_array<builder_dependency> dependents;
        };
        dynamic_map<id_type, node_info> infoMap;
        dependency_graph result;
        for (const auto* node : sortedNodes)
        {
            infoMap.try_emplace(node->id).first.index = result.m_nodes.size();

            result.m_nodes.emplace_back(dependency_graph_node{ .id = node->id, .dependencies{}, .dependents = {} });

            for (const builder_dependency& dep : node->dependencies)
            {
                infoMap.try_emplace(dep.id).first.dependents.emplace_back(builder_dependency{ .id = node->id, .type = dep.type });
            }
        }

        for (const auto* node : sortedNodes)
        {
            const auto& [index, dependents] = infoMap.at(node->id);

            const size_type dependentsOffset = result.m_relationships.size();
            for (const builder_dependency& dependent : dependents)
            {
                result.m_relationships.emplace_back(
                        dependency{
                            .node = &result.m_nodes[infoMap.at(dependent.id).index],
                            .type = dependent.type,
                        }
                        );
            }

            const size_type dependenciesOffset = result.m_relationships.size();
            for (const builder_dependency& dep : node->dependencies)
            {
                result.m_relationships.emplace_back(
                        dependency{
                            .node = &result.m_nodes[infoMap.at(dep.id).index],
                            .type = dep.type,
                        }
                        );
            }

            auto& resultNode = result.m_nodes[index];
            resultNode.dependents = result.m_relationships.subview(dependentsOffset, dependents.size());
            resultNode.dependencies = result.m_relationships.subview(dependenciesOffset, node->dependencies.size());
        }

        return result;
    }
}
