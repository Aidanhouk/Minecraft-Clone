#include "ChunkMeshBuilder.h"

#include "../ChunkSection.h"

#include "../../AO_Constants.h"

namespace {
	bool isBlockMakesAO(ChunkBlock block);
	float vertexAO(bool side1, bool side2, bool corner);
}

void ChunkMeshBuilder::calculateAO_Top(int x, int y, int z, std::array<GLfloat, 4>& verticesAO)
{
    calculateAO(x, y, z, verticesAO, ADJ_BLOCKS_DATA_TOP, AO_DATA_TOP);
}

void ChunkMeshBuilder::calculateAO_Bottom(int x, int y, int z, std::array<GLfloat, 4>& verticesAO)
{
    calculateAO(x, y, z, verticesAO, ADJ_BLOCKS_DATA_BOTTOM, AO_DATA_BOTTOM);
}

void ChunkMeshBuilder::calculateAO_Left(int x, int y, int z, std::array<GLfloat, 4>& verticesAO)
{
    calculateAO(x, y, z, verticesAO, ADJ_BLOCKS_DATA_LEFT, AO_DATA_LEFT);
}

void ChunkMeshBuilder::calculateAO_Right(int x, int y, int z, std::array<GLfloat, 4>& verticesAO)
{
    calculateAO(x, y, z, verticesAO, ADJ_BLOCKS_DATA_RIGHT, AO_DATA_RIGHT);
}

void ChunkMeshBuilder::calculateAO_Front(int x, int y, int z, std::array<GLfloat, 4>& verticesAO)
{
    calculateAO(x, y, z, verticesAO, ADJ_BLOCKS_DATA_FRONT, AO_DATA_FRONT);
}

void ChunkMeshBuilder::calculateAO_Back(int x, int y, int z, std::array<GLfloat, 4>& verticesAO)
{
    calculateAO(x, y, z, verticesAO, ADJ_BLOCKS_DATA_BACK, AO_DATA_BACK);
}

void ChunkMeshBuilder::calculateAO(int x, int y, int z, std::array<GLfloat, 4>& verticesAO,
    const ADJ_BLOCKS_DATA& adj_blocks_data, const AO_DATA& AO_data)
{
    if (m_pChunkSection->getBlock(x, y, z).isOpaque()) {
        for (auto vertexAO : verticesAO)
            vertexAO = AO_FULL;

        return;
    }

    std::array<bool, 8> adjBlocks;

    for (size_t i = 0; i < adjBlocks.size(); ++i) {
        auto xx = x + adj_blocks_data[i][0];
        auto yy = y + adj_blocks_data[i][1];
        auto zz = z + adj_blocks_data[i][2];
        adjBlocks[0] = isBlockMakesAO(m_pChunkSection->getBlock(xx, yy, zz));
    }

    for (size_t i = 0; i < verticesAO.size(); ++i) {
        auto side1 = adjBlocks[AO_data[i][0]];
        auto side2 = adjBlocks[AO_data[i][1]];
        auto corner = adjBlocks[AO_data[i][2]];
        verticesAO[i] = vertexAO(side1, side2, corner);
    }

    if (isBlockMakesAO(m_pChunkSection->getBlock(x, y, z))) {
        for (auto vertexAO : verticesAO) {
            if (vertexAO == AO_FULL)
                vertexAO = AO_66;
        }
    }
}

namespace {
	bool isBlockMakesAO(ChunkBlock block)
	{
		return
			block != 0 &&
			block.getData().id != BlockId::Water &&
			block.isMeshCube();
	}

	float vertexAO(bool side1, bool side2, bool corner)
	{
		if (side1 && side2) {
			return AO_33;
		}
		return (3 - (side1 + side2 + corner)) / 3.0f;
	}
}