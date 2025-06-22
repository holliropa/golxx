#pragma once
#include <memory>

namespace gol {
    struct Node;
    using NodePtr = std::shared_ptr<const Node>;
    using NodeRef = const NodePtr&;

    struct Node {
        int level;

        // Union-like design: either it's a leaf with a live state or has children
        union {
            bool alive; // For level 0 nodes
            NodePtr children[4]; // For level > 0: [nw, ne, sw, se]
        };

        mutable NodePtr result; // Cached evolution result

        explicit Node(int level_, bool alive_); // Leaf constructor
        Node(int level_, NodeRef nw_, NodeRef ne_, NodeRef sw_, NodeRef se_);
        ~Node();

        // Accessors with intuitive names
        NodePtr nw() const { return level == 0 ? nullptr : children[0]; }
        NodePtr ne() const { return level == 0 ? nullptr : children[1]; }
        NodePtr sw() const { return level == 0 ? nullptr : children[2]; }
        NodePtr se() const { return level == 0 ? nullptr : children[3]; }

        bool is_leaf() const { return level == 0; }
        bool is_empty() const;

        // No copy/mode - nodes are immutable and cached
        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;
    };
}
