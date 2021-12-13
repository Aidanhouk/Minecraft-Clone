#include "ClusterGenerator.h"

#include "../../Chunk/Chunk.h"
#include "TreeGenerator.h"
#include "World/Block/BlockDatabase.h"

#include <iostream>
#include <vector>
#include <array>

namespace {
	bool outOfBounds(int x, int z)
	{
		return (x < 0 || x >= CHUNK_SIZE || z < 0 || z >= CHUNK_SIZE);
	}

	int distanceFromChunkEdge(int x, int z)
	{
		int minX = std::min(x, CHUNK_SIZE - 1 - x);
		int minZ = std::min(z, CHUNK_SIZE - 1 - z);

		return std::min(minX, minZ);
	}
}

namespace COORDS {

	const std::array<sf::Vector2i, 16> WITCH_CIRCLE{

		sf::Vector2i{-1, -3},
		sf::Vector2i{0, -3},
		sf::Vector2i{1, -3},

		sf::Vector2i{2, -2},

		sf::Vector2i{3, -1},
		sf::Vector2i{3, 0},
		sf::Vector2i{3, 1},

		sf::Vector2i{2, 2},

		sf::Vector2i{1, 3},
		sf::Vector2i{0, 3},
		sf::Vector2i{-1, 3},

		sf::Vector2i{-2, 2},

		sf::Vector2i{-3, 1},
		sf::Vector2i{-3, 0},
		sf::Vector2i{-3, -1},

		sf::Vector2i{-2, -2},
	};

	const std::array<sf::Vector2i, 13> CLUSTER_2R_1{

		sf::Vector2i{-1, -1},
		sf::Vector2i{-1,  0},
		sf::Vector2i{-1,  1},

		sf::Vector2i{0, -1},
		sf::Vector2i{0,  0},
		sf::Vector2i{0,  1},

		sf::Vector2i{1, -1},
		sf::Vector2i{1,  0},
		sf::Vector2i{1,  1},

		sf::Vector2i{-2, 0},
		sf::Vector2i{ 2, 0},
		sf::Vector2i{0, -2},
		sf::Vector2i{0,  2},
	};

	const std::array<sf::Vector2i, 29> CLUSTER_3R_1{

		sf::Vector2i{-2, -2},
		sf::Vector2i{-2, -1},
		sf::Vector2i{-2,  0},
		sf::Vector2i{-2,  1},
		sf::Vector2i{-2,  2},

		sf::Vector2i{-1, -2},
		sf::Vector2i{-1, -1},
		sf::Vector2i{-1,  0},
		sf::Vector2i{-1,  1},
		sf::Vector2i{-1,  2},

		sf::Vector2i{0, -2},
		sf::Vector2i{0, -1},
		sf::Vector2i{0,  0},
		sf::Vector2i{0,  1},
		sf::Vector2i{0,  2},

		sf::Vector2i{1, -2},
		sf::Vector2i{1, -1},
		sf::Vector2i{1,  0},
		sf::Vector2i{1,  1},
		sf::Vector2i{1,  2},

		sf::Vector2i{2, -2},
		sf::Vector2i{2, -1},
		sf::Vector2i{2,  0},
		sf::Vector2i{2,  1},
		sf::Vector2i{2,  2},

		sf::Vector2i{-3, 0},
		sf::Vector2i{ 3, 0},
		sf::Vector2i{0, -3},
		sf::Vector2i{0,  3},
	};

