#include "game-of-life/node.h"

namespace gol {
    Node::Node(const int level_, const bool alive_)
        : level(level_), alive(alive_) {}

    Node::Node(const int level_, NodeRef nw_, NodeRef ne_, NodeRef sw_, NodeRef se_)
        : level(level_), children{nw_, ne_, sw_, se_} {}

    Node::~Node() {}

    bool Node::is_empty() const {
        if (level == 0) {
            return !alive;
        }

        for (const auto& child : children) {
            if (child && !child->is_empty()) {
                return false;
            }
        }

        return true;
    }
}
