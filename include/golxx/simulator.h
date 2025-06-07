#pragma once
#include <unordered_set>
#include "glm_common.h"

namespace golxx {
    class Simulator {
    public:
        Simulator() : generation_(0) {}
        ~Simulator() = default;

        const std::unordered_set<glm::ivec2>& getCells() const {
            return cells_;
        }

        unsigned int getGeneration() const {
            return generation_;
        }

        void set_state(glm::ivec2 cell, bool state);

        void run_cycle();

    private:
        std::unordered_set<glm::ivec2> cells_;
        unsigned int generation_;
    };
}
