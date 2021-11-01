#pragma once

#include "Biome.h"

class DesertBiome : public Biome {
public:
	DesertBiome(int seed);
	
	ChunkBlock getTopBlock(Rand &rand, int y) const override;
	ChunkBlock getUnderGroundBlock(Rand &rand) const override;
	ChunkBlock getUnderWaterBlock(Rand &rand) const override;
	
	ChunkBlock getPlant(Rand &rand) const override;
	void makeTree(Rand &rand, Chunk &chunk, int x, int y, int z) const override;
	void makeUnderwaterCluster(Rand &rand, Chunk &chunk, int x, int z,
		Array2D<int, CHUNK_SIZE> &heightMap) const override;
	
private:
	NoiseParameters getNoiseParameters() override;
};