#include "ClassicOverWorldGenerator.h"

#include <functional>
#include <iostream>

#include "../../../Maths/GeneralMaths.h"
#include "../../Chunk/Chunk.h"
#include "../Structures/TreeGenerator.h"
#include "../Structures/ClusterGenerator.h"
#include "../Caves/CavesGenerator.h"
#include "../Caves/WormCave.h"
#include "World/Block/BlockDatabase.h"
#include "../Seed.h"

#include <array>

namespace {
	bool outOfBounds(int x, int z)
	{
		return x < 0 || x >= CHUNK_SIZE || z < 0 || z >= CHUNK_SIZE;
	}
}

NoiseGenerator ClassicOverWorldGenerator::m_biomeNoiseGen(g_Seed);

ClassicOverWorldGenerator::ClassicOverWorldGenerator()
	: m_valleyBiome(g_Seed)
	, m_tundraBiome(g_Seed)
	, m_desertBiome(g_Seed)
	, m_oceanBiome(g_Seed)
	, m_forestBiome(g_Seed)
	, m_mountainsBiome(g_Seed)
{
	std::cout << "Seed " << g_Seed << "\n";
	setUpNoise();
}

void ClassicOverWorldGenerator::setUpNoise()
{
	static bool noiseGen = false;
	if (!noiseGen) {
		noiseGen = true;

		NoiseParameters biomeParmams;
		biomeParmams.octaves = 5;
		biomeParmams.amplitude = 1000;
		biomeParmams.smoothness = 2000;
		biomeParmams.heightOffset = 0;
		biomeParmams.roughness = 0.75;

		m_biomeNoiseGen.setParameters(biomeParmams);
	}
}

void ClassicOverWorldGenerator::generateTerrainFor(Chunk &chunk)
{
	m_pChunk = &chunk;

	auto location = chunk.getLocation();
	m_random.setSeed((location.x ^ location.y) << 2);

	getBiomeMap();
	getHeightMap();

	auto maxHeight = m_heightMap.getMaxValue();
	setBlocks(std::max(maxHeight, WATER_LEVEL));
}

int ClassicOverWorldGenerator::getMinimumSpawnHeight() const noexcept
{
	return WATER_LEVEL;
}

BiomeId ClassicOverWorldGenerator::getBiomeId(Chunk & chunk, int x, int z)
{
	auto location = chunk.getLocation();
	double height = m_biomeNoiseGen.getHeight(x % CHUNK_SIZE, z % CHUNK_SIZE, location.x, location.y);
	int biomeValue = static_cast<int>(height);
	return getBiome(biomeValue).getBiomeId();
}

void ClassicOverWorldGenerator::getBiomeMap()
{
	auto location = m_pChunk->getLocation();

	for (int x = 0; x < CHUNK_SIZE + 1; ++x) {
		for (int z = 0; z < CHUNK_SIZE + 1; ++z) {
			double h = m_biomeNoiseGen.getHeight(x, z, location.x, location.y);
			m_biomeMap.get(x, z) = static_cast<int>(h);
		}
	}
}

void ClassicOverWorldGenerator::getHeightMap()
{
	getHeightIn(0, 0, CHUNK_SIZE, CHUNK_SIZE);
	return;
	constexpr static auto HALF_CHUNK = CHUNK_SIZE / 2;
	constexpr static auto CHUNK = CHUNK_SIZE;

	getHeightIn(0, 0, HALF_CHUNK, HALF_CHUNK);
	getHeightIn(HALF_CHUNK, 0, CHUNK, HALF_CHUNK);
	getHeightIn(0, HALF_CHUNK, HALF_CHUNK, CHUNK);
	getHeightIn(HALF_CHUNK, HALF_CHUNK, CHUNK, CHUNK);
}

