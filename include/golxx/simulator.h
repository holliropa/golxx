#pragma once
#include <memory>

#include "game-of-life/hashlife.h"

namespace golxx {
    class Simulator {
    public:
        Simulator();

        gol::NodePtr get_root() const { return root_; }

        bool get(int x, int y) const;
        void set(int x, int y, bool alive);
        void evolve();

    private:
        std::unique_ptr<gol::Hashlife> hashlife_;
        gol::NodeFactory node_factory_;
        gol::NodePtr root_ = nullptr;

        bool is_in_bounds(gol::NodeRef node, int x, int y) const;
        gol::NodePtr get(gol::NodeRef node, int x, int y) const;
        // gol::NodePtr set(gol::NodeRef node, int x, int y, bool alive);
        gol::NodePtr expand(gol::NodeRef node, int level);

        static int get_quadrant(int x, int y);
    };
}
