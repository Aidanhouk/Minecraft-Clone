#pragma once

#include "../Biome/Biome.h"

class Chunk;

//struct Biome;

class TerrainGenerator {
public:
	virtual ~TerrainGenerator() = default;

    virtual void generateTerrainFor(Chunk &chunk) = 0;
    virtual int getMinimumSpawnHeight() const noexcept = 0;

	virtual BiomeId getBiomeId(Chunk &chunk, int x, int z) = 0;
};