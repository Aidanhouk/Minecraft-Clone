#include "ValleyBiome.h"

#include "../Structures/TreeGenerator.h"
#include "../Structures/ClusterGenerator.h"
#include "World/Block/BlockDatabase.h"

ValleyBiome::ValleyBiome(int seed)
	: Biome(getNoiseParameters(),
		1100, // tree
		10, // plant
		150, // beachPlant
		4, // flowerCluster
		seed)
{
	m_Id = BiomeId::ValleyBiome;
}

ChunkBlock ValleyBiome::getTopBlock(Rand &rand, int y) const
{
    return BlockId::Grass;
}

ChunkBlock ValleyBiome::getUnderGroundBlock(Rand & rand) const
{
	return { BlockId::Dirt, 0x00 };
}

ChunkBlock ValleyBiome::getUnderWaterBlock(Rand &rand) const
{
    return { BlockId::Dirt, 0x00 };
}

ChunkBlock ValleyBiome::getPlant(Rand &rand) const
{
	int rnd = rand.intInRange(0, 50);
	if (rnd == 0) {
		return BlockDatabase::getRandomFlower(rand);
	}
	else {
		return BlockId::TallGrass;
	}
}

void ValleyBiome::makeTree(Rand &rand, Chunk &chunk, int x, int y, int z) const
{
	int rnd = rand.intInRange(0, 20);
	if (rnd <= 1)
		makeBirchTree(chunk, rand, x, y, z);
	else if (rnd <= 10)
		makeOakTree(chunk, rand, x, y, z);
	else
		makeDoublePlant(chunk, x, y, z, BlockDatabase::getRandomDoubleFlower(rand));
}

void ValleyBiome::makeBeachPlant(Rand &rand, Chunk &chunk, int x, int z, Array2D<int, CHUNK_SIZE> &heightMap) const
{
	makeSugarCane(chunk, rand, x, z, heightMap);
}

void ValleyBiome::makeFlowerCluster(Rand &rand, Chunk &chunk, int x, int z, Array2D<int, CHUNK_SIZE> &heightMap) const
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

void ValleyBiome::makeUnderwaterCluster(Rand & rand, Chunk & chunk, int x, int z, Array2D<int, CHUNK_SIZE>& heightMap) const
{
	int rnd = rand.intInRange(0, 2);

	if (rnd <= 0)
		ClusterGenerator::makeUnderwaterCluster(chunk, rand, x, z, heightMap, BlockId::Sand);
	else if (rnd <= 1)
		ClusterGenerator::makeUnderwaterCluster(chunk, rand, x, z, heightMap, BlockId::Gravel);
	else if (rnd <= 2)
		ClusterGenerator::makeUnderwaterCluster(chunk, rand, x, z, heightMap, BlockId::Clay);
}

NoiseParameters ValleyBiome::getNoiseParameters()
{
	NoiseParameters heightParams;
	heightParams.octaves = 9;
	heightParams.amplitude = 30;
	heightParams.smoothness = 800;
	heightParams.heightOffset = 25;
	heightParams.roughness = 0.7f;
	
	return heightParams;
}
