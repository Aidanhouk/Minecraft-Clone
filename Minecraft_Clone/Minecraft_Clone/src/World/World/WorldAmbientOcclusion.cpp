#include "World.h"

#include <iostream>

#include "../AO_Constants.h"

//#define SLOW_AO_CALCULATION = 0;

namespace
{
	int vertexAO(bool side1, bool side2, bool corner);
} // namespace

void World::calculateAmbientOcclusion(Chunk * chunk)
{
	if (!chunk->hasLoaded())
		return;

	for (int z = 0; z < CHUNK_SIZE; ++z)
		for (int x = 0; x < CHUNK_SIZE; ++x) {

			auto y = chunk->getHeightAt(x, z);
			while (y >= 0) {
				if (chunk->getBlock(x, y, z).isCollidable()) {
					ChunkBlock& block = chunk->getBlockInChunkRef(x, y, z);

					calculateAO_Top(x, y + 1, z, chunk, &block);
					calculateAO_Bottom(x, y - 1, z, chunk, &block);
					calculateAO_Left(x - 1, y, z, chunk, &block);
					calculateAO_Right(x + 1, y, z, chunk, &block);
					calculateAO_Back(x, y, z - 1, chunk, &block);
					calculateAO_Front(x, y, z + 1, chunk, &block);
				}
				--y;
			}
		}

	chunk->setAmbientOcclusionLoaded(true);
}

