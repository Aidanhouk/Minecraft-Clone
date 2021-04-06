#include "MountainsBiome.h"

#include "../../WorldConstants.h"
#include "../Structures/TreeGenerator.h"

MountainBiome::MountainBiome(int seed)
	: Biome(getNoiseParameters(), 300, 300, seed)
{
	m_Id = BiomeId::MountainBiome;
}

ChunkBlock MountainBiome::getTopBlock(Rand &rand) const
{
	return BlockId::Snow;
	//return rand.intInRange(0, 10) > 5 ? BlockId::Snow : BlockId::Stone;
}

ChunkBlock MountainBiome::getUnderGroundBlock(Rand & rand) const
{
	return BlockId::Stone;
}

ChunkBlock MountainBiome::getUnderWaterBlock(Rand &rand) const
{
	return BlockId::Dirt;
}

ChunkBlock MountainBiome::getBeachBlock(Rand & rand) const
{
	return BlockId::Snow;
}

ChunkBlock MountainBiome::getUnderBeachBlock(Rand & rand) const
{
	return BlockId::Dirt;
}

ChunkBlock MountainBiome::getPlant(Rand &rand) const
{
	return BlockId::DeadShrub;
}

void MountainBiome::makeTree(Rand &rand, Chunk &chunk, int x, int y, int z) const
{
	if (y < 90)
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