	const std::array<sf::Vector2i, 49> CLUSTER_4R_1{

		sf::Vector2i{-3, -2},
		sf::Vector2i{-3, -1},
		sf::Vector2i{-3,  0},
		sf::Vector2i{-3,  1},
		sf::Vector2i{-3,  2},

		sf::Vector2i{-2, -3},
		sf::Vector2i{-2, -2},
		sf::Vector2i{-2, -1},
		sf::Vector2i{-2,  0},
		sf::Vector2i{-2,  1},
		sf::Vector2i{-2,  2},
		sf::Vector2i{-2,  3},

		sf::Vector2i{-1, -3},
		sf::Vector2i{-1, -2},
		sf::Vector2i{-1, -1},
		sf::Vector2i{-1,  0},
		sf::Vector2i{-1,  1},
		sf::Vector2i{-1,  2},
		sf::Vector2i{-1,  3},

		sf::Vector2i{0, -3},
		sf::Vector2i{0, -2},
		sf::Vector2i{0, -1},
		sf::Vector2i{0,  0},
		sf::Vector2i{0,  1},
		sf::Vector2i{0,  2},
		sf::Vector2i{0,  3},

		sf::Vector2i{1, -3},
		sf::Vector2i{1, -2},
		sf::Vector2i{1, -1},
		sf::Vector2i{1,  0},
		sf::Vector2i{1,  1},
		sf::Vector2i{1,  2},
		sf::Vector2i{1,  3},

		sf::Vector2i{2, -3},
		sf::Vector2i{2, -2},
		sf::Vector2i{2, -1},
		sf::Vector2i{2,  0},
		sf::Vector2i{2,  1},
		sf::Vector2i{2,  2},
		sf::Vector2i{2,  3},

		sf::Vector2i{3, -2},
		sf::Vector2i{3, -1},
		sf::Vector2i{3,  0},
		sf::Vector2i{3,  1},
		sf::Vector2i{3,  2},

		sf::Vector2i{-4, 0},
		sf::Vector2i{ 4, 0},
		sf::Vector2i{0, -4},
		sf::Vector2i{0,  4},
	};

	const std::array<sf::Vector2i, 81> CLUSTER_5R_1{

		sf::Vector2i{-4, -3},
		sf::Vector2i{-4, -2},
		sf::Vector2i{-4, -1},
		sf::Vector2i{-4,  0},
		sf::Vector2i{-4,  1},
		sf::Vector2i{-4,  2},
		sf::Vector2i{-4,  3},

		sf::Vector2i{-3, -4},
		sf::Vector2i{-3, -3},
		sf::Vector2i{-3, -2},
		sf::Vector2i{-3, -1},
		sf::Vector2i{-3,  0},
		sf::Vector2i{-3,  1},
		sf::Vector2i{-3,  2},
		sf::Vector2i{-3,  3},
		sf::Vector2i{-3,  4},

		sf::Vector2i{-2, -4},
		sf::Vector2i{-2, -3},
		sf::Vector2i{-2, -2},
		sf::Vector2i{-2, -1},
		sf::Vector2i{-2,  0},
		sf::Vector2i{-2,  1},
		sf::Vector2i{-2,  2},
		sf::Vector2i{-2,  3},
		sf::Vector2i{-2,  4},

		sf::Vector2i{-1, -4},
		sf::Vector2i{-1, -3},
		sf::Vector2i{-1, -2},
		sf::Vector2i{-1, -1},
		sf::Vector2i{-1,  0},
		sf::Vector2i{-1,  1},
		sf::Vector2i{-1,  2},
		sf::Vector2i{-1,  3},
		sf::Vector2i{-1,  4},

		sf::Vector2i{0, -4},
		sf::Vector2i{0, -3},
		sf::Vector2i{0, -2},
		sf::Vector2i{0, -1},
		sf::Vector2i{0,  0},
		sf::Vector2i{0,  1},
		sf::Vector2i{0,  2},
		sf::Vector2i{0,  3},
		sf::Vector2i{0,  4},

		sf::Vector2i{1, -4},
		sf::Vector2i{1, -3},
		sf::Vector2i{1, -2},
		sf::Vector2i{1, -1},
		sf::Vector2i{1,  0},
		sf::Vector2i{1,  1},
		sf::Vector2i{1,  2},
		sf::Vector2i{1,  3},
		sf::Vector2i{1,  4},

		sf::Vector2i{2, -4},
		sf::Vector2i{2, -3},
		sf::Vector2i{2, -2},
		sf::Vector2i{2, -1},
		sf::Vector2i{2,  0},
		sf::Vector2i{2,  1},
		sf::Vector2i{2,  2},
		sf::Vector2i{2,  3},
		sf::Vector2i{2,  4},

		sf::Vector2i{3, -4},
		sf::Vector2i{3, -3},
		sf::Vector2i{3, -2},
		sf::Vector2i{3, -1},
		sf::Vector2i{3,  0},
		sf::Vector2i{3,  1},
		sf::Vector2i{3,  2},
		sf::Vector2i{3,  3},
		sf::Vector2i{3,  4},

		sf::Vector2i{4, -3},
		sf::Vector2i{4, -2},
		sf::Vector2i{4, -1},
		sf::Vector2i{4,  0},
		sf::Vector2i{4,  1},
		sf::Vector2i{4,  2},
		sf::Vector2i{4,  3},

		sf::Vector2i{-5, 0},
		sf::Vector2i{ 5, 0},
		sf::Vector2i{0, -5},
		sf::Vector2i{0,  5},
	};

}

