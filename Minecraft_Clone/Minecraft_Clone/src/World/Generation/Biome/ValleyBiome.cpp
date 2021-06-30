#include "ValleyBiome.h"

#include "../Structures/TreeGenerator.h"

ValleyBiome::ValleyBiome(int seed)
    : Biome(getNoiseParameters(), 1000, 10, seed)
{
	m_Id = BiomeId::ValleyBiome;
}

ChunkBlock ValleyBiome::getTopBlock(Rand &rand) const
{
    return BlockId::Grass;
}

ChunkBlock ValleyBiome::getUnderGroundBlock(Rand & rand) const
{
	return BlockId::Dirt;
}

ChunkBlock ValleyBiome::getUnderWaterBlock(Rand &rand) const
{
    return rand.intInRange(0, 10) > 8 ? BlockId::Dirt : BlockId::Sand;
}

ChunkBlock ValleyBiome::getBeachBlock(Rand &rand) const
{
	return BlockId::Grass;
}

ChunkBlock ValleyBiome::getUnderBeachBlock(Rand & rand) const
{
	return BlockId::Dirt;
}

ChunkBlock ValleyBiome::getPlant(Rand &rand) const
{
	int intRand = rand.intInRange(0, 10);
	if (intRand < 1) {
		return BlockId::Rose;
	}
	else if (intRand < 2) {
		return BlockId::Dandelion;
	}
	else {
		return BlockId::TallGrass;
	}
}

void ValleyBiome::makeTree(Rand &rand, Chunk &chunk, int x, int y, int z) const
{
    makeOakTree(chunk, rand, x, y, z);
}

void ValleyBiome::makeBeachPlant(Rand &rand, Chunk &chunk, int x, int y, int z) const
{
	if (rand.intInRange(0, 10) > 6)
		makeSugarCane(chunk, rand, x, y, z);
}

NoiseParameters ValleyBiome::getNoiseParameters()
{
	NoiseParameters heightParams;
	heightParams.octaves = 9;
	heightParams.amplitude = 30;
	heightParams.smoothness = 800;
	heightParams.heightOffset = 35;
	heightParams.roughness = 0.7f;
	
	return heightParams;
}
