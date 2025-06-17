#pragma once
#include <unordered_set>
#include "glm_common.h"
#include "hashlife/hashlife.h"

namespace golxx {
    class Simulator {
    public:
        Simulator() : generation_(0) {
            auto alive_cell = make_leaf(true);
            auto level1 = make_node(alive_cell, alive_cell, alive_cell, alive_cell);
            auto level2 = make_node(level1, level1, level1, level1);
            auto level3 = make_node(level2, level2, level2, level2);
            auto level4 = make_node(level3, level3, level3, level3);
            auto level5 = make_node(level4, level4, level4, level4);
            auto level6 = make_node(level5, level5, level5, level5);
            root = level6;
        }

        ~Simulator() = default;

        const std::unordered_set<glm::ivec2>& getCells() const {
            return cells_;
        }

        unsigned int getGeneration() const {
            return generation_;
        }

        void set_state(glm::ivec2 cell, bool state);

        void run_cycle();

        NodePtr root;

    private:
        std::unordered_set<glm::ivec2> cells_;
        unsigned int generation_;
    };
}
