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

					calculateAO_Top(x, y + 1, z, chunk, block);
					calculateAO_Bottom(x, y - 1, z, chunk, block);
					calculateAO_Left(x - 1, y, z, chunk, block);
					calculateAO_Right(x + 1, y, z, chunk, block);
					calculateAO_Back(x, y, z - 1, chunk, block);
					calculateAO_Front(x, y, z + 1, chunk, block);
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
	//			ChunkBlock &bl = getBlockRef(chunkLoc.x + xx, y + yy, chunkLoc.y + zz);
	//			if (bl.getData().isCollidable) {
	//				calculateAO_Top(x + xx, y + yy + 1, z + zz, chunk, bl);
	//				calculateAO_Bottom(x + xx, y + yy - 1, z + zz, chunk, bl);
	//				calculateAO_Left(x + xx - 1, y + yy, z + zz, chunk, bl);
	//				calculateAO_Right(x + xx + 1, y + yy, z + zz, chunk, bl);
	//				calculateAO_Back(x + xx, y + yy, z + zz - 1, chunk, bl);
	//				calculateAO_Front(x + xx, y + yy, z + zz + 1, chunk, bl);
	//			}
	//		}

	ChunkBlock &block = chunk->getBlockInChunkRef(x, y, z);
	if (block.getData().isCollidable) {
		calculateAO_Top(x, y + 1, z, chunk, block);
		calculateAO_Bottom(x, y - 1, z, chunk, block);
		calculateAO_Left(x - 1, y, z, chunk, block);
		calculateAO_Right(x + 1, y, z, chunk, block);
		calculateAO_Back(x, y, z - 1, chunk, block);
		calculateAO_Front(x, y, z + 1, chunk, block);
	}

	ChunkBlock &blockUp = getBlockRef(chunkLoc.x, y + 1, chunkLoc.y);
	if (blockUp.getData().isCollidable)
		calculateAO_Bottom(x, y, z, chunk, blockUp);
	ChunkBlock &blockDown = getBlockRef(chunkLoc.x, y - 1, chunkLoc.y);
	if (blockDown.getData().isCollidable)
		calculateAO_Top(x, y, z, chunk, blockDown);
	ChunkBlock &blockLeft = getBlockRef(chunkLoc.x - 1, y, chunkLoc.y);
	if (blockLeft.getData().isCollidable)
		calculateAO_Right(x, y, z, chunk, blockLeft);
	ChunkBlock &blockRight = getBlockRef(chunkLoc.x + 1, y, chunkLoc.y);
	if (blockRight.getData().isCollidable)
		calculateAO_Left(x, y, z, chunk, blockRight);
	ChunkBlock &blockBack = getBlockRef(chunkLoc.x, y, chunkLoc.y - 1);
	if (blockBack.getData().isCollidable)
		calculateAO_Front(x, y, z, chunk, blockBack);
	ChunkBlock &blockFront = getBlockRef(chunkLoc.x, y, chunkLoc.y + 1);
	if (blockFront.getData().isCollidable)
		calculateAO_Back(x, y, z, chunk, blockFront);



	ChunkBlock &blockTopLeft = getBlockRef(chunkLoc.x - 1, y + 1, chunkLoc.y);
	if (blockTopLeft.getData().isCollidable) {
		calculateAO_Bottom(x - 1, y, z, chunk, blockTopLeft);
		calculateAO_Right(x, y + 1, z, chunk, blockTopLeft);
	}
	ChunkBlock &blockTopRight = getBlockRef(chunkLoc.x + 1, y + 1, chunkLoc.y);
	if (blockTopRight.getData().isCollidable) {
		calculateAO_Bottom(x + 1, y, z, chunk, blockTopRight);
		calculateAO_Left(x, y + 1, z, chunk, blockTopRight);
	}
	ChunkBlock &blockBottomRight = getBlockRef(chunkLoc.x + 1, y - 1, chunkLoc.y);
	if (blockBottomRight.getData().isCollidable) {
		calculateAO_Top(x + 1, y, z, chunk, blockBottomRight);
		calculateAO_Left(x, y - 1, z, chunk, blockBottomRight);
	}
	ChunkBlock &blockBottomLeft = getBlockRef(chunkLoc.x - 1, y - 1, chunkLoc.y);
	if (blockBottomLeft.getData().isCollidable) {
		calculateAO_Top(x - 1, y, z, chunk, blockBottomLeft);
		calculateAO_Right(x, y - 1, z, chunk, blockBottomLeft);
	}

	ChunkBlock &blockTopBack = getBlockRef(chunkLoc.x, y + 1, chunkLoc.y - 1);
	if (blockTopBack.getData().isCollidable) {
		calculateAO_Bottom(x, y, z - 1, chunk, blockTopBack);
		calculateAO_Front(x, y + 1, z, chunk, blockTopBack);
	}
	ChunkBlock &blockTopFront = getBlockRef(chunkLoc.x, y + 1, chunkLoc.y + 1);
	if (blockTopFront.getData().isCollidable) {
		calculateAO_Bottom(x, y, z + 1, chunk, blockTopFront);
		calculateAO_Back(x, y + 1, z, chunk, blockTopFront);
	}
	ChunkBlock &blockBottomFront = getBlockRef(chunkLoc.x, y - 1, chunkLoc.y + 1);
	if (blockBottomFront.getData().isCollidable) {
		calculateAO_Top(x, y, z + 1, chunk, blockBottomFront);
		calculateAO_Back(x, y - 1, z, chunk, blockBottomFront);
	}
	ChunkBlock &blockBottomBack = getBlockRef(chunkLoc.x, y - 1, chunkLoc.y - 1);
	if (blockBottomBack.getData().isCollidable) {
		calculateAO_Top(x, y, z - 1, chunk, blockBottomBack);
		calculateAO_Front(x, y - 1, z, chunk, blockBottomBack);
	}
	
	ChunkBlock &blockLeftBack = getBlockRef(chunkLoc.x - 1, y, chunkLoc.y - 1);
	if (blockLeftBack.getData().isCollidable) {
		calculateAO_Right(x, y, z - 1, chunk, blockLeftBack);
		calculateAO_Front(x - 1, y, z, chunk, blockLeftBack);
	}
	ChunkBlock &blockRightBack = getBlockRef(chunkLoc.x + 1, y, chunkLoc.y - 1);
	if (blockRightBack.getData().isCollidable) {
		calculateAO_Left(x, y, z - 1, chunk, blockRightBack);
		calculateAO_Front(x + 1, y, z, chunk, blockRightBack);
	}
	ChunkBlock &blockRightFront = getBlockRef(chunkLoc.x + 1, y, chunkLoc.y + 1);
	if (blockRightFront.getData().isCollidable) {
		calculateAO_Left(x, y, z + 1, chunk, blockRightFront);
		calculateAO_Back(x + 1, y, z, chunk, blockRightFront);
	}
	ChunkBlock &blockLeftFront = getBlockRef(chunkLoc.x - 1, y, chunkLoc.y + 1);
	if (blockLeftFront.getData().isCollidable) {
		calculateAO_Right(x, y, z + 1, chunk, blockLeftFront);
		calculateAO_Back(x - 1, y, z, chunk, blockLeftFront);
	}



	ChunkBlock &blockTopLeftBack = getBlockRef(chunkLoc.x - 1, y + 1, chunkLoc.y - 1);
	if (blockTopLeftBack.getData().isCollidable) {
		calculateAO_Bottom(x - 1, y, z - 1, chunk, blockTopLeftBack);
		calculateAO_Right(x, y + 1, z - 1, chunk, blockTopLeftBack);
		calculateAO_Front(x - 1, y + 1, z, chunk, blockTopLeftBack);
	}
	ChunkBlock &blockTopRightBack = getBlockRef(chunkLoc.x + 1, y + 1, chunkLoc.y - 1);
	if (blockTopRightBack.getData().isCollidable) {
		calculateAO_Bottom(x + 1, y, z - 1, chunk, blockTopRightBack);
		calculateAO_Left(x, y + 1, z - 1, chunk, blockTopRightBack);
		calculateAO_Front(x + 1, y + 1, z, chunk, blockTopRightBack);
	}
	ChunkBlock &blockTopRightFront = getBlockRef(chunkLoc.x + 1, y + 1, chunkLoc.y + 1);
	if (blockTopRightFront.getData().isCollidable) {
		calculateAO_Bottom(x + 1, y, z + 1, chunk, blockTopRightFront);
		calculateAO_Left(x, y + 1, z + 1, chunk, blockTopRightFront);
		calculateAO_Back(x + 1, y + 1, z, chunk, blockTopRightFront);
	}
	ChunkBlock &blockTopLeftFront = getBlockRef(chunkLoc.x - 1, y + 1, chunkLoc.y + 1);
	if (blockTopLeftFront.getData().isCollidable) {
		calculateAO_Bottom(x - 1, y, z + 1, chunk, blockTopLeftFront);
		calculateAO_Right(x, y + 1, z + 1, chunk, blockTopLeftFront);
		calculateAO_Back(x - 1, y + 1, z, chunk, blockTopLeftFront);
	}
	
	ChunkBlock &blockBottomLeftBack = getBlockRef(chunkLoc.x - 1, y - 1, chunkLoc.y - 1);
	if (blockBottomLeftBack.getData().isCollidable) {
		calculateAO_Top(x - 1, y, z - 1, chunk, blockBottomLeftBack);
		calculateAO_Right(x, y - 1, z - 1, chunk, blockBottomLeftBack);
		calculateAO_Front(x - 1, y - 1, z, chunk, blockBottomLeftBack);
	}
	ChunkBlock &blockBottomRightBack = getBlockRef(chunkLoc.x + 1, y - 1, chunkLoc.y - 1);
	if (blockBottomRightBack.getData().isCollidable) {
		calculateAO_Top(x + 1, y, z - 1, chunk, blockBottomRightBack);
		calculateAO_Left(x, y - 1, z - 1, chunk, blockBottomRightBack);
		calculateAO_Front(x + 1, y - 1, z, chunk, blockBottomRightBack);
	}
	ChunkBlock &blockBottomRightFront = getBlockRef(chunkLoc.x + 1, y - 1, chunkLoc.y + 1);
	if (blockBottomRightFront.getData().isCollidable) {
		calculateAO_Top(x + 1, y, z + 1, chunk, blockBottomRightFront);
		calculateAO_Left(x, y - 1, z + 1, chunk, blockBottomRightFront);
		calculateAO_Back(x + 1, y - 1, z, chunk, blockBottomRightFront);
	}
	ChunkBlock &blockBottomLeftFront = getBlockRef(chunkLoc.x - 1, y - 1, chunkLoc.y + 1);
	if (blockBottomLeftFront.getData().isCollidable) {
		calculateAO_Top(x - 1, y, z + 1, chunk, blockBottomLeftFront);
		calculateAO_Right(x, y - 1, z + 1, chunk, blockBottomLeftFront);
		calculateAO_Back(x - 1, y - 1, z, chunk, blockBottomLeftFront);
	}
}

