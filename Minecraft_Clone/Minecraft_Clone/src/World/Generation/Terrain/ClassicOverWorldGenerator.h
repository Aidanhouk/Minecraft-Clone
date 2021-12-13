#pragma once

#include "TerrainGenerator.h"

#include "../../../Util/Array2D.h"
#include "../../../Util/Random.h"

#include "../../../Maths/NoiseGenerator.h"
#include "../../World/WorldConstants.h"

#include "../Biome/DesertBiome.h"
#include "../Biome/ValleyBiome.h"
#include "../Biome/ForestBiome.h"
#include "../Biome/OceanBiome.h"
#include "../Biome/TundraBiome.h"
#include "../Biome/MountainsBiome.h"

#include <array>
#include <iostream>

class Chunk;

class ClassicOverWorldGenerator : public TerrainGenerator {
public:
	ClassicOverWorldGenerator();
	
	void generateTerrainFor(Chunk &chunk) override;
	int getMinimumSpawnHeight() const noexcept override;

	BiomeId getBiomeId(Chunk &chunk, int x, int z) override;
private:
	static void setUpNoise();
	
	void setBlocks(int maxHeight);
	
	void getHeightIn(int xMin, int zMin, int xMax, int zMax);
	void getHeightMap();
	void getBiomeMap();

	const Biome &getBiome(int chunkX, int chunkZ) const;
	const Biome &getBiome(int biomeValue) const;
	
	Array2D<int, CHUNK_SIZE> m_heightMap;
	Array2D<int, CHUNK_SIZE + 1> m_biomeMap;
	
	Random<std::minstd_rand> m_random;
	
	static NoiseGenerator m_biomeNoiseGen;
	
	ValleyBiome m_valleyBiome;
	TundraBiome m_tundraBiome;
	DesertBiome m_desertBiome;
	OceanBiome m_oceanBiome;
	ForestBiome m_forestBiome;
	MountainBiome m_mountainsBiome;
	
	Chunk *m_pChunk = nullptr;
};