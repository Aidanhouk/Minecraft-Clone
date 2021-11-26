#include "ChunkMeshBuilder.h"

#include "ChunkMesh.h"
#include "ChunkSection.h"

#include "../Block/BlockData.h"
#include "../Block/BlockDatabase.h"

#include <SFML/System/Clock.hpp>
#include <cassert>
#include <iostream>
#include <vector>

namespace cubeFaces {

	const std::array<GLfloat, 12> frontFace{
		0, 0, 1,
		1, 0, 1,
		1, 1, 1,
		0, 1, 1,
	};
	const std::array<GLfloat, 12> backFace{
		1, 0, 0,
		0, 0, 0,
		0, 1, 0,
		1, 1, 0,
	};
	const std::array<GLfloat, 12> leftFace{
		0, 0, 0,
		0, 0, 1,
		0, 1, 1,
		0, 1, 0,
	};
	const std::array<GLfloat, 12> rightFace{
		1, 0, 1,
		1, 0, 0,
		1, 1, 0,
		1, 1, 1,
	};
	const std::array<GLfloat, 12> topFace{
		0, 1, 1,
		1, 1, 1,
		1, 1, 0,
		0, 1, 0,
	};
	const std::array<GLfloat, 12> bottomFace{
		0, 0, 0,
		1, 0, 0,
		1, 0, 1,
		0, 0, 1,
	};

} // namespace cubeFaces

namespace xBlockFaces {

	const std::array<GLfloat, 12> xFace1{
		0, 0, 0,
		1, 0, 1,
		1, 1, 1,
		0, 1, 0,
	};
	const std::array<GLfloat, 12> xFace1back{
		0, 0, 0,
		0, 1, 0,
		1, 1, 1,
		1, 0, 1,
	};
	const std::array<GLfloat, 12> xFace2{
		0, 0, 1,
		1, 0, 0,
		1, 1, 0,
		0, 1, 1,
	};
	const std::array<GLfloat, 12> xFace2back{
		0, 0, 1,
		0, 1, 1,
		1, 1, 0,
		1, 0, 0,
	};

} // namespace xBlockFaces

namespace cactusFaces {

	const static float bias = 1 / 16.0f;

	const std::array<GLfloat, 12> frontFace{
		0, 0, 1 - bias,
		1, 0, 1 - bias,
		1, 1, 1 - bias,
		0, 1, 1 - bias,
	};
	const std::array<GLfloat, 12> backFace{
		1, 0, bias,
		0, 0, bias,
		0, 1, bias,
		1, 1, bias,
	};
	const std::array<GLfloat, 12> leftFace{
		bias, 0, 0,
		bias, 0, 1,
		bias, 1, 1,
		bias, 1, 0,
	};
	const std::array<GLfloat, 12> rightFace{
		1 - bias, 0, 1,
		1 - bias, 0, 0,
		1 - bias, 1, 0,
		1 - bias, 1, 1,
	};
	const std::array<GLfloat, 12> topFace{
		0, 1, 1,
		1, 1, 1,
		1, 1, 0,
		0, 1, 0,
	};
	const std::array<GLfloat, 12> bottomFace{
		0, 0, 0,
		1, 0, 0,
		1, 0, 1,
		0, 0, 1,
	};

} // namespace cactusFaces

namespace {

constexpr GLfloat LIGHT_TOP = 1.0f;
constexpr GLfloat LIGHT_LEFT = 0.9f;
constexpr GLfloat LIGHT_RIGHT = 0.8f;
constexpr GLfloat LIGHT_BACK = 0.7f;
constexpr GLfloat LIGHT_FRONT = 0.6f;
constexpr GLfloat LIGHT_BOT = 0.5f;

} // namespace

ChunkMeshBuilder::ChunkMeshBuilder(ChunkSection &chunk,
                                   ChunkMeshCollection &mesh)
    : m_pChunkSection(&chunk)
    , m_pMeshes(&mesh)
{
}

struct AdjacentBlockPositions {
    void update(int x, int y, int z)
    {
        up		= {x,		y + 1,	z};
        down	= {x,		y - 1,	z};
        left	= {x - 1,	y,		z};
        right	= {x + 1,	y,		z};
        front	= {x,		y,		z + 1};
        back	= {x,		y,		z - 1};
    }

    sf::Vector3i up;
    sf::Vector3i down;
    sf::Vector3i left;
    sf::Vector3i right;
    sf::Vector3i front;
    sf::Vector3i back;
};

