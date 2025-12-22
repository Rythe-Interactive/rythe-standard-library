#pragma once
#include "array.hpp"

namespace rsl
{
    enum struct dependency_type : uint8
    {
        shared,
        exclusive,
        node,
    };

    struct dependency
    {
        id_type id;
        dependency_type type;
    };

    struct dependency_graph_node
    {
        id_type id;
        void* payload;
        dynamic_array<dependency> dependencies;
    };

    class dependency_graph_builder
    {

    };

    class dependency_graph
    {

    };
}
