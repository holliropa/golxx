#include "game-of-life/node_factory.h"

namespace gol {
    size_t NodeFactory::NodeKeyHash::operator()(const Key& key) const noexcept {
        if (key.level == 0) {
            return std::hash<bool>()(key.alive) ^ std::hash<int>()(key.level);
        }

        auto hash = std::hash<int>()(key.level);
        constexpr size_t prime = 31;
        constexpr std::hash<NodePtr> hasher;

        for (const auto& child : key.children) {
            hash = hash * prime + (child ? hasher(child) : 0);
        }

        return hash;
    }
}
