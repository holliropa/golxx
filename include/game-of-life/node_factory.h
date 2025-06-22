#pragma once
#include <array>
#include <unordered_map>
#include <vector>

#include "node.h"
#include "node_factory.h"

namespace gol {
    class NodeFactory {
    private:
        struct Key {
            int level;

            union {
                bool alive;
                std::array<NodePtr, 4> children;
            };

            explicit Key(int level_, bool alive_);
            Key(int level_, NodeRef nw, NodeRef ne, NodeRef sw, NodeRef se);
            ~Key();

            Key(const Key& other);
            Key& operator=(const Key& other);

            bool operator==(const Key& other) const;
        };

        struct NodeKeyHash {
            size_t operator()(const Key& key) const noexcept;
        };

        std::unordered_map<Key, NodePtr, NodeKeyHash> node_cache_;
        std::vector<NodePtr> empty_nodes_;

        size_t cache_hits_ = 0;
        size_t cache_misses_ = 0;

        NodePtr create_leaf(bool alive);
        NodePtr create_node(int level, NodeRef nw, NodeRef ne, NodeRef sw, NodeRef se);

    public:
        NodeFactory();
        NodePtr leaf(bool alive);

        NodePtr node(int level,
                     NodeRef nw = nullptr,
                     NodeRef ne = nullptr,
                     NodeRef sw = nullptr,
                     NodeRef se = nullptr);

        NodePtr node(NodeRef nw = nullptr,
                     NodeRef ne = nullptr,
                     NodeRef sw = nullptr,
                     NodeRef se = nullptr);

        size_t cache_size() const { return node_cache_.size(); }
        size_t cache_hits() const { return cache_hits_; }
        size_t cache_misses() const { return cache_misses_; }
    };
}
