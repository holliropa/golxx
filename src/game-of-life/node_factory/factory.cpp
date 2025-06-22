#include "game-of-life/node_factory.h"

namespace gol {
    NodeFactory::NodeFactory() {
        empty_nodes_.reserve(10);
        empty_nodes_.emplace_back(leaf(false));
    }

    NodePtr NodeFactory::create_leaf(const bool alive) {
        const Key key(0, alive);
        const auto it = node_cache_.find(key);
        if (it != node_cache_.end()) {
            cache_hits_++;
            return it->second;
        }

        cache_misses_++;
        auto node = std::make_shared<Node>(0, alive);
        node_cache_[key] = node;
        return node;
    }

    NodePtr NodeFactory::create_node(const int level, NodeRef nw, NodeRef ne, NodeRef sw, NodeRef se) {
        const Key key(level, nw, ne, sw, se);
        const auto it = node_cache_.find(key);
        if (it != node_cache_.end()) {
            cache_hits_++;
            return it->second;
        }

        cache_misses_++;
        auto node = std::make_shared<Node>(level, nw, ne, sw, se);
        node_cache_[key] = node;
        return node;
    }

    NodePtr NodeFactory::leaf(const bool alive) {
        return create_leaf(alive);
    }

    NodePtr NodeFactory::node(const int level, NodeRef nw, NodeRef ne, NodeRef sw, NodeRef se) {
        return create_node(level, nw, ne, sw, se);
    }

    NodePtr NodeFactory::node(NodeRef nw, NodeRef ne, NodeRef sw, NodeRef se) {
        int level = -1;

        if (nw)
            level = nw->level;
        else if (ne)
            level = ne->level;
        else if (sw)
            level = sw->level;
        else if (se)
            level = se->level;

        if (level < 0)
            return nullptr;

        return create_node(level + 1, nw, ne, sw, se);
    }
}
