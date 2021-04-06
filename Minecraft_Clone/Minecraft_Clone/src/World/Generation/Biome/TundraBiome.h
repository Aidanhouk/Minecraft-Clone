#ifndef TEMPERATEFORESTBIOME_H_INCLUDED
#define TEMPERATEFORESTBIOME_H_INCLUDED

#include "Biome.h"

class TundraBiome : public Biome {
  public:
    TundraBiome(int seed);

    ChunkBlock getTopBlock(Rand &rand) const override;
	ChunkBlock getUnderGroundBlock(Rand &rand) const override;
    ChunkBlock getUnderWaterBlock(Rand &rand) const override;
	ChunkBlock getBeachBlock(Rand &rand) const override;
	ChunkBlock getUnderBeachBlock(Rand &rand) const override;

	ChunkBlock getPlant(Rand &rand) const override;
    void makeTree(Rand &rand, Chunk &chunk, int x, int y, int z) const override;

  private:
    NoiseParameters getNoiseParameters();
};

#endif // TEMPERATEFORESTBIOME_H_INCLUDED