enum ORE_IDICES
{
	Coal = 0,
	Iron = 1,
	Redstone = 2,
	Gold = 3,
	Diamond = 4,
	Emerald = 5,

	NUMBER_OF_ORES,
};

const std::array<int, ORE_IDICES::NUMBER_OF_ORES> ORE_SPAWN_TIMES
{
	25, // coal 
	25, // iron
	10, // redstone
	3, // gold
	1, // diamond
	1, // emerald. 0.2 in real minecraft
};

using LayersMinMax = std::array<int, 2>;
const std::array<LayersMinMax, ORE_IDICES::NUMBER_OF_ORES> ORE_SPAWN_LAYERS
{
	LayersMinMax{5, 128}, // coal 
	LayersMinMax{5, 54}, // iron
	LayersMinMax{1, 12}, // redstone
	LayersMinMax{1, 29}, // gold
	LayersMinMax{1, 12}, // diamond
	LayersMinMax{1, 29}, // emerald
};

namespace CreateOreVein {

	void setCoalOreBlocks(Chunk &chunk, Random<std::minstd_rand> &rand, int x, int y, int z)
	{
		for (int xx = x - 1; xx <= x + 1; ++xx)
			for (int yy = y - 1; yy <= y + 1; ++yy)
				for (int zz = z - 1; zz <= z + 1; ++zz) {
					if (chunk.getBlock(xx, yy, zz) != 0 &&
						chunk.getBlock(xx, yy, zz).getData().id != BlockId::Bedrock)
						if (rand.intInRange(1, 27) <= 10)
							chunk.setBlock(xx, yy, zz, { BlockId::CoalOre, 0x00 });
				}
	}

	void setIronOreBlocks(Chunk &chunk, Random<std::minstd_rand> &rand, int x, int y, int z)
	{
		for (int xx = x - 1; xx <= x; ++xx)
			for (int yy = y - 1; yy <= y; ++yy)
				for (int zz = z - 1; zz <= z; ++zz) {
					if (chunk.getBlock(xx, yy, zz) != 0 &&
						chunk.getBlock(xx, yy, zz).getData().id != BlockId::Bedrock)
						if (rand.intInRange(1, 16) <= 11)
							chunk.setBlock(xx, yy, zz, { BlockId::IronOre, 0x00 });
				}
	}

	void setRedstoneOreBlocks(Chunk &chunk, Random<std::minstd_rand> &rand, int x, int y, int z)
	{
		for (int xx = x - 1; xx <= x; ++xx)
			for (int yy = y - 1; yy <= y; ++yy)
				for (int zz = z - 1; zz <= z; ++zz) {
					if (chunk.getBlock(xx, yy, zz) != 0 &&
						chunk.getBlock(xx, yy, zz).getData().id != BlockId::Bedrock)
						if (rand.intInRange(1, 24) <= 11)
							chunk.setBlock(xx, yy, zz, { BlockId::RedstoneOre, 0x00 });
				}
	}