// Optimized version
void World::updateAmbientOcclusion(int x, int y, int z, Chunk * chunk)
{
    chunk->addBusyLevel();
	auto chunkLoc = chunk->getLocation();
	chunkLoc.x = chunkLoc.x * CHUNK_SIZE + x;
	chunkLoc.y = chunkLoc.y * CHUNK_SIZE + z;

#ifdef SLOW_AO_CALCULATION
	for (int8_t zz = -1; zz <= 1; ++zz)
		for (int8_t xx = -1; xx <= 1; ++xx)
			for (int8_t yy = -1; yy <= 1; ++yy) {
				auto pair = getBlockRef(chunkLoc.x + xx, y + yy, chunkLoc.y + zz);
				if (pair.second->isCollidable()) {
					calculateAO_Top(x + xx, y + yy + 1, z + zz, chunk, pair.second);
					calculateAO_Bottom(x + xx, y + yy - 1, z + zz, chunk, pair.second);
					calculateAO_Left(x + xx - 1, y + yy, z + zz, chunk, pair.second);
					calculateAO_Right(x + xx + 1, y + yy, z + zz, chunk, pair.second);
					calculateAO_Back(x + xx, y + yy, z + zz - 1, chunk, pair.second);
					calculateAO_Front(x + xx, y + yy, z + zz + 1, chunk, pair.second);
				}
			}
    chunk->subtractBusyLevel();
#else
	ChunkBlock &block = chunk->getBlockInChunkRef(x, y, z);
	if (block.isCollidable()) {
		calculateAO_Top(x, y + 1, z, chunk, &block);
		calculateAO_Bottom(x, y - 1, z, chunk, &block);
		calculateAO_Left(x - 1, y, z, chunk, &block);
		calculateAO_Right(x + 1, y, z, chunk, &block);
		calculateAO_Back(x, y, z - 1, chunk, &block);
		calculateAO_Front(x, y, z + 1, chunk, &block);
	}
	chunk->subtractBusyLevel();

	auto pair = getBlockRef(chunkLoc.x, y + 1, chunkLoc.y);
	pair.first->addBusyLevel();
	if (pair.second->isCollidable())
		calculateAO_Bottom(x, y, z, chunk, pair.second);
	pair.first->subtractBusyLevel();

	pair = getBlockRef(chunkLoc.x, y - 1, chunkLoc.y);
	pair.first->addBusyLevel();
	if (pair.second->isCollidable())
		calculateAO_Top(x, y, z, chunk, pair.second);
	pair.first->subtractBusyLevel();

	pair = getBlockRef(chunkLoc.x - 1, y, chunkLoc.y);
	pair.first->addBusyLevel();
	if (pair.second->isCollidable())
		calculateAO_Right(x, y, z, chunk, pair.second);
	pair.first->subtractBusyLevel();

	pair = getBlockRef(chunkLoc.x + 1, y, chunkLoc.y);
	pair.first->addBusyLevel();
	if (pair.second->isCollidable())
		calculateAO_Left(x, y, z, chunk, pair.second);
	pair.first->subtractBusyLevel();

	pair = getBlockRef(chunkLoc.x, y, chunkLoc.y - 1);
	pair.first->addBusyLevel();
	if (pair.second->isCollidable())
		calculateAO_Front(x, y, z, chunk, pair.second);
	pair.first->subtractBusyLevel();

	pair = getBlockRef(chunkLoc.x, y, chunkLoc.y + 1);
	pair.first->addBusyLevel();
	if (pair.second->isCollidable())
		calculateAO_Back(x, y, z, chunk, pair.second);
	pair.first->subtractBusyLevel();



	pair = getBlockRef(chunkLoc.x - 1, y + 1, chunkLoc.y);
	pair.first->addBusyLevel();
	if (pair.second->isCollidable()) {
		calculateAO_Bottom(x - 1, y, z, chunk, pair.second);
		calculateAO_Right(x, y + 1, z, chunk, pair.second);
	}
	pair.first->subtractBusyLevel();

	pair = getBlockRef(chunkLoc.x + 1, y + 1, chunkLoc.y);
	pair.first->addBusyLevel();
	if (pair.second->isCollidable()) {
		calculateAO_Bottom(x + 1, y, z, chunk, pair.second);
		calculateAO_Left(x, y + 1, z, chunk, pair.second);
	}
	pair.first->subtractBusyLevel();

	pair = getBlockRef(chunkLoc.x + 1, y - 1, chunkLoc.y);
	pair.first->addBusyLevel();
	if (pair.second->isCollidable()) {
		calculateAO_Top(x + 1, y, z, chunk, pair.second);
		calculateAO_Left(x, y - 1, z, chunk, pair.second);
	}
	pair.first->subtractBusyLevel();

	pair = getBlockRef(chunkLoc.x - 1, y - 1, chunkLoc.y);
	pair.first->addBusyLevel();
	if (pair.second->isCollidable()) {
		calculateAO_Top(x - 1, y, z, chunk, pair.second);
		calculateAO_Right(x, y - 1, z, chunk, pair.second);
	}
	pair.first->subtractBusyLevel();


	pair = getBlockRef(chunkLoc.x, y + 1, chunkLoc.y - 1);
	pair.first->addBusyLevel();
	if (pair.second->isCollidable()) {
		calculateAO_Bottom(x, y, z - 1, chunk, pair.second);
		calculateAO_Front(x, y + 1, z, chunk, pair.second);
	}
	pair.first->subtractBusyLevel();

	pair = getBlockRef(chunkLoc.x, y + 1, chunkLoc.y + 1);
	pair.first->addBusyLevel();
	if (pair.second->isCollidable()) {
		calculateAO_Bottom(x, y, z + 1, chunk, pair.second);
		calculateAO_Back(x, y + 1, z, chunk, pair.second);
	}
	pair.first->subtractBusyLevel();

	pair = getBlockRef(chunkLoc.x, y - 1, chunkLoc.y + 1);
	pair.first->addBusyLevel();
	if (pair.second->isCollidable()) {
		calculateAO_Top(x, y, z + 1, chunk, pair.second);
		calculateAO_Back(x, y - 1, z, chunk, pair.second);
	}
	pair.first->subtractBusyLevel();

	pair = getBlockRef(chunkLoc.x, y - 1, chunkLoc.y - 1);
	pair.first->addBusyLevel();
	if (pair.second->isCollidable()) {
		calculateAO_Top(x, y, z - 1, chunk, pair.second);
		calculateAO_Front(x, y - 1, z, chunk, pair.second);
	}
	pair.first->subtractBusyLevel();
	

	pair = getBlockRef(chunkLoc.x - 1, y, chunkLoc.y - 1);
	pair.first->addBusyLevel();
	if (pair.second->isCollidable()) {
		calculateAO_Right(x, y, z - 1, chunk, pair.second);
		calculateAO_Front(x - 1, y, z, chunk, pair.second);
	}
	pair.first->subtractBusyLevel();

	pair = getBlockRef(chunkLoc.x + 1, y, chunkLoc.y - 1);
	pair.first->addBusyLevel();
	if (pair.second->isCollidable()) {
		calculateAO_Left(x, y, z - 1, chunk, pair.second);
		calculateAO_Front(x + 1, y, z, chunk, pair.second);
	}
	pair.first->subtractBusyLevel();

	pair = getBlockRef(chunkLoc.x + 1, y, chunkLoc.y + 1);
	pair.first->addBusyLevel();
	if (pair.second->isCollidable()) {
		calculateAO_Left(x, y, z + 1, chunk, pair.second);
		calculateAO_Back(x + 1, y, z, chunk, pair.second);
	}
	pair.first->subtractBusyLevel();

	pair = getBlockRef(chunkLoc.x - 1, y, chunkLoc.y + 1);
	pair.first->addBusyLevel();
	if (pair.second->isCollidable()) {
		calculateAO_Right(x, y, z + 1, chunk, pair.second);
		calculateAO_Back(x - 1, y, z, chunk, pair.second);
	}
	pair.first->subtractBusyLevel();



	pair = getBlockRef(chunkLoc.x - 1, y + 1, chunkLoc.y - 1);
	pair.first->addBusyLevel();
	if (pair.second->isCollidable()) {
		calculateAO_Bottom(x - 1, y, z - 1, chunk, pair.second);
		calculateAO_Right(x, y + 1, z - 1, chunk, pair.second);
		calculateAO_Front(x - 1, y + 1, z, chunk, pair.second);
	}
	pair.first->subtractBusyLevel();

	pair = getBlockRef(chunkLoc.x + 1, y + 1, chunkLoc.y - 1);
	pair.first->addBusyLevel();
	if (pair.second->isCollidable()) {
		calculateAO_Bottom(x + 1, y, z - 1, chunk, pair.second);
		calculateAO_Left(x, y + 1, z - 1, chunk, pair.second);
		calculateAO_Front(x + 1, y + 1, z, chunk, pair.second);
	}
	pair.first->subtractBusyLevel();

	pair = getBlockRef(chunkLoc.x + 1, y + 1, chunkLoc.y + 1);
	pair.first->addBusyLevel();
	if (pair.second->isCollidable()) {
		calculateAO_Bottom(x + 1, y, z + 1, chunk, pair.second);
		calculateAO_Left(x, y + 1, z + 1, chunk, pair.second);
		calculateAO_Back(x + 1, y + 1, z, chunk, pair.second);
	}
	pair.first->subtractBusyLevel();

	pair = getBlockRef(chunkLoc.x - 1, y + 1, chunkLoc.y + 1);
	pair.first->addBusyLevel();
	if (pair.second->isCollidable()) {
		calculateAO_Bottom(x - 1, y, z + 1, chunk, pair.second);
		calculateAO_Right(x, y + 1, z + 1, chunk, pair.second);
		calculateAO_Back(x - 1, y + 1, z, chunk, pair.second);
	}
	pair.first->subtractBusyLevel();
	

	pair = getBlockRef(chunkLoc.x - 1, y - 1, chunkLoc.y - 1);
	pair.first->addBusyLevel();
	if (pair.second->isCollidable()) {
		calculateAO_Top(x - 1, y, z - 1, chunk, pair.second);
		calculateAO_Right(x, y - 1, z - 1, chunk, pair.second);
		calculateAO_Front(x - 1, y - 1, z, chunk, pair.second);
	}
	pair.first->subtractBusyLevel();

	pair = getBlockRef(chunkLoc.x + 1, y - 1, chunkLoc.y - 1);
	pair.first->addBusyLevel();
	if (pair.second->isCollidable()) {
		calculateAO_Top(x + 1, y, z - 1, chunk, pair.second);
		calculateAO_Left(x, y - 1, z - 1, chunk, pair.second);
		calculateAO_Front(x + 1, y - 1, z, chunk, pair.second);
	}
	pair.first->subtractBusyLevel();

	pair = getBlockRef(chunkLoc.x + 1, y - 1, chunkLoc.y + 1);
	pair.first->addBusyLevel();
	if (pair.second->isCollidable()) {
		calculateAO_Top(x + 1, y, z + 1, chunk, pair.second);
		calculateAO_Left(x, y - 1, z + 1, chunk, pair.second);
		calculateAO_Back(x + 1, y - 1, z, chunk, pair.second);
	}
	pair.first->subtractBusyLevel();

	pair = getBlockRef(chunkLoc.x - 1, y - 1, chunkLoc.y + 1);
	pair.first->addBusyLevel();
	if (pair.second->isCollidable()) {
		calculateAO_Top(x - 1, y, z + 1, chunk, pair.second);
		calculateAO_Right(x, y - 1, z + 1, chunk, pair.second);
		calculateAO_Back(x - 1, y - 1, z, chunk, pair.second);
	}
	pair.first->subtractBusyLevel();
#endif
}

