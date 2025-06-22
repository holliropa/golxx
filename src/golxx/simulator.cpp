#include "golxx/simulator.h"

#include <cmath>
#include <iostream>
#include <bits/ostream.tcc>

namespace golxx {
    Simulator::Simulator() {
        hashlife_ = std::make_unique<gol::Hashlife>(node_factory_);
        const auto dead_cell = node_factory_.leaf(false);
        const auto live_cell = node_factory_.leaf(true);

        const auto nw_ = node_factory_.node(dead_cell, dead_cell, dead_cell, live_cell);
        const auto ne_ = node_factory_.node();
        const auto sw_ = node_factory_.node(dead_cell, dead_cell, live_cell, live_cell);
        const auto se_ = node_factory_.node(live_cell, dead_cell, live_cell, dead_cell);

        const auto glider = node_factory_.node(nw_, ne_, sw_, se_);

        root_ = expand(glider, 9);

        std::cout << "Cache: " << node_factory_.cache_size() << std::endl;
        std::cout << "Hits: " << node_factory_.cache_hits() << std::endl;
        std::cout << "Misses: " << node_factory_.cache_misses() << std::endl;
    }

    bool Simulator::get(const int x, const int y) const {
        if (!is_in_bounds(root_, x, y))
            return false;

        const auto cell = get(root_, x, y);
        return cell ? cell->alive : false;
    }

    void Simulator::set(const int x, const int y, const bool alive) {
        const auto bound = std::max(std::abs(x), std::abs(y));
        const auto level = static_cast<int>(std::ceil(std::log2(2 * bound + 1)));

        if (!root_) {
            root_ = node_factory_.node(level);
        }
        else if (level > root_->level) {
            root_ = expand(root_, level - root_->level);
        }

        // root_ = set(root_, x, y, alive);
    }

    void Simulator::evolve() {
        root_ = hashlife_->evolve(root_);
    }

    bool Simulator::is_in_bounds(gol::NodeRef node, const int x, const int y) const {
        if (!node) return false;

        if (node->level == 0) {
            return x == 0 && y == 0;
        }

        const auto size = 1 << (node->level - 1);

        return x < size && x >= -size &&
            y < size && y >= -size;
    }

    gol::NodePtr Simulator::get(gol::NodeRef node, const int x, const int y) const {
        if (!node) return nullptr;

        if (node->level == 0) {
            if (x == 0 && y == 0) return node;
            return nullptr;
        }

        if (node->level == 1) {
            const auto quadrant = get_quadrant(x, y);
            return node->children[quadrant];
        }

        const auto half_size = 1 << (node->level - 2);
        const auto quadrant = get_quadrant(x, y);

        int new_x = x, new_y = y;
        switch (quadrant) {
        case 0: // NW: x < 0, y ≥ 0
            new_x = x + half_size;
            new_y = y - half_size;
            break;
        case 1: // NE: x ≥ 0, y ≥ 0
            new_x = x - half_size;
            new_y = y - half_size;
            break;
        case 2: // SW: x < 0, y < 0
            new_x = x + half_size;
            new_y = y + half_size;
            break;
        case 3: // SE: x ≥ 0, y < 0
            new_x = x - half_size;
            new_y = y + half_size;
            break;
        default: break;
        }

        return get(node->children[quadrant], new_x, new_y);
    }

    // gol::NodePtr Simulator::set(gol::NodeRef node, int x, int y, const bool alive) {
    //     if (node->level == 0) {
    //         return node_factory_.leaf(alive);
    //     }
    //
    //     const auto children = node->children;
    //     const auto quadrant = get_quadrant(x, y);
    //
    //     if (!children[quadrant]) {
    //         children[quadrant] =
    //     }
    // }

    gol::NodePtr Simulator::expand(gol::NodeRef node, const int level) {
        if (level <= 0)
            return node;

        const auto nw = node->nw();
        const auto ne = node->ne();
        const auto sw = node->sw();
        const auto se = node->se();

        const auto new_nw = node_factory_.node(nullptr, nullptr, nullptr, nw);
        const auto new_ne = node_factory_.node(nullptr, nullptr, ne, nullptr);
        const auto new_sw = node_factory_.node(nullptr, sw, nullptr, nullptr);
        const auto new_se = node_factory_.node(se, nullptr, nullptr, nullptr);

        const auto expanded_node = node_factory_.node(node->level + 1, new_nw, new_ne, new_sw, new_se);
        return expand(expanded_node, level - 1);
    }

    int Simulator::get_quadrant(const int x, const int y) {
        return (y < 0) << 1 | x >= 0;
    }
}