	void setGoldOreBlocks(Chunk &chunk, Random<std::minstd_rand> &rand, int x, int y, int z)
	{
		for (int xx = x - 1; xx <= x; ++xx)
			for (int yy = y - 1; yy <= y; ++yy)
				for (int zz = z - 1; zz <= z; ++zz) {
					if (chunk.getBlock(xx, yy, zz) != 0 &&
						chunk.getBlock(xx, yy, zz).getData().id != BlockId::Bedrock)
						if (rand.intInRange(1, 40) <= 31)
							chunk.setBlock(xx, yy, zz, { BlockId::GoldOre, 0x00 });
				}
	}

	void setDiamondOreBlocks(Chunk &chunk, Random<std::minstd_rand> &rand, int x, int y, int z)
	{
		for (int xx = x - 1; xx <= x; ++xx)
			for (int yy = y - 1; yy <= y; ++yy)
				for (int zz = z - 1; zz <= z; ++zz) {
					if (chunk.getBlock(xx, yy, zz) != 0 &&
						chunk.getBlock(xx, yy, zz).getData().id != BlockId::Bedrock)
						if (rand.intInRange(1, 24) <= 11)
							chunk.setBlock(xx, yy, zz, { BlockId::DiamondOre, 0x00 });
				}
	}

	void setEmeraldOreBlocks(Chunk &chunk, Random<std::minstd_rand> &rand, int x, int y, int z)
	{
		if (chunk.getBlock(x, y, z) != 0 &&
			chunk.getBlock(x, y, z).getData().id != BlockId::Bedrock)
			chunk.setBlock(x, y, z, { BlockId::EmeraldOre, 0x00 });
	}

}

void ClusterGenerator::makeFlowerCluster(Chunk &chunk, Random<std::minstd_rand> &rand, int x, int z,
	Array2D<int, CHUNK_SIZE> &heightMap, BlockId blockId, int radius, int flowerFreq)
{
	for (int i = x - radius; i <= x + radius - 1; ++i)
		for (int j = z - radius; j <= z + radius - 1; ++j) {
			int yy = heightMap.get(i, j) + 1;
			if (yy > WATER_LEVEL) {
				if (chunk.getBlock(i, yy, j) == 0) {
					if (rand.intInRange(1, 100) <= flowerFreq) {
						auto blockBeneath = chunk.getBlock(i, yy - 1, j);
						if (BlockDatabase::canPlaceOnBlock(blockId, blockBeneath.getData().id))
							chunk.setBlock(i, yy, j, blockId);
					}
				}
			}
		}
}

void ClusterGenerator::makeDoubleFlowerCluster(Chunk & chunk, Random<std::minstd_rand>& rand, int x, int z,
	Array2D<int, CHUNK_SIZE>& heightMap, BlockId blockId, int radius, int flowerFreq)
{
	for (int i = x - radius; i <= x + radius - 1; ++i)
		for (int j = z - radius; j <= z + radius - 1; ++j) {
			int yy = heightMap.get(i, j) + 1;
			if (yy > WATER_LEVEL) {
				if (chunk.getBlock(i, yy, j) == 0) {
					if (rand.intInRange(1, 100) <= flowerFreq) {
						auto blockBeneath = chunk.getBlock(i, yy - 1, j);
						if (BlockDatabase::canPlaceOnBlock(blockId, blockBeneath.getData().id))
							makeDoublePlant(chunk, i, yy, j, blockId);
					}
				}
			}
		}
}

void ClusterGenerator::makeFlowerWitchCircle(Chunk & chunk, Random<std::minstd_rand>& rand, int x, int z,
	Array2D<int, CHUNK_SIZE>& heightMap, BlockId blockId)
{
	for (int xx = x - 5; xx <= x + 4; ++xx)
		for (int zz = z - 5; zz <= z + 4; ++zz) {

			int y = heightMap.get(xx + COORDS::WITCH_CIRCLE[0].x, COORDS::WITCH_CIRCLE[0].y);

			bool next = false;
			for (auto & var : COORDS::WITCH_CIRCLE) {
				auto block = chunk.getBlock(xx + var.x, y + 1, zz + var.y);
				auto blockBeneath = chunk.getBlock(xx + var.x, y, zz + var.y);
				if (heightMap.get(xx + var.x, zz + var.y) != y || block != 0 ||
					!BlockDatabase::canPlaceOnBlock(blockId, blockBeneath.getData().id)) {
					next = true;
					break;
				}
			}
			if (next)
				continue;

			for (auto & var : COORDS::WITCH_CIRCLE) {
				chunk.setBlock(xx + var.x, y + 1, zz + var.y, blockId);
			}

			return;
		}
}

