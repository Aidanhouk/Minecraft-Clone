#pragma once

#include <array>

static constexpr float
AO_X2 = 2.0f,
AO_FULL = 1.0f,
AO_66 = 0.666f,
AO_33 = 0.333f;

static constexpr std::array<std::array<int, 3>, 8> ADJ_BLOCKS_DATA_TOP{ {
        //    x  y  z
            {-1, 0,-1},
            { 0, 0,-1},
            {+1, 0,-1},
            {+1, 0, 0},
            {+1, 0,+1},
            { 0, 0,+1},
            {-1, 0,+1},
            {-1, 0, 0}
        } };
static constexpr std::array<std::array<int, 3>, 8> ADJ_BLOCKS_DATA_BOTTOM = ADJ_BLOCKS_DATA_TOP;

static constexpr std::array<std::array<int, 3>, 8> ADJ_BLOCKS_DATA_LEFT{ {
        //   x  y  z
            {0,+1,-1},
            {0,+1, 0},
            {0,+1,+1},
            {0, 0,+1},
            {0,-1,+1},
            {0,-1, 0},
            {0,-1,-1},
            {0, 0,-1}
        } };
static constexpr std::array<std::array<int, 3>, 8> ADJ_BLOCKS_DATA_RIGHT = ADJ_BLOCKS_DATA_LEFT;

static constexpr std::array<std::array<int, 3>, 8> ADJ_BLOCKS_DATA_BACK{ {
        //    x  y  z
            {-1,+1, 0},
            { 0,+1, 0},
            {+1,+1, 0},
            {+1, 0, 0},
            {+1,-1, 0},
            { 0,-1, 0},
            {-1,-1, 0},
            {-1, 0, 0}
        } };
static constexpr std::array<std::array<int, 3>, 8> ADJ_BLOCKS_DATA_FRONT = ADJ_BLOCKS_DATA_BACK;



static constexpr std::array<std::array<int, 3>, 4> AO_DATA_TOP{ {
            {5,7,6},
            {3,5,4},
            {1,3,2},
            {7,1,0},
        } };
static constexpr std::array<std::array<int, 3>, 4> AO_DATA_BOTTOM{ {
            {7,1,0},
            {1,3,2},
            {3,5,4},
            {5,7,6},
        } };
static constexpr std::array<std::array<int, 3>, 4> AO_DATA_LEFT{ {
            {3,5,6},
            {5,7,4},
            {1,3,2},
            {7,1,0},
        } };
static constexpr std::array<std::array<int, 3>, 4> AO_DATA_RIGHT{ {
            {3,5,4},
            {5,7,6},
            {1,3,2},
            {7,1,0},
        } };
static constexpr std::array<std::array<int, 3>, 4> AO_DATA_BACK{ {
            {5,7,6},
            {3,5,4},
            {1,3,2},
            {7,1,0},
        } };
static constexpr std::array<std::array<int, 3>, 4> AO_DATA_FRONT{ {
            {3,5,4},
            {7,1,0},
            {5,7,6},
            {1,3,2},
        } };
