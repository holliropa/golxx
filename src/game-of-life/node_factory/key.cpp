#include "game-of-life/node_factory.h"

namespace gol {
    NodeFactory::Key::Key(const int level_, const bool alive_)
        : level(level_), alive(alive_) {}

    NodeFactory::Key::Key(const int level_, NodeRef nw, NodeRef ne, NodeRef sw, NodeRef se)
        : level(level_), children{nw, ne, sw, se} {}

    NodeFactory::Key::~Key() {
        if (level > 0) {
            children.~array();
        }
    }

    NodeFactory::Key::Key(const Key& other) : level(other.level) {
        if (level == 0) {
            alive = other.alive;
        }
        else {
            new(&children) std::array<NodePtr, 4>(other.children);
        }
    }

    NodeFactory::Key& NodeFactory::Key::operator=(const Key& other) {
        if (this != &other) {
            if (level > 0) {
                children.~array();
            }

            level = other.level;

            if (level == 0) {
                alive = other.alive;
            }
            else {
                new(&children) std::array<NodePtr, 4>(other.children);
            }
        }

        return *this;
    }


    bool NodeFactory::Key::operator==(const Key& other) const {
        if (level != other.level) return false;
        if (level == 0) return alive == other.alive;
        return children == other.children;
    }
}