void ClusterGenerator::makeUnderwaterCluster(Chunk & chunk, Random<std::minstd_rand>& rand, int x, int z, Array2D<int, CHUNK_SIZE>& heightMap, BlockId blockId)
{
	int distFromEdge = distanceFromChunkEdge(x, z);

	const sf::Vector2i *coordsArray;
	int size;

	switch (distFromEdge)
	{
	case 2:
		coordsArray = &(COORDS::CLUSTER_2R_1[0]);
		size = COORDS::CLUSTER_2R_1.size();
		break;
	case 3:
		coordsArray = &(COORDS::CLUSTER_3R_1[0]);
		size = COORDS::CLUSTER_3R_1.size();
		break;
	case 4:
		coordsArray = &(COORDS::CLUSTER_4R_1[0]);
		size = COORDS::CLUSTER_4R_1.size();
		break;
	case 6://
	case 7://
	case 5:
		coordsArray = &(COORDS::CLUSTER_5R_1[0]);
		size = COORDS::CLUSTER_5R_1.size();
		break;
	default:
		return;
	}

	for (int i = 0; i < size; ++i) {
		int xx = x + coordsArray[i].x;
		int zz = z + coordsArray[i].y;

		chunk.setBlock(xx, heightMap.get(xx, zz) - 1, zz, blockId);
		chunk.setBlock(xx, heightMap.get(xx, zz), zz, blockId);
	}
}

void ClusterGenerator::makeOreClusters(Chunk &chunk, Random<std::minstd_rand> &rand, Array2D<int, CHUNK_SIZE> &heightMap)
{
	for (int ore = static_cast<ORE_IDICES>(0); ore != ORE_IDICES::NUMBER_OF_ORES; ++ore) {
		std::vector<sf::Vector3i> spawnPositions;
		for (int i = 0; i < ORE_SPAWN_TIMES[ore]; ++i) {
			int posX = rand.intInRange(0, CHUNK_SIZE - 1);
			int posZ = rand.intInRange(0, CHUNK_SIZE - 1);
			int posY = rand.intInRange(ORE_SPAWN_LAYERS[ore][0],
				std::min(ORE_SPAWN_LAYERS[ore][1], heightMap.get(posX, posZ) - 3));

			sf::Vector3i pos{ posX, posY, posZ };
			spawnPositions.emplace_back(pos);
		}
		// refactor this later
		switch (static_cast<ORE_IDICES>(ore))
		{
		case Coal:
			for (auto & orePos : spawnPositions)
				CreateOreVein::setCoalOreBlocks(chunk, rand, orePos.x, orePos.y, orePos.z);
			break;
		case Iron:
			for (auto & orePos : spawnPositions)
				CreateOreVein::setIronOreBlocks(chunk, rand, orePos.x, orePos.y, orePos.z);
			break;
		case Redstone:
			for (auto & orePos : spawnPositions)
				CreateOreVein::setRedstoneOreBlocks(chunk, rand, orePos.x, orePos.y, orePos.z);
			break;
		case Gold:
			for (auto & orePos : spawnPositions)
				CreateOreVein::setGoldOreBlocks(chunk, rand, orePos.x, orePos.y, orePos.z);
			break;
		case Diamond:
			for (auto & orePos : spawnPositions)
				CreateOreVein::setDiamondOreBlocks(chunk, rand, orePos.x, orePos.y, orePos.z);
			break;
		case Emerald:
			for (auto & orePos : spawnPositions)
				CreateOreVein::setEmeraldOreBlocks(chunk, rand, orePos.x, orePos.y, orePos.z);
			break;
		default:
			break;
		}
	}
}
