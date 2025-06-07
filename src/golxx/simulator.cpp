#include "golxx/simulator.h"

namespace golxx {
    void Simulator::set_state(const glm::ivec2 cell, const bool state) {
        if (state) {
            cells_.insert(cell);
        }
        else {
            cells_.erase(cell);
        }
    }

    void Simulator::run_cycle() {
        std::unordered_set<glm::ivec2> nextCells{};
        std::unordered_set<glm::ivec2> activeCells{};

        const std::vector<glm::ivec2> neighborOffsets = {
            {-1, -1},
            {-1, 0},
            {-1, 1},
            {0, -1},
            {0, 1},
            {1, -1},
            {1, 0},
            {1, 1}
        };

        for (const auto& cell : cells_) {
            activeCells.insert(cell);

            for (const auto& offset : neighborOffsets) {
                activeCells.insert(cell + offset);
            }
        }

        for (const auto& cell : activeCells) {
            int liveNeighbors = 0;

            for (const auto& offset : neighborOffsets) {
                if (cells_.find(cell + offset) != cells_.end()) {
                    liveNeighbors++;
                }
            }

            if (cells_.find(cell) != cells_.end()) {
                if (liveNeighbors < 2 || liveNeighbors > 3) {
                    nextCells.erase(cell); // Cell dies
                }
                else {
                    nextCells.insert(cell); // Cell lives
                }
            }
            else {
                if (liveNeighbors == 3) {
                    nextCells.insert(cell); // Cell becomes alive
                }
            }
        }

        cells_ = nextCells;
        generation_++;
    }
}