void World::calculateAO_Top(int x, int y, int z, Chunk * chunk, ChunkBlock &block)
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

		block.setAO_Top_LeftBack(vertexAO(adjBlocks[5], adjBlocks[7], adjBlocks[6]));
		block.setAO_Top_RightBack(vertexAO(adjBlocks[3], adjBlocks[5], adjBlocks[4]));
		block.setAO_Top_RightFront(vertexAO(adjBlocks[1], adjBlocks[3], adjBlocks[2]));
		block.setAO_Top_LeftFront(vertexAO(adjBlocks[7], adjBlocks[1], adjBlocks[0]));
	}
	// in case of leaves
	else if (chunk->getBlock(x, y, z).getData().shaderType == BlockShaderType::Flora) {
		block.setAO_Top_LeftBack(1);
		block.setAO_Top_RightBack(1);
		block.setAO_Top_RightFront(1);
		block.setAO_Top_LeftFront(1);
	}
	// occlude sand below cactus
	else if (chunk->getBlock(x, y, z).getData().meshType == BlockMeshType::Cactus) {
		block.setAO_Top_LeftBack(2);
		block.setAO_Top_RightBack(2);
		block.setAO_Top_RightFront(2);
		block.setAO_Top_LeftFront(2);
	}
}

void World::calculateAO_Bottom(int x, int y, int z, Chunk * chunk, ChunkBlock &block)
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

		block.setAO_Bottom_LeftBack(vertexAO(adjBlocks[7], adjBlocks[1], adjBlocks[0]));
		block.setAO_Bottom_RightBack(vertexAO(adjBlocks[1], adjBlocks[3], adjBlocks[2]));
		block.setAO_Bottom_RightFront(vertexAO(adjBlocks[3], adjBlocks[5], adjBlocks[4]));
		block.setAO_Bottom_LeftFront(vertexAO(adjBlocks[5], adjBlocks[7], adjBlocks[6]));
	}
	else if (chunk->getBlock(x, y, z).getData().shaderType == BlockShaderType::Flora) {
		block.setAO_Bottom_LeftBack(1);
		block.setAO_Bottom_RightBack(1);
		block.setAO_Bottom_RightFront(1);
		block.setAO_Bottom_LeftFront(1);
	}
}

