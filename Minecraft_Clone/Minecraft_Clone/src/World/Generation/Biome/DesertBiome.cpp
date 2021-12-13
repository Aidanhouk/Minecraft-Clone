#include "DesertBiome.h"

#include "../Structures/TreeGenerator.h"
#include "../Structures/ClusterGenerator.h"

DesertBiome::DesertBiome(int seed)
	: Biome(getNoiseParameters(),
		650, // tree
		500, // plant
		50, // beachPlant
		1000, // flowerCluster
		seed)
{
	m_Id = BiomeId::DesertBiome;
}

ChunkBlock DesertBiome::getTopBlock(Rand &rand, int y) const
{
    return BlockId::Sand;
}

ChunkBlock DesertBiome::getUnderGroundBlock(Rand & rand) const
{
	return { BlockId::Sand, 0x00 };
}

ChunkBlock DesertBiome::getUnderWaterBlock(Rand &rand) const
{
    return { BlockId::Sand, 0x00 };
}

ChunkBlock DesertBiome::getPlant(Rand &rand) const
{
	return BlockId::DeadShrub;
}

void DesertBiome::makeTree(Rand &rand, Chunk &chunk, int x, int y, int z) const
{
    if (y < WATER_LEVEL + 5) {
		if (rand.intInRange(1, 10) <= 2) {
			makeCactus(chunk, rand, x, y, z);
		}
		else {
			makePalmTree(chunk, rand, x, y, z);
		}
    }
    else {
		if (rand.intInRange(1, 10) > 2) {
			makeCactus(chunk, rand, x, y, z);
		}
		else {
			makePalmTree(chunk, rand, x, y, z);
		}
    }
}

void DesertBiome::makeUnderwaterCluster(Rand & rand, Chunk & chunk, int x, int z, Array2D<int, CHUNK_SIZE>& heightMap) const
{
	int rnd = rand.intInRange(0, 2);

	if (rnd <= 0)
		ClusterGenerator::makeUnderwaterCluster(chunk, rand, x, z, heightMap, BlockId::Dirt);
	else if (rnd <= 1)
		ClusterGenerator::makeUnderwaterCluster(chunk, rand, x, z, heightMap, BlockId::Gravel);
	else if (rnd <= 2)
		ClusterGenerator::makeUnderwaterCluster(chunk, rand, x, z, heightMap, BlockId::Clay);
}

NoiseParameters DesertBiome::getNoiseParameters()
{
	NoiseParameters heightParams;
	heightParams.octaves = 7;
	heightParams.amplitude = 10;
	heightParams.smoothness = 300;
	heightParams.heightOffset = 57;
	heightParams.roughness = 0.7f;
	
	return heightParams;
}