void ClassicOverWorldGenerator::getHeightIn(int xMin, int zMin, int xMax, int zMax)
{
	auto getHeightAt = [&](int x, int z) {
		const Biome &biome = getBiome(x, z);

		return biome.getHeight(x, z, m_pChunk->getLocation().x, m_pChunk->getLocation().y);
	};

	float bottomLeft = static_cast<float>(getHeightAt(xMin, zMin));
	float bottomRight = static_cast<float>(getHeightAt(xMax, zMin));
	float topLeft = static_cast<float>(getHeightAt(xMin, zMax));
	float topRight = static_cast<float>(getHeightAt(xMax, zMax));

	for (int x = xMin; x < xMax; ++x) {
		for (int z = zMin; z < zMax; ++z) {
			if (x == CHUNK_SIZE)
				continue;
			if (z == CHUNK_SIZE)
				continue;

			float h = smoothInterpolation(
				bottomLeft, topLeft, bottomRight, topRight,
				static_cast<float>(xMin), static_cast<float>(xMax),
				static_cast<float>(zMin), static_cast<float>(zMax),
				static_cast<float>(x), static_cast<float>(z));

			m_heightMap.get(x, z) = static_cast<int>(h);
		}
	}
}

void ClassicOverWorldGenerator::setBlocks(int maxHeight)
{
	std::vector<sf::Vector3i> trees;
	std::vector<sf::Vector3i> plants;
	std::vector<sf::Vector3i> beachPlants;

	for (int y = 0; y < maxHeight + 1; ++y) {
		for (int x = 0; x < CHUNK_SIZE; ++x) {
			for (int z = 0; z < CHUNK_SIZE; ++z) {

				int height = m_heightMap.get(x, z);
				auto &biome = getBiome(x, z);

				if (y > height) {
					if (y < WATER_LEVEL) {
						if (m_pChunk->getBlock(x, y, z) == 0)
							m_pChunk->setBlock(x, y, z, biome.getWaterBlock(m_random));
					}
					else if (y == WATER_LEVEL)
						if (m_pChunk->getBlock(x, y, z) == 0)
							m_pChunk->setBlock(x, y, z, biome.getWaterSurfaceBlock(m_random));
				}
				else if (y == height) {
					if (y >= WATER_LEVEL) {

						if (m_pChunk->getBlock(x, y, z) == 0) {
							// following code gets rid of straight lines of collidable blocks on water surface
							if (y == WATER_LEVEL) {
								int numberOfAdjBlocksOnSurface = -1;

								for (int xx = x - 1; xx <= x + 1; ++xx)
									for (int zz = z - 1; zz <= z + 1; ++zz)
										if (!outOfBounds(xx, zz))
											if (m_heightMap.get(xx, zz) >= WATER_LEVEL)
												++numberOfAdjBlocksOnSurface;

								if (numberOfAdjBlocksOnSurface >= 3)
									m_pChunk->setBlock(x, y, z, getBiome(x, z).getTopBlock(m_random, y));
								else
									m_pChunk->setBlock(x, y, z, getBiome(x, z).getWaterSurfaceBlock(m_random));
							}
							else
								m_pChunk->setBlock(x, y, z, getBiome(x, z).getTopBlock(m_random, y));
						}

						if (x == (CHUNK_SIZE / 2) && z == (CHUNK_SIZE / 2)) {
							if (m_random.intInRange(0, biome.getFlowerClusterFrequency()) == 0) {
								getBiome(x, z).makeFlowerCluster(m_random, *m_pChunk, x, z, m_heightMap);
							}
						}

						if (m_random.intInRange(0, biome.getTreeFrequency()) == 0) {
							trees.emplace_back(x, y + 1, z);
						}
						else if (m_random.intInRange(0, biome.getPlantFrequency()) == 0) {
							plants.emplace_back(x, y + 1, z);
						}
						else if (y == WATER_LEVEL && m_random.intInRange(0, biome.getBeachPlantFrequency()) == 0) {
							beachPlants.emplace_back(x, y + 1, z);
						}
					}
					else {
						if (m_random.intInRange(0, 100) <= 0)
							getBiome(x, z).makeUnderwaterCluster(m_random, *m_pChunk, x, z, m_heightMap);
						if (m_pChunk->getBlock(x, y, z) == 0)
							m_pChunk->setBlock(x, y, z, biome.getUnderWaterBlock(m_random));
						// place underwater plant
						//if (m_random.intInRange(0, biome.getPlantFrequency()) ==
						//	5) {
						//	plants.emplace_back(x, y + 1, z);
						//}
					}
				}
				else if (y >= height - 3) {
					if (m_pChunk->getBlock(x, y, z) == 0)
						m_pChunk->setBlock(x, y, z, getBiome(x, z).getUnderGroundBlock(m_random));
				}
				else if (y == 0) {
					m_pChunk->setBlock(x, y, z, { BlockId::Bedrock, 0x00 });
				}
				else if (y == 1) {
					if (m_pChunk->getBlock(x, y, z) == 0) {
						int rand = m_random.intInRange(0, 2);
						if (rand == 0)
							m_pChunk->setBlock(x, y, z, { BlockId::Stone, 0x00 });
						else
							m_pChunk->setBlock(x, y, z, { BlockId::Bedrock, 0x00 });
					}
				}
				else if (y == 2) {
					if (m_pChunk->getBlock(x, y, z) == 0) {
						int rand = m_random.intInRange(0, 2);
						if (rand == 0)
							m_pChunk->setBlock(x, y, z, { BlockId::Bedrock, 0x00 });
						else
							m_pChunk->setBlock(x, y, z, { BlockId::Stone, 0x00 });
					}
				}
				else {
					if (m_pChunk->getBlock(x, y, z) == 0) {
						m_pChunk->setBlock(x, y, z, { BlockId::Stone, 0x00 });
					}
				}
			}
		}
	}

	ClusterGenerator::makeOreClusters(*m_pChunk, m_random, m_heightMap);
	
	Caves::createCaves(*m_pChunk, m_random, m_heightMap);

	for (auto &plant : plants) {
		int x = plant.x;
		int z = plant.z;
		int y = plant.y;

		if (m_pChunk->getBlock(x, y, z) != 0)
			continue;
		auto block = getBiome(x, z).getPlant(m_random);

		if (BlockDatabase::canPlaceOnBlock(block.getData().id, m_pChunk->getBlock(x, y - 1, z).getData().id))
			m_pChunk->setBlock(x, y, z, block);
	}

	for (auto &tree : trees) {
		int x = tree.x;
		int z = tree.z;
		int y = tree.y;

		if (m_pChunk->getBlock(x, y, z) != 0)
			continue;
		if (m_pChunk->getBlock(x, y - 1, z).getData().id == BlockId::Water ||
			m_pChunk->getBlock(x, y - 1, z).getData().id == BlockId::Ice)
			continue;

		// doesn't create trees close to chunk edges otherwise leaves won't be created
		//if (x > 1 && x < CHUNK_SIZE - 2 && z > 1 && z < CHUNK_SIZE - 2)
			getBiome(x, z).makeTree(m_random, *m_pChunk, x, y, z);
	}

	for (auto &beachPlant : beachPlants) {
		int x = beachPlant.x;
		int z = beachPlant.z;
		int y = beachPlant.y;

		if (m_pChunk->getBlock(x, y, z) != 0)
			continue;
		if (m_pChunk->getBlock(x, y - 1, z).getData().id == BlockId::Water ||
			m_pChunk->getBlock(x, y - 1, z).getData().id == BlockId::Ice)
			continue;

		getBiome(x, z).makeBeachPlant(m_random, *m_pChunk, x, z, m_heightMap);
	}
}

