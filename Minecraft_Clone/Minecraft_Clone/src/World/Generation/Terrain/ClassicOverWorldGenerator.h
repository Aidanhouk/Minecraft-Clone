#ifndef CLASSICOVERWORLDGENERATOR_H_INCLUDED
#define CLASSICOVERWORLDGENERATOR_H_INCLUDED

#include "TerrainGenerator.h"

#include "../../../Util/Array2D.h"
#include "../../../Util/Random.h"

#include "../../../Maths/NoiseGenerator.h"
#include "../../WorldConstants.h"

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
	//~ClassicOverWorldGenerator() {
	//	std::cout << "\n";
	//	
	//	for (int i = 0; i < m_ARR.size(); ++i) {
	//		std::cout << i << " " << m_ARR[i] << "\n";
	//	}
	//	std::cout << "\n";
	//}
	
	void generateTerrainFor(Chunk &chunk) override;
	int getMinimumSpawnHeight() const noexcept override;
private:
	static void setUpNoise();
	
	void setBlocks(int maxHeight);
	
	void getHeightIn(int xMin, int zMin, int xMax, int zMax);
	void getHeightMap();
	void getBiomeMap();
	
	const Biome &getBiome(int x, int z) const;
	
	Array2D<int, CHUNK_SIZE> m_heightMap;
	Array2D<int, CHUNK_SIZE + 1> m_biomeMap;
	
	Random<std::minstd_rand> m_random;
	
	static NoiseGenerator m_biomeNoiseGen;
	
	ValleyBiome m_valleyBiome;
	TundraBiome m_tundraBiome;
	DesertBiome m_desertBiome;
	OceanBiome m_oceanBiome;
	ForestBiome m_forestBiome;
	MountainBiome m_mountaineBiome;
	
	Chunk *m_pChunk = nullptr;

	//mutable std::array<int, 500> m_ARR{ 0 };
};

#endif // CLASSICOVERWORLDGENERATOR_H_INCLUDED
