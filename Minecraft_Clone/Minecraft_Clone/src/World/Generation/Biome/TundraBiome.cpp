#include "TundraBiome.h"

#include "../Structures/TreeGenerator.h"
#include "../Structures/ClusterGenerator.h"
#include "World/Block/BlockDatabase.h"

TundraBiome::TundraBiome(int seed)
    : Biome(getNoiseParameters(),
		19, // tree
		20, // plant
		100, // beachPlant
		3, // flowerCluster
		seed)
{
	m_Id = BiomeId::TundraBiome;
}

ChunkBlock TundraBiome::getTopBlock(Rand &rand, int y) const
{
	return BlockId::TundraGrass;
}

ChunkBlock TundraBiome::getUnderGroundBlock(Rand & rand) const
{
	return { BlockId::Dirt, 0x00 };
}

ChunkBlock TundraBiome::getUnderWaterBlock(Rand &rand) const
{
    return { BlockId::Dirt, 0x00 };
}

ChunkBlock TundraBiome::getWaterSurfaceBlock(Rand & rand) const
{
	return BlockId::Ice;
}

ChunkBlock TundraBiome::getPlant(Rand &rand) const
{
	int rnd = rand.intInRange(0, 50);
	if (rnd == 0) {
		return BlockDatabase::getRandomFlower(rand);
	}
	else {
		return BlockId::Fern;
	}
}

void TundraBiome::makeTree(Rand &rand, Chunk &chunk, int x, int y, int z) const
{
	int rnd = rand.intInRange(0, 1);
	if (rnd == 0)
		makeSpruceTree(chunk, rand, x, y, z);
	else
		makeDoublePlant(chunk, x, y, z, BlockId::LargeFern);
}

void TundraBiome::makeBeachPlant(Rand & rand, Chunk & chunk, int x, int z, Array2D<int, CHUNK_SIZE>& heightMap) const
{
	makeSugarCane(chunk, rand, x, z, heightMap);
}

void TundraBiome::makeFlowerCluster(Rand &rand, Chunk &chunk, int x, int z, Array2D<int, CHUNK_SIZE> &heightMap) const
{
	int typeOfCluster = rand.intInRange(0, 15);

	if (typeOfCluster == 0) {
		ClusterGenerator::makeFlowerWitchCircle(chunk, rand, x, z, heightMap,
			BlockDatabase::getRandomFlower(rand));
	}
	else if (typeOfCluster <= 10) {
		int radius = rand.intInRange(3, 4);
		int frequency = rand.intInRange(15, 20);
		ClusterGenerator::makeFlowerCluster(chunk, rand, x, z, heightMap,
			BlockDatabase::getRandomFlower(rand), radius, frequency);
	}
	else {
		int radius = rand.intInRange(3, 4);
		ClusterGenerator::makeDoubleFlowerCluster(chunk, rand, x, z, heightMap,
			BlockDatabase::getRandomDoubleFlower(rand), radius, 10);
	}
}

void TundraBiome::makeUnderwaterCluster(Rand & rand, Chunk & chunk, int x, int z, Array2D<int, CHUNK_SIZE>& heightMap) const
{
	int rnd = rand.intInRange(0, 2);

	if (rnd <= 0)
		ClusterGenerator::makeUnderwaterCluster(chunk, rand, x, z, heightMap, BlockId::Sand);
	else if (rnd <= 1)
		ClusterGenerator::makeUnderwaterCluster(chunk, rand, x, z, heightMap, BlockId::Gravel);
	else if (rnd <= 2)
		ClusterGenerator::makeUnderwaterCluster(chunk, rand, x, z, heightMap, BlockId::Clay);
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