void World::calculateAO_Left(int x, int y, int z, Chunk * chunk, ChunkBlock &block)
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

		block.setAO_Left_TopBack(vertexAO(adjBlocks[7], adjBlocks[1], adjBlocks[0]));
		block.setAO_Left_TopFront(vertexAO(adjBlocks[1], adjBlocks[3], adjBlocks[2]));
		block.setAO_Left_BottomFront(vertexAO(adjBlocks[3], adjBlocks[5], adjBlocks[4]));
		block.setAO_Left_BottomBack(vertexAO(adjBlocks[5], adjBlocks[7], adjBlocks[6]));
	}
	else if (chunk->getBlock(x, y, z).getData().shaderType == BlockShaderType::Flora) {
		block.setAO_Left_TopBack(1);
		block.setAO_Left_TopFront(1);
		block.setAO_Left_BottomFront(1);
		block.setAO_Left_BottomBack(1);
	}
}

void World::calculateAO_Right(int x, int y, int z, Chunk * chunk, ChunkBlock &block)
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

		block.setAO_Right_TopBack(vertexAO(adjBlocks[7], adjBlocks[1], adjBlocks[0]));
		block.setAO_Right_TopFront(vertexAO(adjBlocks[1], adjBlocks[3], adjBlocks[2]));
		block.setAO_Right_BottomFront(vertexAO(adjBlocks[3], adjBlocks[5], adjBlocks[4]));
		block.setAO_Right_BottomBack(vertexAO(adjBlocks[5], adjBlocks[7], adjBlocks[6]));
	}
	else if (chunk->getBlock(x, y, z).getData().shaderType == BlockShaderType::Flora) {
		block.setAO_Right_TopBack(1);
		block.setAO_Right_TopFront(1);
		block.setAO_Right_BottomFront(1);
		block.setAO_Right_BottomBack(1);
	}
}

