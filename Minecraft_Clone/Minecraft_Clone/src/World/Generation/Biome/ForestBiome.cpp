#include "ForestBiome.h"

#include "../Structures/TreeGenerator.h"
#include "../Structures/ClusterGenerator.h"
#include "World/Block/BlockDatabase.h"

ForestBiome::ForestBiome(int seed)
	: Biome(getNoiseParameters(),
		36, // tree
		20, // plant
		100, // beachPlant
		3, // flowerCluster
		seed)
{
	m_Id = BiomeId::ForestBiome;
}

ChunkBlock ForestBiome::getTopBlock(Rand &rand, int y) const
{
    return BlockId::Grass;
}

ChunkBlock ForestBiome::getUnderGroundBlock(Rand & rand) const
{
	return { BlockId::Dirt, 0x00 };
}

ChunkBlock ForestBiome::getUnderWaterBlock(Rand &rand) const
{
    return { BlockId::Dirt, 0x00 };
}

ChunkBlock ForestBiome::getPlant(Rand &rand) const
{
	int rnd = rand.intInRange(0, 50);
	if (rnd == 0) {
		return BlockDatabase::getRandomFlower(rand);
	}
	else {
		return BlockId::TallGrass;
	}
}

void ForestBiome::makeTree(Rand &rand, Chunk &chunk, int x, int y, int z) const
{
	int rnd = rand.intInRange(0, 50);
	if (rnd == 0) 
		makeDoublePlant(chunk, x, y, z, BlockDatabase::getRandomDoubleFlower(rand));
	else if (rnd < 35)
		makeOakTree(chunk, rand, x, y, z);
	else
		makeBirchTree(chunk, rand, x, y, z);
}

void ForestBiome::makeBeachPlant(Rand &rand, Chunk &chunk, int x, int z, Array2D<int, CHUNK_SIZE> &heightMap) const
{
	makeSugarCane(chunk, rand, x, z, heightMap);
}

void ForestBiome::makeFlowerCluster(Rand & rand, Chunk & chunk, int x, int z, Array2D<int, CHUNK_SIZE>& heightMap) const
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

void ForestBiome::makeUnderwaterCluster(Rand & rand, Chunk & chunk, int x, int z, Array2D<int, CHUNK_SIZE>& heightMap) const
{
	int rnd = rand.intInRange(0, 2);

	if (rnd <= 0)
		ClusterGenerator::makeUnderwaterCluster(chunk, rand, x, z, heightMap, BlockId::Sand);
	else if (rnd <= 1)
		ClusterGenerator::makeUnderwaterCluster(chunk, rand, x, z, heightMap, BlockId::Gravel);
	else if (rnd <= 2)
		ClusterGenerator::makeUnderwaterCluster(chunk, rand, x, z, heightMap, BlockId::Clay);
}

NoiseParameters ForestBiome::getNoiseParameters()
{
	NoiseParameters heightParams;
	heightParams.octaves = 7;
	heightParams.amplitude = 30;
	heightParams.smoothness = 100;
	heightParams.heightOffset = 35;
	heightParams.roughness = 0.5;
	
	return heightParams;
}
