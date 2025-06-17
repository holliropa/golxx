#pragma once
#include <memory>
#include <unordered_map>

struct Node;
using NodePtr = std::shared_ptr<Node>;

struct Node {
    NodePtr nw, ne, sw, se;
    NodePtr result;
    int level;
    bool alive;

    explicit Node(const bool alive_)
        : level(0), alive(alive_) {}

    Node(const NodePtr& nw_, const NodePtr& ne_, const NodePtr& sw_, const NodePtr& se_)
        : nw(nw_), ne(ne_), sw(sw_), se(se_), level(nw_->level + 1), alive(false) {}
};

struct NodeKey {
    NodePtr nw, ne, sw, se;
    int level;
    bool alive;

    NodeKey(const NodePtr& nw_, const NodePtr& ne_, const NodePtr& sw_, const NodePtr& se_)
        : nw(nw_), ne(ne_), sw(sw_), se(se_), level(nw_->level + 1), alive(false) {}

    explicit NodeKey(const bool alive_)
        : nw(nullptr), ne(nullptr), sw(nullptr), se(nullptr), level(0), alive(alive_) {}

    bool operator==(const NodeKey& other) const {
        if (level != other.level) return false;
        if (level == 0) return alive == other.alive;
        return nw == other.nw && ne == other.ne && sw == other.sw && se == other.se;
    }
};

template <>
struct std::hash<NodeKey> {
    size_t operator()(const NodeKey& key) const noexcept {
        if (key.level == 0) {
            return std::hash<bool>()(key.alive);
        }

        size_t hash = 0;
        constexpr size_t prime = 31;
        constexpr std::hash<NodePtr> hasher;

        hash = hash * prime + hasher(key.nw);
        hash = hash * prime + hasher(key.ne);
        hash = hash * prime + hasher(key.sw);
        hash = hash * prime + hasher(key.se);
        hash = hash * prime + std::hash<int>()(key.level);

        return hash;
    }
};

NodePtr make_leaf(const bool alive);

NodePtr make_node(const NodePtr& nw, const NodePtr& ne, const NodePtr& sw, const NodePtr& se);

NodePtr evolve_4x4(const NodePtr& node);

NodePtr evolve(const NodePtr& node);

NodePtr set(NodePtr node, const int x, const int y, const bool alive);
