#include "Biome.h"

Biome::Biome(const NoiseParameters &parameters, int treeFreq, int plantFreq,
	int beackPlantFreq, int flowerClusterFreq, int seed)
    : m_heightGenerator(seed)
    , m_treeFreq(treeFreq)
    , m_plantFreq(plantFreq)
	, m_beachPlantFreq(beackPlantFreq)
	, m_flowerClusterFreq(flowerClusterFreq)
{
    m_heightGenerator.setParameters(parameters);
}

ChunkBlock Biome::getWaterSurfaceBlock(Rand & rand) const
{
	return BlockId::Water;
}

ChunkBlock Biome::getWaterBlock(Rand & rand) const
{
	return BlockId::Water;
}

int Biome::getHeight(int x, int z, int chunkX, int chunkZ) const
{
    return m_heightGenerator.getHeight(x, z, chunkX, chunkZ);
}

int Biome::getTreeFrequency() const noexcept
{
    return m_treeFreq;
}

int Biome::getPlantFrequency() const noexcept
{
    return m_plantFreq;
}

int Biome::getBeachPlantFrequency() const noexcept
{
	return m_beachPlantFreq;
}

int Biome::getFlowerClusterFrequency() const noexcept
{
	return m_flowerClusterFreq;
}

BiomeId Biome::getBiomeId() const noexcept
{
	return m_Id;
}
