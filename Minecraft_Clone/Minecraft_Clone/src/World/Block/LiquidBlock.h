#pragma once

#include "ChunkBlock.h"

class LiquidBlock

{
public:
    LiquidBlock(uint8_t waterLevel = 15)
        : m_waterLevel(waterLevel)
    {}

private:

    uint8_t m_waterLevel;
};