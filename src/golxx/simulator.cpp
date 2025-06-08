#include "golxx/simulator.h"
#include <unordered_map>

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
        std::unordered_map<glm::ivec2, int> neighborCounts{};

        const std::vector<glm::ivec2> neighborOffsets = {
            {-1, -1}, {-1, 0}, {-1, 1},
            {0, -1}, {0, 1},
            {1, -1}, {1, 0}, {1, 1}
        };

        // Calculate neighbor counts for all relevant cells
        for (const auto& cell : cells_) {
            for (const auto& offset : neighborOffsets) {
                glm::ivec2 neighbor = cell + offset;
                neighborCounts[neighbor]++;
            }
        }

        // Apply Conway's Game of Life rules
        for (const auto& [cell, count] : neighborCounts) {
            if (cells_.find(cell) != cells_.end()) {
                // Live cell with 2 or 3 neighbors survives
                if (count == 2 || count == 3) {
                    nextCells.insert(cell);
                }
                // Otherwise it dies (underpopulation or overpopulation)
            }
            else {
                // Dead cell with exactly 3 neighbors becomes alive
                if (count == 3) {
                    nextCells.insert(cell);
                }
            }
        }

        cells_ = std::move(nextCells);
        generation_++;
    }
}