void World::calculateAO_Top(int x, int y, int z, Chunk * chunk, ChunkBlock *block)
{
	if (!chunk->getBlock(x, y, z).isCollidable()) {
		std::array<bool, 8> adjBlocks;

		for (size_t i = 0; i < adjBlocks.size(); ++i) {
			auto xx = x + ADJ_BLOCKS_DATA_TOP[i][0];
			auto yy = y + ADJ_BLOCKS_DATA_TOP[i][1];
			auto zz = z + ADJ_BLOCKS_DATA_TOP[i][2];
			adjBlocks[i] = chunk->getBlock(xx, yy, zz).isCollidable();
		}

		block->setAO_Top_LeftBack(vertexAO(adjBlocks[5], adjBlocks[7], adjBlocks[6]));
		block->setAO_Top_RightBack(vertexAO(adjBlocks[3], adjBlocks[5], adjBlocks[4]));
		block->setAO_Top_RightFront(vertexAO(adjBlocks[1], adjBlocks[3], adjBlocks[2]));
		block->setAO_Top_LeftFront(vertexAO(adjBlocks[7], adjBlocks[1], adjBlocks[0]));
	}
	// in case of leaves
	else if (chunk->getBlock(x, y, z).isShaderFlora()) {
		block->setAO_Top_LeftBack(AO_FULL);
		block->setAO_Top_RightBack(AO_FULL);
		block->setAO_Top_RightFront(AO_FULL);
		block->setAO_Top_LeftFront(AO_FULL);
	}
	// occlude sand below cactus
	else if (chunk->getBlock(x, y, z).isMeshCactus()) {
		block->setAO_Top_LeftBack(AO_X2);
		block->setAO_Top_RightBack(AO_X2);
		block->setAO_Top_RightFront(AO_X2);
		block->setAO_Top_LeftFront(AO_X2);
	}
}