void ChunkMeshBuilder::buildMesh()
{
	static int i = 1;

    AdjacentBlockPositions directions;
    m_pBlockPtr = m_pChunkSection->begin();
	for (uint8_t y = 0; y < CHUNK_SIZE; ++y) {

		if (!shouldMakeLayer(y))
			continue;

		for (uint8_t z = 0; z < CHUNK_SIZE; ++z)
			for (uint8_t x = 0; x < CHUNK_SIZE; ++x) {

				ChunkBlock block = *m_pBlockPtr++;

				if (block == 0)
					continue;

#ifdef _DEBUG
				if (block.id >= (Block_t)BlockId::NUM_TYPES) {
					block = 0;
					continue;
				}
#endif // DEBUG

				sf::Vector3i position(x, y, z);
				setActiveMesh(block);

				m_pBlockData = &block.getData();
				auto &data = *m_pBlockData;

				if (data.meshType == BlockMeshType::Cube) {
					directions.update(x, y, z);

					// Up/ Down
					tryAddFaceToMesh(cubeFaces::topFace, data.texTopCoord, position, directions.up, LIGHT_TOP);
					if ((m_pChunkSection->getLocation().y != 0) || y != 0) {
						tryAddFaceToMesh(cubeFaces::bottomFace, data.texBottomCoord, position, directions.down, LIGHT_BOT);
						// Left/ Right
						tryAddFaceToMesh(cubeFaces::leftFace, data.texSideCoord, position, directions.left, LIGHT_LEFT);
						tryAddFaceToMesh(cubeFaces::rightFace, data.texSideCoord, position, directions.right, LIGHT_RIGHT);
						// Front/ Back
						tryAddFaceToMesh(cubeFaces::frontFace, data.texSideCoord, position, directions.front, LIGHT_FRONT);
						tryAddFaceToMesh(cubeFaces::backFace, data.texSideCoord, position, directions.back, LIGHT_BACK);
					}
				}
				else if (data.meshType == BlockMeshType::X) {
					addXBlockToMesh(data.texTopCoord, position);
				}
				else if (data.meshType == BlockMeshType::Cactus) {
					directions.update(x, y, z);

					// Up/ Down
					tryAddFaceToMesh(cactusFaces::bottomFace,	data.texBottomCoord,	position, directions.down,	LIGHT_BOT);
					tryAddFaceToMesh(cactusFaces::topFace,		data.texTopCoord,		position, directions.up,	LIGHT_TOP);
					// Left/ Right
					tryAddFaceToMesh(cactusFaces::leftFace,		data.texSideCoord,		position, directions.left,	LIGHT_LEFT);
					tryAddFaceToMesh(cactusFaces::rightFace,	data.texSideCoord,		position, directions.right, LIGHT_RIGHT);
					// Front/ Back
					tryAddFaceToMesh(cactusFaces::frontFace,	data.texSideCoord,		position, directions.front,	LIGHT_FRONT);
					tryAddFaceToMesh(cactusFaces::backFace,		data.texSideCoord,		position, directions.back,	LIGHT_BACK);
				}
			}
    }
}

void ChunkMeshBuilder::setActiveMesh(ChunkBlock block)
{
    switch (block.getData().shaderType) {
        case BlockShaderType::Cube:
            m_pActiveMesh = &m_pMeshes->solidMesh;
            break;

        case BlockShaderType::Liquid:
            m_pActiveMesh = &m_pMeshes->waterMesh;
            break;

        case BlockShaderType::Flora:
            m_pActiveMesh = &m_pMeshes->floraMesh;
            break;
    }
}

void ChunkMeshBuilder::addXBlockToMesh(const sf::Vector2i &textureCoords,
                                       const sf::Vector3i &blockPosition)
{
	std::array<GLfloat, 8> texCoords;
	BlockDatabase::get().textureAtlas.getTextureCoords(texCoords, textureCoords);
	m_pActiveMesh->addFace(xBlockFaces::xFace1, texCoords, m_pChunkSection->getLocation(), blockPosition, LIGHT_LEFT);
	m_pActiveMesh->addFace(xBlockFaces::xFace2, texCoords, m_pChunkSection->getLocation(), blockPosition, LIGHT_RIGHT);

	std::array<GLfloat, 8> texCoordsBack{
		texCoords[0], texCoords[1],
		texCoords[6], texCoords[7],
		texCoords[4], texCoords[5],
		texCoords[2], texCoords[3],
	};
	m_pActiveMesh->addFace(xBlockFaces::xFace1back, texCoordsBack, m_pChunkSection->getLocation(), blockPosition, LIGHT_LEFT);
	m_pActiveMesh->addFace(xBlockFaces::xFace2back, texCoordsBack, m_pChunkSection->getLocation(), blockPosition, LIGHT_RIGHT);
}

void ChunkMeshBuilder::tryAddFaceToMesh(
	const std::array<GLfloat, 12> &blockFace,
	const sf::Vector2i &textureCoords,
	const sf::Vector3i &blockPosition,
	const sf::Vector3i &blockFacing,
	GLfloat cardinalLight)
{
	if (shouldMakeFace(blockFacing, *m_pBlockData)) {
		std::array<GLfloat, 8> texCoords;
		BlockDatabase::get().textureAtlas.getTextureCoords(texCoords, textureCoords);
		m_pActiveMesh->addFace(
			blockFace,
			texCoords,
			m_pChunkSection->getLocation(),
			blockPosition,
			cardinalLight);
	}
}

bool ChunkMeshBuilder::shouldMakeFace(const sf::Vector3i &adjBlockPos,
                                      const BlockDataHolder &blockData)
{
    auto adjBlock = m_pChunkSection->getBlock(adjBlockPos.x, adjBlockPos.y, adjBlockPos.z);
    auto &data = adjBlock.getData();

    if (adjBlock == 0) {
        return true;
    }
    else if (!data.isOpaque) {
		if (data.id != m_pBlockData->id
			|| data.shaderType == BlockShaderType::Flora
			|| data.meshType == BlockMeshType::Cactus
			) {
			return true;
		}
    }
    return false;
}

bool ChunkMeshBuilder::shouldMakeLayer(int y)
{
    auto adjIsSolid = [&](int dx, int dz) {
        const ChunkSection &sect = m_pChunkSection->getAdjacent(dx, dz);
        return sect.getLayer(y).isAllSolid();
    };

    return (!m_pChunkSection->getLayer(y).isAllSolid()) ||
           (!m_pChunkSection->getLayer(y + 1).isAllSolid()) ||
           (!m_pChunkSection->getLayer(y - 1).isAllSolid()) ||
           (!adjIsSolid(1, 0)) || (!adjIsSolid(0, 1)) || (!adjIsSolid(-1, 0)) || (!adjIsSolid(0, -1));
}