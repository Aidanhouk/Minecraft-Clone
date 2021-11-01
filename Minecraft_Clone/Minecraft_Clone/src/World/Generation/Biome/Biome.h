#pragma once

#include "../../../Maths/NoiseGenerator.h"
#include "../../../Util/Random.h"
#include "../../Block/ChunkBlock.h"
#include "Util/Array2D.h"
#include "World/WorldConstants.h"

enum class BiomeId
{
	ValleyBiome = 0,
	ForestBiome = 1,
	TundraBiome = 2,
	DesertBiome = 3,
	OceanBiome = 4,
	MountainBiome = 5,
};

using Rand = Random<std::minstd_rand>;

class Chunk;

struct Biome {
public:
    Biome(const NoiseParameters &parameters, int treeFreq, int plantFreq,
		int beackPlantFreq, int flowerClusterFreq, int seed);
    virtual ~Biome() = default;
	
	virtual ChunkBlock getTopBlock(Rand &rand, int y) const = 0;
	virtual ChunkBlock getUnderGroundBlock(Rand &rand) const = 0;
    virtual ChunkBlock getUnderWaterBlock(Rand &rand) const = 0;
	virtual ChunkBlock getWaterSurfaceBlock(Rand &rand) const;
	virtual ChunkBlock getWaterBlock(Rand &rand) const;

	virtual ChunkBlock getPlant(Rand &rand) const = 0;
	virtual void makeTree(Rand &rand, Chunk &chunk, int x, int y, int z) const = 0;
	virtual void makeBeachPlant(Rand &rand, Chunk &chunk, int x, int z,
		Array2D<int, CHUNK_SIZE> &heightMap) const { return; };
	virtual void makeFlowerCluster(Rand &rand, Chunk &chunk, int x, int z,
		Array2D<int, CHUNK_SIZE> &heightMap) const { return; };
	virtual void makeUnderwaterCluster(Rand &rand, Chunk &chunk, int x, int z,
		Array2D<int, CHUNK_SIZE> &heightMap) const { return; };

    int getHeight(int x, int z, int chunkX, int chunkZ) const;
    int getTreeFrequency() const noexcept;
    int getPlantFrequency() const noexcept;
	int getBeachPlantFrequency() const noexcept;
	int getFlowerClusterFrequency() const noexcept;
	BiomeId getBiomeId() const noexcept;

protected:
    virtual NoiseParameters getNoiseParameters() = 0;

	BiomeId m_Id;
private:
    NoiseGenerator m_heightGenerator;
    int m_treeFreq;
    int m_plantFreq;
	int m_beachPlantFreq;
	int m_flowerClusterFreq;
};