const Biome &ClassicOverWorldGenerator::getBiome(int chunkX, int chunkZ) const
{
	int biomeValue = m_biomeMap.get(chunkX, chunkZ);

	if (biomeValue < 750) {
		return m_oceanBiome;
	}
	else if (biomeValue < 950) {
		return m_desertBiome;
	}
	else if (biomeValue < 1050) {
		return m_valleyBiome;
	}
	else if (biomeValue < 1250) {
		return m_forestBiome;
	}
	else if (biomeValue < 1450) {
		return m_mountainsBiome;
	}
	else if (biomeValue < 1850) {
		return m_tundraBiome;
	}
	else {
		return m_valleyBiome;
	}
}

const Biome & ClassicOverWorldGenerator::getBiome(int biomeValue) const
{
	if (biomeValue < 750) {
		return m_oceanBiome;
	}
	else if (biomeValue < 950) {
		return m_desertBiome;
	}
	else if (biomeValue < 1050) {
		return m_valleyBiome;
	}
	else if (biomeValue < 1250) {
		return m_forestBiome;
	}
	else if (biomeValue < 1450) {
		return m_mountainsBiome;
	}
	else if (biomeValue < 1850) {
		return m_tundraBiome;
	}
	else {
		return m_valleyBiome;
	}
}
