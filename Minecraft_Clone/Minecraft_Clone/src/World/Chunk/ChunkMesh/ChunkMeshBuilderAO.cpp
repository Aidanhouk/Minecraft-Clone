#include "ChunkMeshBuilder.h"

#include "../ChunkSection.h"

void ChunkMeshBuilder::calculateAO_Top(int x, int y, int z, std::array<GLfloat, 4>& verticesAO)
{
	if (m_pChunkSection->getBlock(x, y, z).getData().isOpaque) {
		verticesAO[0] = 1.0f;
		verticesAO[1] = 1.0f;
		verticesAO[2] = 1.0f;
		verticesAO[3] = 1.0f;
		return;
	}

	std::array<bool, 8> adjBlocks;
	ChunkBlock block;

	block = m_pChunkSection->getBlock(x - 1, y, z - 1);
	adjBlocks[0] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;
	block = m_pChunkSection->getBlock(x, y, z - 1);
	adjBlocks[1] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;
	block = m_pChunkSection->getBlock(x + 1, y, z - 1);
	adjBlocks[2] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;
	block = m_pChunkSection->getBlock(x + 1, y, z);
	adjBlocks[3] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;
	block = m_pChunkSection->getBlock(x + 1, y, z + 1);
	adjBlocks[4] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;
	block = m_pChunkSection->getBlock(x, y, z + 1);
	adjBlocks[5] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;
	block = m_pChunkSection->getBlock(x - 1, y, z + 1);
	adjBlocks[6] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;
	block = m_pChunkSection->getBlock(x - 1, y, z);
	adjBlocks[7] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;

	verticesAO[0] = vertexAO(adjBlocks[5], adjBlocks[7], adjBlocks[6]);
	verticesAO[1] = vertexAO(adjBlocks[3], adjBlocks[5], adjBlocks[4]);
	verticesAO[2] = vertexAO(adjBlocks[1], adjBlocks[3], adjBlocks[2]);
	verticesAO[3] = vertexAO(adjBlocks[7], adjBlocks[1], adjBlocks[0]);

	block = m_pChunkSection->getBlock(x, y, z);
	if (block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube) {
		if (verticesAO[0] == 1.0f)
			verticesAO[0] = 0.666f;
		if (verticesAO[1] == 1.0f)
			verticesAO[1] = 0.666f;
		if (verticesAO[2] == 1.0f)
			verticesAO[2] = 0.666f;
		if (verticesAO[3] == 1.0f)
			verticesAO[3] = 0.666f;
		return;
	}
}

void ChunkMeshBuilder::calculateAO_Bottom(int x, int y, int z, std::array<GLfloat, 4>& verticesAO)
{
	if (m_pChunkSection->getBlock(x, y, z).getData().isOpaque) {
		verticesAO[0] = 1.0f;
		verticesAO[1] = 1.0f;
		verticesAO[2] = 1.0f;
		verticesAO[3] = 1.0f;
		return;
	}

	std::array<bool, 8> adjBlocks;
	ChunkBlock block;

	block = m_pChunkSection->getBlock(x - 1, y, z - 1);
	adjBlocks[0] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;
	block = m_pChunkSection->getBlock(x, y, z - 1);
	adjBlocks[1] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;
	block = m_pChunkSection->getBlock(x + 1, y, z - 1);
	adjBlocks[2] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;
	block = m_pChunkSection->getBlock(x + 1, y, z);
	adjBlocks[3] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;
	block = m_pChunkSection->getBlock(x + 1, y, z + 1);
	adjBlocks[4] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;
	block = m_pChunkSection->getBlock(x, y, z + 1);
	adjBlocks[5] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;
	block = m_pChunkSection->getBlock(x - 1, y, z + 1);
	adjBlocks[6] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;
	block = m_pChunkSection->getBlock(x - 1, y, z);
	adjBlocks[7] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;

	verticesAO[0] = vertexAO(adjBlocks[7], adjBlocks[1], adjBlocks[0]);
	verticesAO[1] = vertexAO(adjBlocks[1], adjBlocks[3], adjBlocks[2]);
	verticesAO[2] = vertexAO(adjBlocks[3], adjBlocks[5], adjBlocks[4]);
	verticesAO[3] = vertexAO(adjBlocks[5], adjBlocks[7], adjBlocks[6]);

	block = m_pChunkSection->getBlock(x, y, z);
	if (block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube) {
		if (verticesAO[0] == 1.0f)
			verticesAO[0] = 0.666f;
		if (verticesAO[1] == 1.0f)
			verticesAO[1] = 0.666f;
		if (verticesAO[2] == 1.0f)
			verticesAO[2] = 0.666f;
		if (verticesAO[3] == 1.0f)
			verticesAO[3] = 0.666f;
		return;
	}
}

