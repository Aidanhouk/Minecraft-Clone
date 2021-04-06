#ifndef BIOME_H_INCLUDED
#define BIOME_H_INCLUDED

#include "../../../Maths/NoiseGenerator.h"
#include "../../../Util/Random.h"
#include "../../Block/ChunkBlock.h"

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
    Biome(const NoiseParameters &parameters, int treeFreq, int plantFreq, int seed);
    virtual ~Biome() = default;
	
	virtual ChunkBlock getTopBlock(Rand &rand) const = 0;
	virtual ChunkBlock getUnderGroundBlock(Rand &rand) const = 0;
    virtual ChunkBlock getUnderWaterBlock(Rand &rand) const = 0;
	virtual ChunkBlock getBeachBlock(Rand &rand) const;
	virtual ChunkBlock getUnderBeachBlock(Rand &rand) const;

	virtual ChunkBlock getPlant(Rand &rand) const = 0;
	virtual void makeTree(Rand &rand, Chunk &chunk, int x, int y, int z) const = 0;
	// tree or plant that grows near water
	virtual void makeBeachPlant(Rand &rand, Chunk &chunk, int x, int y, int z) const { return; };

    int getHeight(int x, int z, int chunkX, int chunkZ) const;
    int getTreeFrequency() const noexcept;
    int getPlantFrequency() const noexcept;
	BiomeId getBiomeId() const noexcept;

protected:
    virtual NoiseParameters getNoiseParameters() = 0;

	BiomeId m_Id;
private:
    NoiseGenerator m_heightGenerator;
    int m_treeFreq;
    int m_plantFreq;
};

#endif // BIOME_H_INCLUDED
