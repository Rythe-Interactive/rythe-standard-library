#pragma once
#include "../array.hpp"

#include "dependency_graph.hpp"

namespace rsl
{
    struct task_graph_node
    {
        id_type id;
        size_type iteration;
    };

    class task_queue
    {
    public:
        id_type acquire_task() noexcept;
        void release_task(id_type id, bool markFinished = true) noexcept;

        size_type remaining() noexcept;
        size_type size() noexcept;
    };

    class task_graph
    {
    public:
        task_queue create_queue();

    };

    struct builder_resource_dependency
    {
        id_type id;
        dependency_type type;
    };

    struct builder_task
    {
        id_type id;
        dynamic_array<id_type> dependencies;
        dynamic_array<builder_resource_dependency> resources;
    };

    class task_graph_builder
    {
    public:
        bool add_task(const id_type id) noexcept
        {
            const index_type nextIndex = m_nodes.size();
            if (m_nodeIdMap.try_emplace(id, nextIndex).second)
            {
                m_nodes.emplace_back().id = id;
                return true;
            }
            return false;
        }

        bool add_task_dependency(const id_type nodeId, const id_type depId)
        {
            const index_type* nodeIndex = m_nodeIdMap.find(nodeId);
            if (!nodeIndex)
            {
                return false;
            }

            auto& node = m_nodes[*nodeIndex];
            if (linear_search(node.dependencies.view(), depId) != npos)
            {
                return false;
            }

            node.dependencies.emplace_back(depId);
            return true;
        }

        bool add_resource_dependency(const id_type nodeId, const builder_resource_dependency dep)
        {
            const index_type* nodeIndex = m_nodeIdMap.find(nodeId);
            if (!nodeIndex)
            {
                return false;
            }

            auto& node = m_nodes[*nodeIndex];

            struct node_searcher
            {
                id_type id;
                bool operator==(const builder_resource_dependency& node) const noexcept { return node.id == id; }
            };
            if (linear_search(node.resources.view(), node_searcher{ .id = dep.id }) != npos)
            {
                return false;
            }

            node.resources.emplace_back(dep);
            return true;
        }

    private:
        dynamic_array<builder_task> m_nodes;
        dynamic_map<id_type, index_type> m_nodeIdMap;
    };
}

#include "task_graph.inl"
