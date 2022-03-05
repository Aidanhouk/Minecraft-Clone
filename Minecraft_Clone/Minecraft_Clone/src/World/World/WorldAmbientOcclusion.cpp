#include "World.h"

#include <iostream>

void World::calculateAmbientOcclusion(Chunk * chunk)
{
	if (!chunk->hasLoaded())
		return;

	for (uint8_t z = 0; z < CHUNK_SIZE; ++z)
		for (uint8_t x = 0; x < CHUNK_SIZE; ++x) {

			auto y = chunk->getHeightAt(x, z);
			while (y >= 0) {
				ChunkBlock &block = chunk->getBlockInChunkRef(x, y, z);
				if (chunk->getBlock(x, y, z).getData().isCollidable) {

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

void World::updateAmbientOcclusion(int x, int y, int z, Chunk * chunk)
{
	auto chunkLoc = chunk->getLocation();
	chunkLoc.x = chunkLoc.x * CHUNK_SIZE + x;
	chunkLoc.y = chunkLoc.y * CHUNK_SIZE + z;

	//for (int8_t zz = -1; zz <= 1; ++zz)
	//	for (int8_t xx = -1; xx <= 1; ++xx)
	//		for (int8_t yy = -1; yy <= 1; ++yy) {
	//			auto pair = getBlockRef(chunkLoc.x + xx, y + yy, chunkLoc.y + zz);
	//			if (pair.second->getData().isCollidable) {
	//				calculateAO_Top(x + xx, y + yy + 1, z + zz, chunk, pair.second);
	//				calculateAO_Bottom(x + xx, y + yy - 1, z + zz, chunk, pair.second);
	//				calculateAO_Left(x + xx - 1, y + yy, z + zz, chunk, pair.second);
	//				calculateAO_Right(x + xx + 1, y + yy, z + zz, chunk, pair.second);
	//				calculateAO_Back(x + xx, y + yy, z + zz - 1, chunk, pair.second);
	//				calculateAO_Front(x + xx, y + yy, z + zz + 1, chunk, pair.second);
	//			}
	//		}

	ChunkBlock &block = chunk->getBlockInChunkRef(x, y, z);
	if (block.getData().isCollidable) {
		calculateAO_Top(x, y + 1, z, chunk, &block);
		calculateAO_Bottom(x, y - 1, z, chunk, &block);
		calculateAO_Left(x - 1, y, z, chunk, &block);
		calculateAO_Right(x + 1, y, z, chunk, &block);
		calculateAO_Back(x, y, z - 1, chunk, &block);
		calculateAO_Front(x, y, z + 1, chunk, &block);
	}

	auto pair = getBlockRef(chunkLoc.x, y + 1, chunkLoc.y);
	pair.first->addBusyLevel();
	if (pair.second->getData().isCollidable)
		calculateAO_Bottom(x, y, z, chunk, pair.second);
	pair.first->subtractBusyLevel();
	pair = getBlockRef(chunkLoc.x, y - 1, chunkLoc.y);
	pair.first->addBusyLevel();
	if (pair.second->getData().isCollidable)
		calculateAO_Top(x, y, z, chunk, pair.second);
	pair.first->subtractBusyLevel();
	pair = getBlockRef(chunkLoc.x - 1, y, chunkLoc.y);
	pair.first->addBusyLevel();
	if (pair.second->getData().isCollidable)
		calculateAO_Right(x, y, z, chunk, pair.second);
	pair.first->subtractBusyLevel();
	pair = getBlockRef(chunkLoc.x + 1, y, chunkLoc.y);
	pair.first->addBusyLevel();
	if (pair.second->getData().isCollidable)
		calculateAO_Left(x, y, z, chunk, pair.second);
	pair.first->subtractBusyLevel();
	pair = getBlockRef(chunkLoc.x, y, chunkLoc.y - 1);
	pair.first->addBusyLevel();
	if (pair.second->getData().isCollidable)
		calculateAO_Front(x, y, z, chunk, pair.second);
	pair.first->subtractBusyLevel();
	pair = getBlockRef(chunkLoc.x, y, chunkLoc.y + 1);
	pair.first->addBusyLevel();
	if (pair.second->getData().isCollidable)
		calculateAO_Back(x, y, z, chunk, pair.second);
	pair.first->subtractBusyLevel();



	pair = getBlockRef(chunkLoc.x - 1, y + 1, chunkLoc.y);
	pair.first->addBusyLevel();
	if (pair.second->getData().isCollidable) {
		calculateAO_Bottom(x - 1, y, z, chunk, pair.second);
		calculateAO_Right(x, y + 1, z, chunk, pair.second);
	}
	pair.first->subtractBusyLevel();
	pair = getBlockRef(chunkLoc.x + 1, y + 1, chunkLoc.y);
	pair.first->addBusyLevel();
	if (pair.second->getData().isCollidable) {
		calculateAO_Bottom(x + 1, y, z, chunk, pair.second);
		calculateAO_Left(x, y + 1, z, chunk, pair.second);
	}
	pair.first->subtractBusyLevel();
	pair = getBlockRef(chunkLoc.x + 1, y - 1, chunkLoc.y);
	pair.first->addBusyLevel();
	if (pair.second->getData().isCollidable) {
		calculateAO_Top(x + 1, y, z, chunk, pair.second);
		calculateAO_Left(x, y - 1, z, chunk, pair.second);
	}
	pair.first->subtractBusyLevel();
	pair = getBlockRef(chunkLoc.x - 1, y - 1, chunkLoc.y);
	pair.first->addBusyLevel();
	if (pair.second->getData().isCollidable) {
		calculateAO_Top(x - 1, y, z, chunk, pair.second);
		calculateAO_Right(x, y - 1, z, chunk, pair.second);
	}
	pair.first->subtractBusyLevel();

	pair = getBlockRef(chunkLoc.x, y + 1, chunkLoc.y - 1);
	pair.first->addBusyLevel();
	if (pair.second->getData().isCollidable) {
		calculateAO_Bottom(x, y, z - 1, chunk, pair.second);
		calculateAO_Front(x, y + 1, z, chunk, pair.second);
	}
	pair.first->subtractBusyLevel();
	pair = getBlockRef(chunkLoc.x, y + 1, chunkLoc.y + 1);
	pair.first->addBusyLevel();
	if (pair.second->getData().isCollidable) {
		calculateAO_Bottom(x, y, z + 1, chunk, pair.second);
		calculateAO_Back(x, y + 1, z, chunk, pair.second);
	}
	pair.first->subtractBusyLevel();
	pair = getBlockRef(chunkLoc.x, y - 1, chunkLoc.y + 1);
	pair.first->addBusyLevel();
	if (pair.second->getData().isCollidable) {
		calculateAO_Top(x, y, z + 1, chunk, pair.second);
		calculateAO_Back(x, y - 1, z, chunk, pair.second);
	}
	pair.first->subtractBusyLevel();
	pair = getBlockRef(chunkLoc.x, y - 1, chunkLoc.y - 1);
	pair.first->addBusyLevel();
	if (pair.second->getData().isCollidable) {
		calculateAO_Top(x, y, z - 1, chunk, pair.second);
		calculateAO_Front(x, y - 1, z, chunk, pair.second);
	}
	pair.first->subtractBusyLevel();
	
	pair = getBlockRef(chunkLoc.x - 1, y, chunkLoc.y - 1);
	pair.first->addBusyLevel();
	if (pair.second->getData().isCollidable) {
		calculateAO_Right(x, y, z - 1, chunk, pair.second);
		calculateAO_Front(x - 1, y, z, chunk, pair.second);
	}
	pair.first->subtractBusyLevel();
	pair = getBlockRef(chunkLoc.x + 1, y, chunkLoc.y - 1);
	pair.first->addBusyLevel();
	if (pair.second->getData().isCollidable) {
		calculateAO_Left(x, y, z - 1, chunk, pair.second);
		calculateAO_Front(x + 1, y, z, chunk, pair.second);
	}
	pair.first->subtractBusyLevel();
	pair = getBlockRef(chunkLoc.x + 1, y, chunkLoc.y + 1);
	pair.first->addBusyLevel();
	if (pair.second->getData().isCollidable) {
		calculateAO_Left(x, y, z + 1, chunk, pair.second);
		calculateAO_Back(x + 1, y, z, chunk, pair.second);
	}
	pair.first->subtractBusyLevel();
	pair = getBlockRef(chunkLoc.x - 1, y, chunkLoc.y + 1);
	pair.first->addBusyLevel();
	if (pair.second->getData().isCollidable) {
		calculateAO_Right(x, y, z + 1, chunk, pair.second);
		calculateAO_Back(x - 1, y, z, chunk, pair.second);
	}
	pair.first->subtractBusyLevel();



	pair = getBlockRef(chunkLoc.x - 1, y + 1, chunkLoc.y - 1);
	pair.first->addBusyLevel();
	if (pair.second->getData().isCollidable) {
		calculateAO_Bottom(x - 1, y, z - 1, chunk, pair.second);
		calculateAO_Right(x, y + 1, z - 1, chunk, pair.second);
		calculateAO_Front(x - 1, y + 1, z, chunk, pair.second);
	}
	pair.first->subtractBusyLevel();
	pair = getBlockRef(chunkLoc.x + 1, y + 1, chunkLoc.y - 1);
	pair.first->addBusyLevel();
	if (pair.second->getData().isCollidable) {
		calculateAO_Bottom(x + 1, y, z - 1, chunk, pair.second);
		calculateAO_Left(x, y + 1, z - 1, chunk, pair.second);
		calculateAO_Front(x + 1, y + 1, z, chunk, pair.second);
	}
	pair.first->subtractBusyLevel();
	pair = getBlockRef(chunkLoc.x + 1, y + 1, chunkLoc.y + 1);
	pair.first->addBusyLevel();
	if (pair.second->getData().isCollidable) {
		calculateAO_Bottom(x + 1, y, z + 1, chunk, pair.second);
		calculateAO_Left(x, y + 1, z + 1, chunk, pair.second);
		calculateAO_Back(x + 1, y + 1, z, chunk, pair.second);
	}
	pair.first->subtractBusyLevel();
	pair = getBlockRef(chunkLoc.x - 1, y + 1, chunkLoc.y + 1);
	pair.first->addBusyLevel();
	if (pair.second->getData().isCollidable) {
		calculateAO_Bottom(x - 1, y, z + 1, chunk, pair.second);
		calculateAO_Right(x, y + 1, z + 1, chunk, pair.second);
		calculateAO_Back(x - 1, y + 1, z, chunk, pair.second);
	}
	pair.first->subtractBusyLevel();
	
	pair = getBlockRef(chunkLoc.x - 1, y - 1, chunkLoc.y - 1);
	pair.first->addBusyLevel();
	if (pair.second->getData().isCollidable) {
		calculateAO_Top(x - 1, y, z - 1, chunk, pair.second);
		calculateAO_Right(x, y - 1, z - 1, chunk, pair.second);
		calculateAO_Front(x - 1, y - 1, z, chunk, pair.second);
	}
	pair.first->subtractBusyLevel();
	pair = getBlockRef(chunkLoc.x + 1, y - 1, chunkLoc.y - 1);
	pair.first->addBusyLevel();
	if (pair.second->getData().isCollidable) {
		calculateAO_Top(x + 1, y, z - 1, chunk, pair.second);
		calculateAO_Left(x, y - 1, z - 1, chunk, pair.second);
		calculateAO_Front(x + 1, y - 1, z, chunk, pair.second);
	}
	pair.first->subtractBusyLevel();
	pair = getBlockRef(chunkLoc.x + 1, y - 1, chunkLoc.y + 1);
	pair.first->addBusyLevel();
	if (pair.second->getData().isCollidable) {
		calculateAO_Top(x + 1, y, z + 1, chunk, pair.second);
		calculateAO_Left(x, y - 1, z + 1, chunk, pair.second);
		calculateAO_Back(x + 1, y - 1, z, chunk, pair.second);
	}
	pair.first->subtractBusyLevel();
	pair = getBlockRef(chunkLoc.x - 1, y - 1, chunkLoc.y + 1);
	pair.first->addBusyLevel();
	if (pair.second->getData().isCollidable) {
		calculateAO_Top(x - 1, y, z + 1, chunk, pair.second);
		calculateAO_Right(x, y - 1, z + 1, chunk, pair.second);
		calculateAO_Back(x - 1, y - 1, z, chunk, pair.second);
	}
	pair.first->subtractBusyLevel();
}

void World::calculateAO_Top(int x, int y, int z, Chunk * chunk, ChunkBlock *block)
{
	if (!chunk->getBlock(x, y, z).getData().isCollidable) {
		std::array<bool, 8> adjBlocks;

		adjBlocks[0] = chunk->getBlock(x - 1, y, z - 1).getData().isCollidable;
		adjBlocks[1] = chunk->getBlock(x, y, z - 1).getData().isCollidable;
		adjBlocks[2] = chunk->getBlock(x + 1, y, z - 1).getData().isCollidable;
		adjBlocks[3] = chunk->getBlock(x + 1, y, z).getData().isCollidable;
		adjBlocks[4] = chunk->getBlock(x + 1, y, z + 1).getData().isCollidable;
		adjBlocks[5] = chunk->getBlock(x, y, z + 1).getData().isCollidable;
		adjBlocks[6] = chunk->getBlock(x - 1, y, z + 1).getData().isCollidable;
		adjBlocks[7] = chunk->getBlock(x - 1, y, z).getData().isCollidable;

		block->setAO_Top_LeftBack(vertexAO(adjBlocks[5], adjBlocks[7], adjBlocks[6]));
		block->setAO_Top_RightBack(vertexAO(adjBlocks[3], adjBlocks[5], adjBlocks[4]));
		block->setAO_Top_RightFront(vertexAO(adjBlocks[1], adjBlocks[3], adjBlocks[2]));
		block->setAO_Top_LeftFront(vertexAO(adjBlocks[7], adjBlocks[1], adjBlocks[0]));
	}
	// in case of leaves
	else if (chunk->getBlock(x, y, z).getData().shaderType == BlockShaderType::Flora) {
		block->setAO_Top_LeftBack(1);
		block->setAO_Top_RightBack(1);
		block->setAO_Top_RightFront(1);
		block->setAO_Top_LeftFront(1);
	}
	// occlude sand below cactus
	else if (chunk->getBlock(x, y, z).getData().meshType == BlockMeshType::Cactus) {
		block->setAO_Top_LeftBack(2);
		block->setAO_Top_RightBack(2);
		block->setAO_Top_RightFront(2);
		block->setAO_Top_LeftFront(2);
	}
}

void World::calculateAO_Bottom(int x, int y, int z, Chunk * chunk, ChunkBlock *block)
{
	if (!chunk->getBlock(x, y, z).getData().isCollidable) {
		std::array<bool, 8> adjBlocks;

		adjBlocks[0] = chunk->getBlock(x - 1, y, z - 1).getData().isCollidable;
		adjBlocks[1] = chunk->getBlock(x, y, z - 1).getData().isCollidable;
		adjBlocks[2] = chunk->getBlock(x + 1, y, z - 1).getData().isCollidable;
		adjBlocks[3] = chunk->getBlock(x + 1, y, z).getData().isCollidable;
		adjBlocks[4] = chunk->getBlock(x + 1, y, z + 1).getData().isCollidable;
		adjBlocks[5] = chunk->getBlock(x, y, z + 1).getData().isCollidable;
		adjBlocks[6] = chunk->getBlock(x - 1, y, z + 1).getData().isCollidable;
		adjBlocks[7] = chunk->getBlock(x - 1, y, z).getData().isCollidable;

		block->setAO_Bottom_LeftBack(vertexAO(adjBlocks[7], adjBlocks[1], adjBlocks[0]));
		block->setAO_Bottom_RightBack(vertexAO(adjBlocks[1], adjBlocks[3], adjBlocks[2]));
		block->setAO_Bottom_RightFront(vertexAO(adjBlocks[3], adjBlocks[5], adjBlocks[4]));
		block->setAO_Bottom_LeftFront(vertexAO(adjBlocks[5], adjBlocks[7], adjBlocks[6]));
	}
	else if (chunk->getBlock(x, y, z).getData().shaderType == BlockShaderType::Flora) {
		block->setAO_Bottom_LeftBack(1);
		block->setAO_Bottom_RightBack(1);
		block->setAO_Bottom_RightFront(1);
		block->setAO_Bottom_LeftFront(1);
	}
}

void World::calculateAO_Left(int x, int y, int z, Chunk * chunk, ChunkBlock *block)
{
	if (!chunk->getBlock(x, y, z).getData().isCollidable) {
		std::array<bool, 8> adjBlocks;

		adjBlocks[0] = chunk->getBlock(x, y + 1, z - 1).getData().isCollidable;
		adjBlocks[1] = chunk->getBlock(x, y + 1, z).getData().isCollidable;
		adjBlocks[2] = chunk->getBlock(x, y + 1, z + 1).getData().isCollidable;
		adjBlocks[3] = chunk->getBlock(x, y, z + 1).getData().isCollidable;
		adjBlocks[4] = chunk->getBlock(x, y - 1, z + 1).getData().isCollidable;
		adjBlocks[5] = chunk->getBlock(x, y - 1, z).getData().isCollidable;
		adjBlocks[6] = chunk->getBlock(x, y - 1, z - 1).getData().isCollidable;
		adjBlocks[7] = chunk->getBlock(x, y, z - 1).getData().isCollidable;

		block->setAO_Left_TopBack(vertexAO(adjBlocks[7], adjBlocks[1], adjBlocks[0]));
		block->setAO_Left_TopFront(vertexAO(adjBlocks[1], adjBlocks[3], adjBlocks[2]));
		block->setAO_Left_BottomFront(vertexAO(adjBlocks[3], adjBlocks[5], adjBlocks[4]));
		block->setAO_Left_BottomBack(vertexAO(adjBlocks[5], adjBlocks[7], adjBlocks[6]));
	}
	else if (chunk->getBlock(x, y, z).getData().shaderType == BlockShaderType::Flora) {
		block->setAO_Left_TopBack(1);
		block->setAO_Left_TopFront(1);
		block->setAO_Left_BottomFront(1);
		block->setAO_Left_BottomBack(1);
	}
}

void World::calculateAO_Right(int x, int y, int z, Chunk * chunk, ChunkBlock *block)
{
	if (!chunk->getBlock(x, y, z).getData().isCollidable) {
		std::array<bool, 8> adjBlocks;

		adjBlocks[0] = chunk->getBlock(x, y + 1, z - 1).getData().isCollidable;
		adjBlocks[1] = chunk->getBlock(x, y + 1, z).getData().isCollidable;
		adjBlocks[2] = chunk->getBlock(x, y + 1, z + 1).getData().isCollidable;
		adjBlocks[3] = chunk->getBlock(x, y, z + 1).getData().isCollidable;
		adjBlocks[4] = chunk->getBlock(x, y - 1, z + 1).getData().isCollidable;
		adjBlocks[5] = chunk->getBlock(x, y - 1, z).getData().isCollidable;
		adjBlocks[6] = chunk->getBlock(x, y - 1, z - 1).getData().isCollidable;
		adjBlocks[7] = chunk->getBlock(x, y, z - 1).getData().isCollidable;

		block->setAO_Right_TopBack(vertexAO(adjBlocks[7], adjBlocks[1], adjBlocks[0]));
		block->setAO_Right_TopFront(vertexAO(adjBlocks[1], adjBlocks[3], adjBlocks[2]));
		block->setAO_Right_BottomFront(vertexAO(adjBlocks[3], adjBlocks[5], adjBlocks[4]));
		block->setAO_Right_BottomBack(vertexAO(adjBlocks[5], adjBlocks[7], adjBlocks[6]));
	}
	else if (chunk->getBlock(x, y, z).getData().shaderType == BlockShaderType::Flora) {
		block->setAO_Right_TopBack(1);
		block->setAO_Right_TopFront(1);
		block->setAO_Right_BottomFront(1);
		block->setAO_Right_BottomBack(1);
	}
}

void World::calculateAO_Back(int x, int y, int z, Chunk * chunk, ChunkBlock *block)
{
	if (!chunk->getBlock(x, y, z).getData().isCollidable) {
		std::array<bool, 8> adjBlocks;

		adjBlocks[0] = chunk->getBlock(x - 1, y + 1, z).getData().isCollidable;
		adjBlocks[1] = chunk->getBlock(x, y + 1, z).getData().isCollidable;
		adjBlocks[2] = chunk->getBlock(x + 1, y + 1, z).getData().isCollidable;
		adjBlocks[3] = chunk->getBlock(x + 1, y, z).getData().isCollidable;
		adjBlocks[4] = chunk->getBlock(x + 1, y - 1, z).getData().isCollidable;
		adjBlocks[5] = chunk->getBlock(x, y - 1, z).getData().isCollidable;
		adjBlocks[6] = chunk->getBlock(x - 1, y - 1, z).getData().isCollidable;
		adjBlocks[7] = chunk->getBlock(x - 1, y, z).getData().isCollidable;

		block->setAO_Back_TopRight(vertexAO(adjBlocks[7], adjBlocks[1], adjBlocks[0]));
		block->setAO_Back_TopLeft(vertexAO(adjBlocks[1], adjBlocks[3], adjBlocks[2]));
		block->setAO_Back_BottomLeft(vertexAO(adjBlocks[3], adjBlocks[5], adjBlocks[4]));
		block->setAO_Back_BottomRight(vertexAO(adjBlocks[5], adjBlocks[7], adjBlocks[6]));
	}
	else if (chunk->getBlock(x, y, z).getData().shaderType == BlockShaderType::Flora) {
		block->setAO_Back_TopRight(1);
		block->setAO_Back_TopLeft(1);
		block->setAO_Back_BottomLeft(1);
		block->setAO_Back_BottomRight(1);
	}
}

void World::calculateAO_Front(int x, int y, int z, Chunk * chunk, ChunkBlock *block)
{
	if (!chunk->getBlock(x, y, z).getData().isCollidable) {
		std::array<bool, 8> adjBlocks;

		adjBlocks[0] = chunk->getBlock(x - 1, y + 1, z).getData().isCollidable;
		adjBlocks[1] = chunk->getBlock(x, y + 1, z).getData().isCollidable;
		adjBlocks[2] = chunk->getBlock(x + 1, y + 1, z).getData().isCollidable;
		adjBlocks[3] = chunk->getBlock(x + 1, y, z).getData().isCollidable;
		adjBlocks[4] = chunk->getBlock(x + 1, y - 1, z).getData().isCollidable;
		adjBlocks[5] = chunk->getBlock(x, y - 1, z).getData().isCollidable;
		adjBlocks[6] = chunk->getBlock(x - 1, y - 1, z).getData().isCollidable;
		adjBlocks[7] = chunk->getBlock(x - 1, y, z).getData().isCollidable;

		block->setAO_Front_TopLeft(vertexAO(adjBlocks[7], adjBlocks[1], adjBlocks[0]));
		block->setAO_Front_TopRight(vertexAO(adjBlocks[1], adjBlocks[3], adjBlocks[2]));
		block->setAO_Front_BottomRight(vertexAO(adjBlocks[3], adjBlocks[5], adjBlocks[4]));
		block->setAO_Front_BottomLeft(vertexAO(adjBlocks[5], adjBlocks[7], adjBlocks[6]));
	}
	else if (chunk->getBlock(x, y, z).getData().shaderType == BlockShaderType::Flora) {
		block->setAO_Front_TopLeft(1);
		block->setAO_Front_TopRight(1);
		block->setAO_Front_BottomRight(1);
		block->setAO_Front_BottomLeft(1);
	}
}

int World::vertexAO(bool side1, bool side2, bool corner)
{
	if (side1 && side2) {
		return 1;
	}
	return 3 - (side1 + side2 + corner);
}