#ifndef LIGHTFOREST_H_INCLUDED
#define LIGHTFOREST_H_INCLUDED

#include "Biome.h"

class ForestBiome : public Biome {
  public:
    ForestBiome(int seed);

    ChunkBlock getTopBlock(Rand &rand) const override;
	ChunkBlock getUnderGroundBlock(Rand &rand) const override;
    ChunkBlock getUnderWaterBlock(Rand &rand) const override;
	ChunkBlock getBeachBlock(Rand &rand) const override;
	ChunkBlock getUnderBeachBlock(Rand &rand) const override;

	ChunkBlock getPlant(Rand &rand) const override;
    void makeTree(Rand &rand, Chunk &chunk, int x, int y, int z) const override;
	void makeBeachPlant(Rand &rand, Chunk &chunk, int x, int y, int z) const override;

  private:
    NoiseParameters getNoiseParameters() override;
};

#endif // LIGHTFOREST_H_INCLUDED