void ChunkMeshBuilder::calculateAO_Left(int x, int y, int z, std::array<GLfloat, 4>& verticesAO)
{
	if (m_pChunkSection->getBlock(x, y, z).getData().isOpaque) {
		verticesAO[0] = 1.0f;
		verticesAO[1] = 1.0f;
		verticesAO[2] = 1.0f;
		verticesAO[3] = 1.0f;
		return;
	}

	std::array<bool, 8> adjBlocks;
	ChunkBlock block;

	block = m_pChunkSection->getBlock(x, y + 1, z - 1);
	adjBlocks[0] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;
	block = m_pChunkSection->getBlock(x, y + 1, z);
	adjBlocks[1] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;
	block = m_pChunkSection->getBlock(x, y + 1, z + 1);
	adjBlocks[2] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;
	block = m_pChunkSection->getBlock(x, y, z + 1);
	adjBlocks[3] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;
	block = m_pChunkSection->getBlock(x, y - 1, z + 1);
	adjBlocks[4] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;
	block = m_pChunkSection->getBlock(x, y - 1, z);
	adjBlocks[5] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;
	block = m_pChunkSection->getBlock(x, y - 1, z - 1);
	adjBlocks[6] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;
	block = m_pChunkSection->getBlock(x, y, z - 1);
	adjBlocks[7] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;

	verticesAO[0] = vertexAO(adjBlocks[5], adjBlocks[7], adjBlocks[6]);
	verticesAO[1] = vertexAO(adjBlocks[3], adjBlocks[5], adjBlocks[4]);
	verticesAO[2] = vertexAO(adjBlocks[1], adjBlocks[3], adjBlocks[2]);
	verticesAO[3] = vertexAO(adjBlocks[7], adjBlocks[1], adjBlocks[0]);

	block = m_pChunkSection->getBlock(x, y, z);
	if (block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube) {
		if (verticesAO[0] == 1.0f)
			verticesAO[0] = 0.666f;
		if (verticesAO[1] == 1.0f)
			verticesAO[1] = 0.666f;
		if (verticesAO[2] == 1.0f)
			verticesAO[2] = 0.666f;
		if (verticesAO[3] == 1.0f)
			verticesAO[3] = 0.666f;
		return;
	}
}

void ChunkMeshBuilder::calculateAO_Right(int x, int y, int z, std::array<GLfloat, 4>& verticesAO)
{
	if (m_pChunkSection->getBlock(x, y, z).getData().isOpaque) {
		verticesAO[0] = 1.0f;
		verticesAO[1] = 1.0f;
		verticesAO[2] = 1.0f;
		verticesAO[3] = 1.0f;
		return;
	}

	std::array<bool, 8> adjBlocks;
	ChunkBlock block;

	block = m_pChunkSection->getBlock(x, y + 1, z - 1);
	adjBlocks[0] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;
	block = m_pChunkSection->getBlock(x, y + 1, z);
	adjBlocks[1] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;
	block = m_pChunkSection->getBlock(x, y + 1, z + 1);
	adjBlocks[2] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;
	block = m_pChunkSection->getBlock(x, y, z + 1);
	adjBlocks[3] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;
	block = m_pChunkSection->getBlock(x, y - 1, z + 1);
	adjBlocks[4] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;
	block = m_pChunkSection->getBlock(x, y - 1, z);
	adjBlocks[5] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;
	block = m_pChunkSection->getBlock(x, y - 1, z - 1);
	adjBlocks[6] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;
	block = m_pChunkSection->getBlock(x, y, z - 1);
	adjBlocks[7] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;

	verticesAO[0] = vertexAO(adjBlocks[3], adjBlocks[5], adjBlocks[4]);
	verticesAO[1] = vertexAO(adjBlocks[5], adjBlocks[7], adjBlocks[6]);
	verticesAO[3] = vertexAO(adjBlocks[1], adjBlocks[3], adjBlocks[2]);
	verticesAO[2] = vertexAO(adjBlocks[7], adjBlocks[1], adjBlocks[0]);

	block = m_pChunkSection->getBlock(x, y, z);
	if (block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube) {
		if (verticesAO[0] == 1.0f)
			verticesAO[0] = 0.666f;
		if (verticesAO[1] == 1.0f)
			verticesAO[1] = 0.666f;
		if (verticesAO[2] == 1.0f)
			verticesAO[2] = 0.666f;
		if (verticesAO[3] == 1.0f)
			verticesAO[3] = 0.666f;
		return;
	}
}

