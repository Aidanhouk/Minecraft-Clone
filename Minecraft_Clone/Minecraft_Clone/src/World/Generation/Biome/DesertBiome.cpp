#include "DesertBiome.h"

#include "../../WorldConstants.h"
#include "../Structures/TreeGenerator.h"

DesertBiome::DesertBiome(int seed)
    : Biome(getNoiseParameters(), 350, 500, seed)
{
	m_Id = BiomeId::DesertBiome;
}

ChunkBlock DesertBiome::getTopBlock(Rand &rand) const
{
    return BlockId::Sand;
}

ChunkBlock DesertBiome::getUnderGroundBlock(Rand & rand) const
{
	return BlockId::Sand;
}

ChunkBlock DesertBiome::getUnderWaterBlock(Rand &rand) const
{
    return BlockId::Sand;
}

ChunkBlock DesertBiome::getPlant(Rand &rand) const
{
	return BlockId::DeadShrub;
}

void DesertBiome::makeTree(Rand &rand, Chunk &chunk, int x, int y, int z) const
{
    if (y < WATER_LEVEL + 5) {
        makePalmTree(chunk, rand, x, y, z);
    }
    else {
        makeCactus(chunk, rand, x, y, z);
    }
}

NoiseParameters DesertBiome::getNoiseParameters()
{
	NoiseParameters heightParams;
	heightParams.octaves = 9;
	heightParams.amplitude = 30;
	heightParams.smoothness = 300;
	heightParams.heightOffset = 35;
	heightParams.roughness = 0.6f;
	
	return heightParams;
}
