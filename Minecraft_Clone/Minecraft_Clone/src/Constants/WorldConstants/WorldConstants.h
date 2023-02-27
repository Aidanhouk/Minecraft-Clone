#pragma once

#include <cstdint>

static constexpr int
CHUNK_SIZE = 16,
CHUNK_AREA = CHUNK_SIZE * CHUNK_SIZE,
CHUNK_VOLUME = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE,

WATER_LEVEL = 64;

static constexpr uint8_t
WATER_LEVEL_MAX = 8u,
LAVA_LEVEL_MAX = 4u;