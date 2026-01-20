#pragma once

namespace rsl
{
    template <typename PayloadType>
    void dependency_graph_builder<PayloadType>::add_node(dependency_graph_builder_node<PayloadType>&& node)
        noexcept(nothrow_move_constructible<PayloadType>)
    {
        //todo
    }

    template <typename PayloadType>
    void dependency_graph_builder<PayloadType>::add_node(const dependency_graph_builder_node<PayloadType>& node)
        noexcept(nothrow_copy_constructible<PayloadType>)
    {
        //todo
    }

    template <typename PayloadType>
    void dependency_graph_builder<PayloadType>::add_dependency(dependency_id nodeId, dependency_id dependency)
    {
        //todo
    }

    template <typename PayloadType>
    result<dependency_graph<PayloadType>> dependency_graph_builder<PayloadType>::build()
    {
        dynamic_array<const builder_node*> sortedNodes;
        for (auto& [id, node] : m_nodes)
        {
            sortedNodes.push_back(&node);
        }

        hybrid_sort(sortedNodes, [](const builder_node* lhs, const builder_node* rhs){ return lhs->dependencies.size() < rhs->dependencies.size(); });

        struct node_info
        {
            index_type index;
            dynamic_array<dependency_id> dependents;
        };
        dynamic_map<dependency_id, node_info> infoMap;
        dependency_graph<PayloadType> result;
        for (auto* node : sortedNodes)
        {
            infoMap.try_emplace(node->id).first.index = result.m_nodes.size();

            result.m_nodes.emplace_back(
                dependency_graph_node<PayloadType>{
                    .payload = node->payload
            });

            for (auto& dependency : node->dependencies)
            {
                infoMap.try_emplace(dependency.id).first.dependents.push_back(node->id);
            }
        }

        for (auto* node : sortedNodes)
        {

        }
    }
}