void World::calculateAO_Bottom(int x, int y, int z, Chunk * chunk, ChunkBlock *block)
{
	if (!chunk->getBlock(x, y, z).isCollidable()) {
		std::array<bool, 8> adjBlocks;

		for (size_t i = 0; i < adjBlocks.size(); ++i) {
			auto xx = x + ADJ_BLOCKS_DATA_BOTTOM[i][0];
			auto yy = y + ADJ_BLOCKS_DATA_BOTTOM[i][1];
			auto zz = z + ADJ_BLOCKS_DATA_BOTTOM[i][2];
			adjBlocks[i] = chunk->getBlock(xx, yy, zz).isCollidable();
		}

		block->setAO_Bottom_LeftBack(vertexAO(adjBlocks[7], adjBlocks[1], adjBlocks[0]));
		block->setAO_Bottom_RightBack(vertexAO(adjBlocks[1], adjBlocks[3], adjBlocks[2]));
		block->setAO_Bottom_RightFront(vertexAO(adjBlocks[3], adjBlocks[5], adjBlocks[4]));
		block->setAO_Bottom_LeftFront(vertexAO(adjBlocks[5], adjBlocks[7], adjBlocks[6]));
	}
	else if (chunk->getBlock(x, y, z).isShaderFlora()) {
		block->setAO_Bottom_LeftBack(AO_FULL);
		block->setAO_Bottom_RightBack(AO_FULL);
		block->setAO_Bottom_RightFront(AO_FULL);
		block->setAO_Bottom_LeftFront(AO_FULL);
	}
}

void World::calculateAO_Left(int x, int y, int z, Chunk * chunk, ChunkBlock *block)
{
	if (!chunk->getBlock(x, y, z).isCollidable()) {
		std::array<bool, 8> adjBlocks;

		for (size_t i = 0; i < adjBlocks.size(); ++i) {
			auto xx = x + ADJ_BLOCKS_DATA_LEFT[i][0];
			auto yy = y + ADJ_BLOCKS_DATA_LEFT[i][1];
			auto zz = z + ADJ_BLOCKS_DATA_LEFT[i][2];
			adjBlocks[i] = chunk->getBlock(xx, yy, zz).isCollidable();
		}

		block->setAO_Left_TopBack(vertexAO(adjBlocks[7], adjBlocks[1], adjBlocks[0]));
		block->setAO_Left_TopFront(vertexAO(adjBlocks[1], adjBlocks[3], adjBlocks[2]));
		block->setAO_Left_BottomFront(vertexAO(adjBlocks[3], adjBlocks[5], adjBlocks[4]));
		block->setAO_Left_BottomBack(vertexAO(adjBlocks[5], adjBlocks[7], adjBlocks[6]));
	}
	else if (chunk->getBlock(x, y, z).isShaderFlora()) {
		block->setAO_Left_TopBack(AO_FULL);
		block->setAO_Left_TopFront(AO_FULL);
		block->setAO_Left_BottomFront(AO_FULL);
		block->setAO_Left_BottomBack(AO_FULL);
	}
}

