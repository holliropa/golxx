#include "game-of-life/hashlife.h"

namespace gol {
    NodePtr Hashlife::make_leaf(const bool alive) {
        const NodeKey key(alive);
        const auto it = node_cache_.find(key);
        if (it != node_cache_.end()) {
            return it->second;
        }
        auto node = std::make_shared<Node>(alive);
        node_cache_[key] = node;
        return node;
    }

    NodePtr Hashlife::make_node(const NodePtr& nw, const NodePtr& ne, const NodePtr& sw, const NodePtr& se) {
        const NodeKey key(nw, ne, sw, se);
        const auto it = node_cache_.find(key);
        if (it != node_cache_.end()) {
            return it->second;
        }
        const auto node = std::make_shared<Node>(nw, ne, sw, se);
        node_cache_[key] = node;
        return node;
    }

    NodePtr Hashlife::evolve_4x4(const NodePtr& node) {
        bool grid[4][4];

        // Top-left quadrant (nw)
        grid[0][0] = node->nw->nw->alive;
        grid[0][1] = node->nw->ne->alive;
        grid[1][0] = node->nw->sw->alive;
        grid[1][1] = node->nw->se->alive;

        // Top-right quadrant (ne)
        grid[0][2] = node->ne->nw->alive;
        grid[0][3] = node->ne->ne->alive;
        grid[1][2] = node->ne->sw->alive;
        grid[1][3] = node->ne->se->alive;

        // Bottom-left quadrant (sw)
        grid[2][0] = node->sw->nw->alive;
        grid[2][1] = node->sw->ne->alive;
        grid[3][0] = node->sw->sw->alive;
        grid[3][1] = node->sw->se->alive;

        // Bottom-right quadrant (se)
        grid[2][2] = node->se->nw->alive;
        grid[2][3] = node->se->ne->alive;
        grid[3][2] = node->se->sw->alive;
        grid[3][3] = node->se->se->alive;

        bool evolved_grid[2][2];

        for (int row = 1; row < 3; row++) {
            for (int col = 1; col < 3; col++) {
                int neighbours = 0;

                for (int dr = -1; dr <= 1; dr++) {
                    for (int dc = -1; dc <= 1; dc++) {
                        if (dr == 0 && dc == 0) continue; // Skip the cell itself
                        if (grid[row + dr][col + dc]) neighbours++;
                    }
                }

                const auto current_alive = grid[row][col];
                auto next_alive = false;

                if (current_alive) {
                    next_alive = neighbours == 2 || neighbours == 3;
                }
                else {
                    next_alive = neighbours == 3;
                }

                evolved_grid[row - 1][col - 1] = next_alive;
            }
        }

        // The result as a level 1 node (2x2)
        const auto nw = make_leaf(evolved_grid[0][0]);
        const auto ne = make_leaf(evolved_grid[0][1]);
        const auto sw = make_leaf(evolved_grid[1][0]);
        const auto se = make_leaf(evolved_grid[1][1]);

        return make_node(nw, ne, sw, se);
    }

    NodePtr Hashlife::evolve(const NodePtr& node) {
        if (!node) return nullptr;

        if (node->result) return node->result;

        // 1x1 node - cell always dies;
        // if (node->level == 0) {
        //     node->result = evolve_1x1(node);
        //     return node->result;
        // }

        // 2x2 node;
        // if (node->level == 1) {
        //     node->result = evolve_2x2(node);
        //     return node->result;;
        // }

        // 4x4 node;
        if (node->level == 2) {
            node->result = evolve_4x4(node);
            return node->result;
        }

        const auto nw = node->nw;
        const auto ne = node->ne;
        const auto sw = node->sw;
        const auto se = node->se;

        // n00 n01 n02
        // n10 n11 n12
        // n20 n21 n22
        auto n00 = make_node(nw->nw, nw->ne, nw->sw, nw->se);
        auto n01 = make_node(nw->ne, ne->nw, nw->se, ne->sw);
        auto n02 = make_node(ne->nw, ne->ne, ne->sw, ne->se);
        auto n10 = make_node(nw->sw, nw->se, sw->nw, sw->ne);
        auto n11 = make_node(nw->se, ne->sw, sw->ne, se->nw);
        auto n12 = make_node(ne->sw, ne->se, se->nw, se->ne);
        auto n20 = make_node(sw->nw, sw->ne, sw->sw, sw->se);
        auto n21 = make_node(sw->ne, se->nw, sw->se, se->sw);
        auto n22 = make_node(se->nw, se->ne, se->sw, se->se);

        n00 = evolve(n00);
        n01 = evolve(n01);
        n02 = evolve(n02);
        n10 = evolve(n10);
        n11 = evolve(n11);
        n12 = evolve(n12);
        n20 = evolve(n20);
        n21 = evolve(n21);
        n22 = evolve(n22);

        node->result = make_node(
            evolve(make_node(n00, n01, n10, n11)),
            evolve(make_node(n01, n02, n11, n12)),
            evolve(make_node(n10, n11, n20, n21)),
            evolve(make_node(n11, n12, n21, n22))
        );

        return node->result;
    }

    NodePtr Hashlife::set(const NodePtr& node, int x, int y, bool alive) {
        if (!node) return nullptr;

        if (node->level == 0) {
            return make_leaf(alive);
        }

        auto new_nw = node->nw;
        auto new_ne = node->ne;
        auto new_sw = node->sw;
        auto new_se = node->se;

        const auto half_size = 1 << (node->level - 1);

        if (x >= 0 && y >= 0) {
            new_ne = set(node->ne, x - half_size, y - half_size, alive);
        }
        else if (x >= 0) {
            new_se = set(node->se, x - half_size, y + half_size, alive);
        }
        else if (y >= 0) {
            new_nw = set(node->nw, x + half_size, y - half_size, alive);
        }
        else {
            new_sw = set(node->sw, x + half_size, y + half_size, alive);
        }

        return make_node(new_nw, new_ne, new_sw, new_se);
    }

    NodePtr Hashlife::get(const NodePtr& node, const int x, const int y) {
        if (!node) return nullptr;

        if (node->level == 0) {
            return node;
        }

        int offset;
        if (node->level == 1) {
            offset = 0;
        }
        else if (node->level == 2) {
            offset = 1;
        }
        else {
            offset = 1 << (node->level - 2);
        }

        if (x >= 0 && y >= 0) {
            return get(node->ne, x - offset, y - offset);
        }

        if (x >= 0 && y < 0) {
            return get(node->se, x - offset, y + offset);
        }

        if (x < 0 && y >= 0) {
            return get(node->nw, x + offset, y - offset);
        }

        return get(node->sw, x + offset, y + offset);
    }

    void Hashlife::expand(const int level) {
        const auto leaf = make_leaf(false);

        if (!root_) {
            root_ = make_node(leaf, leaf, leaf, leaf);
        }

        for (int i = 0; i < level; i++) {}iii
    }
}
