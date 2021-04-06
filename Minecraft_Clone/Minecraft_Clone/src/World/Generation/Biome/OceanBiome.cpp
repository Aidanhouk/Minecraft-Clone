#include "OceanBiome.h"

#include "../Structures/TreeGenerator.h"

OceanBiome::OceanBiome(int seed)
    : Biome(getNoiseParameters(), 80, 1000, seed)
{
	m_Id = BiomeId::OceanBiome;
}

ChunkBlock OceanBiome::getTopBlock(Rand &rand) const
{
	return BlockId::Sand;
}

ChunkBlock OceanBiome::getUnderGroundBlock(Rand & rand) const
{
	return BlockId::Dirt;
}

ChunkBlock OceanBiome::getUnderWaterBlock(Rand &rand) const
{
    return BlockId::Sand;
}

ChunkBlock OceanBiome::getPlant(Rand &rand) const
{
	return rand.intInRange(0, 10) > 8 ? BlockId::Rose : BlockId::TallGrass;
}

void OceanBiome::makeTree(Rand &rand, Chunk &chunk, int x, int y, int z) const
{
	makePalmTree(chunk, rand, x, y, z);
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
