#pragma once

#include "Biome.h"

class MountainBiome : public Biome {
public:
	MountainBiome(int seed);

	ChunkBlock getTopBlock(Rand &rand, int y) const override;
	ChunkBlock getUnderGroundBlock(Rand &rand) const override;
	ChunkBlock getUnderWaterBlock(Rand &rand) const override;
	ChunkBlock getWaterSurfaceBlock(Rand &rand) const override;

	ChunkBlock getPlant(Rand &rand) const override;
	void makeTree(Rand &rand, Chunk &chunk, int x, int y, int z) const override;

private:
	NoiseParameters getNoiseParameters() override;
};