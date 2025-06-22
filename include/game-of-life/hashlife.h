#pragma once

#include "node.h"
#include "node_factory.h"

namespace gol {
    class Hashlife {
    public:
        explicit Hashlife(NodeFactory& factory);

        NodePtr evolve(NodeRef node);

    private:
        NodeFactory& factory_;

        NodePtr evolve_4x4(NodeRef node);
        NodePtr evolve_high_level(NodeRef node);

        NodePtr centered_horizontal(NodeRef west, NodeRef east) const;
        NodePtr centered_vertical(NodeRef north, NodeRef south) const;
        NodePtr centered_subnode(NodeRef nw, NodeRef ne, NodeRef sw, NodeRef se) const;
    };
}
