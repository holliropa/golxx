#include "game-of-life/hashlife.h"

namespace gol {
    Hashlife::Hashlife(NodeFactory& factory)
        : factory_(factory) {}

    NodePtr Hashlife::evolve(NodeRef node) {
        if (!node) return nullptr;

        if (node->result) return node->result;

        if (node->level == 2) {
            evolve_4x4(node);
        }
        else if (node->level > 2) {
            evolve_high_level(node);
        }
        else {
            // Level 0 and 1 nodes don't evolve properly
            node->result = node;
        }

        return node->result;
    }

    NodePtr Hashlife::evolve_4x4(NodeRef node) {
        bool grid[4][4];

        // NW
        if (const auto nw = node->nw()) {
            grid[0][0] = nw->nw() ? nw->nw()->alive : false;
            grid[0][1] = nw->ne() ? nw->ne()->alive : false;
            grid[1][0] = nw->sw() ? nw->sw()->alive : false;
            grid[1][1] = nw->se() ? nw->se()->alive : false;
        }
        else {
            grid[0][0] = grid[0][1] = grid[1][0] = grid[1][1] = false;
        }

        // NE
        if (const auto ne = node->ne()) {
            grid[0][2] = ne->nw() ? ne->nw()->alive : false;
            grid[0][3] = ne->ne() ? ne->ne()->alive : false;
            grid[1][2] = ne->sw() ? ne->sw()->alive : false;
            grid[1][3] = ne->se() ? ne->se()->alive : false;
        }
        else {
            grid[0][2] = grid[0][3] = grid[1][2] = grid[1][3] = false;
        }

        // SW
        if (const auto sw = node->sw()) {
            grid[2][0] = sw->nw() ? sw->nw()->alive : false;
            grid[2][1] = sw->ne() ? sw->ne()->alive : false;
            grid[3][0] = sw->sw() ? sw->sw()->alive : false;
            grid[3][1] = sw->se() ? sw->se()->alive : false;
        }
        else {
            grid[2][0] = grid[2][1] = grid[3][0] = grid[3][1] = false;
        }

        // SE
        if (const auto se = node->se()) {
            grid[2][2] = se->nw() ? se->nw()->alive : false;
            grid[2][3] = se->ne() ? se->ne()->alive : false;
            grid[3][2] = se->sw() ? se->sw()->alive : false;
            grid[3][3] = se->se() ? se->se()->alive : false;
        }
        else {
            grid[2][2] = grid[2][3] = grid[3][2] = grid[3][3] = false;
        }

        bool result[2][2];
        for (int y = 1; y < 3; y++) {
            for (int x = 1; x < 3; x++) {
                int neighbours = 0;
                for (int dy = -1; dy <= 1; dy++) {
                    for (int dx = -1; dx <= 1; dx++) {
                        if (dy == 0 && dx == 0) continue; // Skip the cell itself
                        if (grid[y + dy][x + dx]) neighbours++;
                    }
                }

                const bool current = grid[y][x];
                const bool next = current ? (neighbours == 2 || neighbours == 3) : (neighbours == 3);
                result[y - 1][x - 1] = next;
            }
        }

        const auto result_nw = factory_.leaf(result[0][0]);
        const auto result_ne = factory_.leaf(result[0][1]);
        const auto result_sw = factory_.leaf(result[1][0]);
        const auto result_se = factory_.leaf(result[1][1]);

        node->result = factory_.node(1, result_nw, result_ne, result_sw, result_se);
        return node->result;
    }


    NodePtr Hashlife::evolve_high_level(NodeRef node) {
        const auto nw = node->nw();
        const auto ne = node->ne();
        const auto sw = node->sw();
        const auto se = node->se();

        // n00 n01 n02
        // n10 n11 n12
        // n20 n21 n22
        const auto n00 = evolve(nw);
        const auto n01 = evolve(centered_horizontal(nw, ne));
        const auto n02 = evolve(ne);
        const auto n10 = evolve(centered_vertical(nw, sw));
        const auto n11 = evolve(centered_subnode(nw, ne, sw, se));
        const auto n12 = evolve(centered_vertical(ne, se));
        const auto n20 = evolve(sw);
        const auto n21 = evolve(centered_horizontal(sw, se));
        const auto n22 = evolve(se);

        const auto result_nw = evolve(factory_.node(n00, n01, n10, n11));
        const auto result_ne = evolve(factory_.node(n01, n02, n11, n12));
        const auto result_sw = evolve(factory_.node(n10, n11, n20, n21));
        const auto result_se = evolve(factory_.node(n11, n12, n21, n22));

        node->result = factory_.node(result_nw, result_ne, result_sw, result_se);
        return node->result;
    }

    NodePtr Hashlife::centered_horizontal(NodeRef west, NodeRef east) const {
        NodePtr nw, ne, sw, se;

        if (west) {
            nw = west->ne();
            sw = west->se();
        }
        else {
            nw = sw = nullptr;
        }

        if (east) {
            ne = east->nw();
            se = east->sw();
        }
        else {
            ne = se = nullptr;
        }

        return factory_.node(nw, ne, sw, se);
    }

    NodePtr Hashlife::centered_vertical(NodeRef north, NodeRef south) const {
        NodePtr nw, ne, sw, se;

        if (north) {
            nw = north->sw();
            ne = north->se();
        }
        else {
            nw = ne = nullptr;
        }

        if (south) {
            sw = south->nw();
            se = south->ne();
        }
        else {
            sw = se = nullptr;
        }

        return factory_.node(nw, ne, sw, se);
    }

    NodePtr Hashlife::centered_subnode(NodeRef nw, NodeRef ne, NodeRef sw, NodeRef se) const {
        const auto centered_nw = nw ? nw->se() : nullptr;
        const auto centered_ne = ne ? ne->sw() : nullptr;
        const auto centered_sw = sw ? sw->ne() : nullptr;
        const auto centered_se = se ? se->nw() : nullptr;

        return factory_.node(centered_nw, centered_ne, centered_sw, centered_se);
    }
}
