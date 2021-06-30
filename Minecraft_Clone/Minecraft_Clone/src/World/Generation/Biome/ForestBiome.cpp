#include "ForestBiome.h"

#include "../Structures/TreeGenerator.h"

ForestBiome::ForestBiome(int seed)
    : Biome(getNoiseParameters(), 20, 20, seed)
{
	m_Id = BiomeId::ForestBiome;
}

ChunkBlock ForestBiome::getTopBlock(Rand &rand) const
{
    return BlockId::Grass;
}

ChunkBlock ForestBiome::getUnderGroundBlock(Rand & rand) const
{
	return BlockId::Dirt;
}

ChunkBlock ForestBiome::getUnderWaterBlock(Rand &rand) const
{
    return rand.intInRange(0, 10) > 9 ? BlockId::Sand : BlockId::Dirt;
}

ChunkBlock ForestBiome::getBeachBlock(Rand & rand) const
{
	return BlockId::Grass;
}

ChunkBlock ForestBiome::getUnderBeachBlock(Rand & rand) const
{
	return BlockId::Dirt;
}

ChunkBlock ForestBiome::getPlant(Rand &rand) const
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

void ForestBiome::makeTree(Rand &rand, Chunk &chunk, int x, int y, int z) const
{
	rand.intInRange(0, 10) > 1 ? makeOakTree(chunk, rand, x, y, z) : makeBirchTree(chunk, rand, x, y, z);
}

void ForestBiome::makeBeachPlant(Rand &rand, Chunk &chunk, int x, int y, int z) const
{
	if (rand.intInRange(0, 10) > 9)
		makeSugarCane(chunk, rand, x, y, z);
}

NoiseParameters ForestBiome::getNoiseParameters()
{
	NoiseParameters heightParams;
	heightParams.octaves = 7;
	heightParams.amplitude = 30;
	heightParams.smoothness = 200;
	heightParams.heightOffset = 35;
	heightParams.roughness = 0.5;
	
	return heightParams;
}