void World::calculateAO_Right(int x, int y, int z, Chunk * chunk, ChunkBlock *block)
{
	if (!chunk->getBlock(x, y, z).isCollidable()) {
		std::array<bool, 8> adjBlocks;

		for (size_t i = 0; i < adjBlocks.size(); ++i) {
			auto xx = x + ADJ_BLOCKS_DATA_RIGHT[i][0];
			auto yy = y + ADJ_BLOCKS_DATA_RIGHT[i][1];
			auto zz = z + ADJ_BLOCKS_DATA_RIGHT[i][2];
			adjBlocks[i] = chunk->getBlock(xx, yy, zz).isCollidable();
		}

		block->setAO_Right_TopBack(vertexAO(adjBlocks[7], adjBlocks[1], adjBlocks[0]));
		block->setAO_Right_TopFront(vertexAO(adjBlocks[1], adjBlocks[3], adjBlocks[2]));
		block->setAO_Right_BottomFront(vertexAO(adjBlocks[3], adjBlocks[5], adjBlocks[4]));
		block->setAO_Right_BottomBack(vertexAO(adjBlocks[5], adjBlocks[7], adjBlocks[6]));
	}
	else if (chunk->getBlock(x, y, z).isShaderFlora()) {
		block->setAO_Right_TopBack(AO_FULL);
		block->setAO_Right_TopFront(AO_FULL);
		block->setAO_Right_BottomFront(AO_FULL);
		block->setAO_Right_BottomBack(AO_FULL);
	}
}

void World::calculateAO_Back(int x, int y, int z, Chunk * chunk, ChunkBlock *block)
{
	if (!chunk->getBlock(x, y, z).isCollidable()) {
		std::array<bool, 8> adjBlocks;

		for (size_t i = 0; i < adjBlocks.size(); ++i) {
			auto xx = x + ADJ_BLOCKS_DATA_BACK[i][0];
			auto yy = y + ADJ_BLOCKS_DATA_BACK[i][1];
			auto zz = z + ADJ_BLOCKS_DATA_BACK[i][2];
			adjBlocks[i] = chunk->getBlock(xx, yy, zz).isCollidable();
		}

		block->setAO_Back_TopRight(vertexAO(adjBlocks[7], adjBlocks[1], adjBlocks[0]));
		block->setAO_Back_TopLeft(vertexAO(adjBlocks[1], adjBlocks[3], adjBlocks[2]));
		block->setAO_Back_BottomLeft(vertexAO(adjBlocks[3], adjBlocks[5], adjBlocks[4]));
		block->setAO_Back_BottomRight(vertexAO(adjBlocks[5], adjBlocks[7], adjBlocks[6]));
	}
	else if (chunk->getBlock(x, y, z).isShaderFlora()) {
		block->setAO_Back_TopRight(AO_FULL);
		block->setAO_Back_TopLeft(AO_FULL);
		block->setAO_Back_BottomLeft(AO_FULL);
		block->setAO_Back_BottomRight(AO_FULL);
	}
}

void World::calculateAO_Front(int x, int y, int z, Chunk * chunk, ChunkBlock *block)
{
	if (!chunk->getBlock(x, y, z).isCollidable()) {
		std::array<bool, 8> adjBlocks;

		for (size_t i = 0; i < adjBlocks.size(); ++i) {
			auto xx = x + ADJ_BLOCKS_DATA_FRONT[i][0];
			auto yy = y + ADJ_BLOCKS_DATA_FRONT[i][1];
			auto zz = z + ADJ_BLOCKS_DATA_FRONT[i][2];
			adjBlocks[i] = chunk->getBlock(xx, yy, zz).isCollidable();
		}

		block->setAO_Front_TopLeft(vertexAO(adjBlocks[7], adjBlocks[1], adjBlocks[0]));
		block->setAO_Front_TopRight(vertexAO(adjBlocks[1], adjBlocks[3], adjBlocks[2]));
		block->setAO_Front_BottomRight(vertexAO(adjBlocks[3], adjBlocks[5], adjBlocks[4]));
		block->setAO_Front_BottomLeft(vertexAO(adjBlocks[5], adjBlocks[7], adjBlocks[6]));
	}
	else if (chunk->getBlock(x, y, z).isShaderFlora()) {
		block->setAO_Front_TopLeft(AO_FULL);
		block->setAO_Front_TopRight(AO_FULL);
		block->setAO_Front_BottomRight(AO_FULL);
		block->setAO_Front_BottomLeft(AO_FULL);
	}
}

namespace {

    int vertexAO(bool side1, bool side2, bool corner)
    {
        if (side1 && side2) {
            return 1;
        }
        return 3 - (side1 + side2 + corner);
    }
}