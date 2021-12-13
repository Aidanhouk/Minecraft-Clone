#include "MountainsBiome.h"

#include "../Structures/TreeGenerator.h"

MountainBiome::MountainBiome(int seed)
	: Biome(getNoiseParameters(),
		550, // tree
		300, // plant
		1000, // beachPlant
		1000, // flowerCluster
		seed)
{
	m_Id = BiomeId::MountainBiome;
}

ChunkBlock MountainBiome::getTopBlock(Rand &rand, int y) const
{
	return BlockId::Snow;
}

ChunkBlock MountainBiome::getUnderGroundBlock(Rand & rand) const
{
	return { BlockId::Stone, 0x00 };
}

ChunkBlock MountainBiome::getUnderWaterBlock(Rand &rand) const
{
	return { BlockId::Dirt, 0x00 };
}

ChunkBlock MountainBiome::getWaterSurfaceBlock(Rand & rand) const
{
	return BlockId::Ice;
}

ChunkBlock MountainBiome::getPlant(Rand &rand) const
{
	return BlockId::DeadShrub;
}

void MountainBiome::makeTree(Rand &rand, Chunk &chunk, int x, int y, int z) const
{
	makeSpruceTree(chunk, rand, x, y, z);
}

NoiseParameters MountainBiome::getNoiseParameters()
{
	NoiseParameters heightParams;
	heightParams.octaves = 3;
	heightParams.amplitude = 50;
	heightParams.smoothness = 100;
	heightParams.heightOffset = 20;
	heightParams.roughness = 0.99f;

	return heightParams;
}
