#pragma once

#include "TerrainGenerator.h"

class SuperFlatGenerator : public TerrainGenerator {
public:
	void generateTerrainFor(Chunk &chunk) override;
	int getMinimumSpawnHeight() const noexcept override;

	BiomeId getBiomeId(Chunk &chunk, int x, int z) override
	{
		return BiomeId::ValleyBiome;
	};
};