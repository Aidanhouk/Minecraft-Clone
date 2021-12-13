#include "OceanBiome.h"

#include "../Structures/TreeGenerator.h"
#include "../Structures/ClusterGenerator.h"

OceanBiome::OceanBiome(int seed)
	: Biome(getNoiseParameters(),
		150, // tree
		1000, // plant
		1000, // beachPlant
		1000, // flowerCluster
		seed)
{
	m_Id = BiomeId::OceanBiome;
}

ChunkBlock OceanBiome::getTopBlock(Rand &rand, int y) const
{
	return BlockId::Sand;
}

ChunkBlock OceanBiome::getUnderGroundBlock(Rand & rand) const
{
	return { BlockId::Dirt, 0x00 };
}

ChunkBlock OceanBiome::getUnderWaterBlock(Rand &rand) const
{
    return { BlockId::Sand, 0x00 };
}

ChunkBlock OceanBiome::getPlant(Rand & rand) const
{
	return BlockId::DeadShrub;
}

void OceanBiome::makeTree(Rand &rand, Chunk &chunk, int x, int y, int z) const
{
	makePalmTree(chunk, rand, x, y, z);
}

void OceanBiome::makeUnderwaterCluster(Rand & rand, Chunk & chunk, int x, int z, Array2D<int, CHUNK_SIZE>& heightMap) const
{
	int rnd = rand.intInRange(0, 2);

	if (rnd <= 0)
		ClusterGenerator::makeUnderwaterCluster(chunk, rand, x, z, heightMap, BlockId::Dirt);
	else if (rnd <= 1)
		ClusterGenerator::makeUnderwaterCluster(chunk, rand, x, z, heightMap, BlockId::Gravel);
	else if (rnd <= 2)
		ClusterGenerator::makeUnderwaterCluster(chunk, rand, x, z, heightMap, BlockId::Clay);
}

NoiseParameters OceanBiome::getNoiseParameters()
{
	NoiseParameters heightParams;
	heightParams.octaves = 7;
	heightParams.amplitude = 43;
	heightParams.smoothness = 55;
	heightParams.heightOffset = 0;
	heightParams.roughness = 0.50;
	
	return heightParams;
}
