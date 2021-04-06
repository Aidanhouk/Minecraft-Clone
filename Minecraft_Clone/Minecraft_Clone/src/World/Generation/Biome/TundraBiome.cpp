#include "TundraBiome.h"

#include "../Structures/TreeGenerator.h"

TundraBiome::TundraBiome(int seed)
    : Biome(getNoiseParameters(), 120, 80, seed)
{
	m_Id = BiomeId::TundraBiome;
}

ChunkBlock TundraBiome::getTopBlock(Rand &rand) const
{
	return BlockId::Snow;
}

ChunkBlock TundraBiome::getUnderGroundBlock(Rand & rand) const
{
	return BlockId::Dirt;
}

ChunkBlock TundraBiome::getUnderWaterBlock(Rand &rand) const
{
    return BlockId::Dirt;
}

ChunkBlock TundraBiome::getBeachBlock(Rand & rand) const
{
	return BlockId::Snow;
}

ChunkBlock TundraBiome::getUnderBeachBlock(Rand & rand) const
{
	return BlockId::Dirt;
}

ChunkBlock TundraBiome::getPlant(Rand &rand) const
{
	return rand.intInRange(0, 100) > 99 ? BlockId::Rose : BlockId::DeadShrub;
}

void TundraBiome::makeTree(Rand &rand, Chunk &chunk, int x, int y, int z) const
{
    makeSpruceTree(chunk, rand, x, y, z);
}

NoiseParameters TundraBiome::getNoiseParameters()
{
	NoiseParameters heightParams;
	heightParams.octaves = 7;
	heightParams.amplitude = 30;
	heightParams.smoothness = 200;
	heightParams.heightOffset = 35;
	heightParams.roughness = 0.6;
	
	return heightParams;
}