void World::calculateAO_Back(int x, int y, int z, Chunk * chunk, ChunkBlock &block)
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

		block.setAO_Back_TopRight(vertexAO(adjBlocks[7], adjBlocks[1], adjBlocks[0]));
		block.setAO_Back_TopLeft(vertexAO(adjBlocks[1], adjBlocks[3], adjBlocks[2]));
		block.setAO_Back_BottomLeft(vertexAO(adjBlocks[3], adjBlocks[5], adjBlocks[4]));
		block.setAO_Back_BottomRight(vertexAO(adjBlocks[5], adjBlocks[7], adjBlocks[6]));
	}
	else if (chunk->getBlock(x, y, z).getData().shaderType == BlockShaderType::Flora) {
		block.setAO_Back_TopRight(1);
		block.setAO_Back_TopLeft(1);
		block.setAO_Back_BottomLeft(1);
		block.setAO_Back_BottomRight(1);
	}
}

void World::calculateAO_Front(int x, int y, int z, Chunk * chunk, ChunkBlock &block)
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

		block.setAO_Front_TopLeft(vertexAO(adjBlocks[7], adjBlocks[1], adjBlocks[0]));
		block.setAO_Front_TopRight(vertexAO(adjBlocks[1], adjBlocks[3], adjBlocks[2]));
		block.setAO_Front_BottomRight(vertexAO(adjBlocks[3], adjBlocks[5], adjBlocks[4]));
		block.setAO_Front_BottomLeft(vertexAO(adjBlocks[5], adjBlocks[7], adjBlocks[6]));
	}
	else if (chunk->getBlock(x, y, z).getData().shaderType == BlockShaderType::Flora) {
		block.setAO_Front_TopLeft(1);
		block.setAO_Front_TopRight(1);
		block.setAO_Front_BottomRight(1);
		block.setAO_Front_BottomLeft(1);
	}
}

int World::vertexAO(bool side1, bool side2, bool corner)
{
	if (side1 && side2) {
		return 1;
	}
	return 3 - (side1 + side2 + corner);
}