void ChunkMeshBuilder::calculateAO_Front(int x, int y, int z, std::array<GLfloat, 4>& verticesAO)
{
	if (m_pChunkSection->getBlock(x, y, z).getData().isOpaque) {
		verticesAO[0] = 1.0f;
		verticesAO[1] = 1.0f;
		verticesAO[2] = 1.0f;
		verticesAO[3] = 1.0f;
		return;
	}

	std::array<bool, 8> adjBlocks;
	ChunkBlock block;

	block = m_pChunkSection->getBlock(x - 1, y + 1, z);
	adjBlocks[0] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;
	block = m_pChunkSection->getBlock(x, y + 1, z);
	adjBlocks[1] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;
	block = m_pChunkSection->getBlock(x + 1, y + 1, z);
	adjBlocks[2] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;
	block = m_pChunkSection->getBlock(x + 1, y, z);
	adjBlocks[3] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;
	block = m_pChunkSection->getBlock(x + 1, y - 1, z);
	adjBlocks[4] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;
	block = m_pChunkSection->getBlock(x, y - 1, z);
	adjBlocks[5] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;
	block = m_pChunkSection->getBlock(x - 1, y - 1, z);
	adjBlocks[6] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;
	block = m_pChunkSection->getBlock(x - 1, y, z);
	adjBlocks[7] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;

	verticesAO[0] = vertexAO(adjBlocks[5], adjBlocks[7], adjBlocks[6]);
	verticesAO[1] = vertexAO(adjBlocks[3], adjBlocks[5], adjBlocks[4]);
	verticesAO[2] = vertexAO(adjBlocks[1], adjBlocks[3], adjBlocks[2]);
	verticesAO[3] = vertexAO(adjBlocks[7], adjBlocks[1], adjBlocks[0]);

	block = m_pChunkSection->getBlock(x, y, z);
	if (block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube) {
		if (verticesAO[0] == 1.0f)
			verticesAO[0] = 0.666f;
		if (verticesAO[1] == 1.0f)
			verticesAO[1] = 0.666f;
		if (verticesAO[2] == 1.0f)
			verticesAO[2] = 0.666f;
		if (verticesAO[3] == 1.0f)
			verticesAO[3] = 0.666f;
		return;
	}
}

void ChunkMeshBuilder::calculateAO_Back(int x, int y, int z, std::array<GLfloat, 4>& verticesAO)
{
	if (m_pChunkSection->getBlock(x, y, z).getData().isOpaque) {
		verticesAO[0] = 1.0f;
		verticesAO[1] = 1.0f;
		verticesAO[2] = 1.0f;
		verticesAO[3] = 1.0f;
		return;
	}

	std::array<bool, 8> adjBlocks;
	ChunkBlock block;

	block = m_pChunkSection->getBlock(x - 1, y + 1, z);
	adjBlocks[0] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;
	block = m_pChunkSection->getBlock(x, y + 1, z);
	adjBlocks[1] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;
	block = m_pChunkSection->getBlock(x + 1, y + 1, z);
	adjBlocks[2] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;
	block = m_pChunkSection->getBlock(x + 1, y, z);
	adjBlocks[3] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;
	block = m_pChunkSection->getBlock(x + 1, y - 1, z);
	adjBlocks[4] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;
	block = m_pChunkSection->getBlock(x, y - 1, z);
	adjBlocks[5] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;
	block = m_pChunkSection->getBlock(x - 1, y - 1, z);
	adjBlocks[6] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;
	block = m_pChunkSection->getBlock(x - 1, y, z);
	adjBlocks[7] = block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube;

	verticesAO[0] = vertexAO(adjBlocks[3], adjBlocks[5], adjBlocks[4]);
	verticesAO[2] = vertexAO(adjBlocks[7], adjBlocks[1], adjBlocks[0]);
	verticesAO[1] = vertexAO(adjBlocks[5], adjBlocks[7], adjBlocks[6]);
	verticesAO[3] = vertexAO(adjBlocks[1], adjBlocks[3], adjBlocks[2]);

	block = m_pChunkSection->getBlock(x, y, z);
	if (block != 0 && block.getData().id != BlockId::Water &&
		block.getData().meshType == BlockMeshType::Cube) {
		if (verticesAO[0] == 1.0f)
			verticesAO[0] = 0.666f;
		if (verticesAO[1] == 1.0f)
			verticesAO[1] = 0.666f;
		if (verticesAO[2] == 1.0f)
			verticesAO[2] = 0.666f;
		if (verticesAO[3] == 1.0f)
			verticesAO[3] = 0.666f;
		return;
	}
}

float ChunkMeshBuilder::vertexAO(bool side1, bool side2, bool corner)
{
	if (side1 && side2) {
		return 0.333f;
	}
	return (3 - (side1 + side2 + corner)) / 3.0f;
}