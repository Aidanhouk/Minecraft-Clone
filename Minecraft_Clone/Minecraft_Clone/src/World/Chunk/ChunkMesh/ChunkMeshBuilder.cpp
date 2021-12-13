#include "ChunkMeshBuilder.h"

#include "ChunkMesh.h"
#include "../ChunkSection.h"
#include "../../Block/BlockData.h"
#include "../../Block/BlockDatabase.h"

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

//namespace {
//
//constexpr GLfloat LIGHT_TOP = 1.0f;
//constexpr GLfloat LIGHT_LEFT = 0.9f;
//constexpr GLfloat LIGHT_RIGHT = 0.8f;
//constexpr GLfloat LIGHT_BACK = 0.7f;
//constexpr GLfloat LIGHT_FRONT = 0.6f;
//constexpr GLfloat LIGHT_BOT = 0.5f;
//
//} // namespace

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

				float torchLight, sunlight;

				if (data.meshType == BlockMeshType::Cube) {
					directions.update(x, y, z);

					std::array<GLfloat, 4> ambientOcclusion;

					// Top
					torchLight = m_pChunkSection->getBlock(x, y + 1, z).getTorchLight();
					sunlight = m_pChunkSection->getBlock(x, y + 1, z).getSunLight();
					ambientOcclusion[0] = block.getAO_Top_LeftBack() * 0.333f;
					ambientOcclusion[1] = block.getAO_Top_RightBack() * 0.333f;
					ambientOcclusion[2] = block.getAO_Top_RightFront() * 0.333f;
					ambientOcclusion[3] = block.getAO_Top_LeftFront() * 0.333f;
					//for (int i = 0; i < 4; i++)
					//	if (ambientOcclusion[i] < 0.9f)
					//		std::cout << ambientOcclusion[i] << "\n";
					tryAddFaceToMesh(cubeFaces::topFace, data.texTopCoord, position, directions.up, ambientOcclusion, torchLight, sunlight);
					if ((m_pChunkSection->getLocation().y != 0) || y != 0) {
						// Bottom
						ambientOcclusion[0] = block.getAO_Bottom_LeftBack() * 0.333f;
						ambientOcclusion[1] = block.getAO_Bottom_RightBack() * 0.333f;
						ambientOcclusion[2] = block.getAO_Bottom_RightFront() * 0.333f;
						ambientOcclusion[3] = block.getAO_Bottom_LeftFront() * 0.333f;
						torchLight = m_pChunkSection->getBlock(x, y - 1, z).getTorchLight();
						sunlight = m_pChunkSection->getBlock(x, y - 1, z).getSunLight();
						tryAddFaceToMesh(cubeFaces::bottomFace, data.texBottomCoord, position, directions.down, ambientOcclusion, torchLight, sunlight);
						// Left
						ambientOcclusion[0] = block.getAO_Left_BottomBack() * 0.333f;
						ambientOcclusion[1] = block.getAO_Left_BottomFront() * 0.333f;
						ambientOcclusion[2] = block.getAO_Left_TopFront() * 0.333f;
						ambientOcclusion[3] = block.getAO_Left_TopBack() * 0.333f;
						torchLight = m_pChunkSection->getBlock(x - 1, y, z).getTorchLight();
						sunlight = m_pChunkSection->getBlock(x - 1, y, z).getSunLight();
						tryAddFaceToMesh(cubeFaces::leftFace, data.texSideCoord, position, directions.left, ambientOcclusion, torchLight, sunlight);
						// Right
						ambientOcclusion[0] = block.getAO_Right_BottomFront() * 0.333f;
						ambientOcclusion[1] = block.getAO_Right_BottomBack() * 0.333f;
						ambientOcclusion[2] = block.getAO_Right_TopBack() * 0.333f;
						ambientOcclusion[3] = block.getAO_Right_TopFront() * 0.333f;
						torchLight = m_pChunkSection->getBlock(x + 1, y, z).getTorchLight();
						sunlight = m_pChunkSection->getBlock(x + 1, y, z).getSunLight();
						tryAddFaceToMesh(cubeFaces::rightFace, data.texSideCoord, position, directions.right, ambientOcclusion, torchLight, sunlight);
						// Front
						ambientOcclusion[0] = block.getAO_Front_BottomLeft() * 0.333f;
						ambientOcclusion[1] = block.getAO_Front_BottomRight() * 0.333f;
						ambientOcclusion[2] = block.getAO_Front_TopRight() * 0.333f;
						ambientOcclusion[3] = block.getAO_Front_TopLeft() * 0.333f;
						torchLight = m_pChunkSection->getBlock(x, y, z + 1).getTorchLight();
						sunlight = m_pChunkSection->getBlock(x, y, z + 1).getSunLight();
						tryAddFaceToMesh(cubeFaces::frontFace, data.texSideCoord, position, directions.front, ambientOcclusion, torchLight, sunlight);
						// Back
						ambientOcclusion[0] = block.getAO_Back_BottomLeft() * 0.333f;
						ambientOcclusion[1] = block.getAO_Back_BottomRight() * 0.333f;
						ambientOcclusion[2] = block.getAO_Back_TopRight() * 0.333f;
						ambientOcclusion[3] = block.getAO_Back_TopLeft() * 0.333f;
						torchLight = m_pChunkSection->getBlock(x, y, z - 1).getTorchLight(); 
						sunlight = m_pChunkSection->getBlock(x, y, z - 1).getSunLight();
						tryAddFaceToMesh(cubeFaces::backFace, data.texSideCoord, position, directions.back, ambientOcclusion, torchLight, sunlight);
					}
				}
				else if (data.meshType == BlockMeshType::X) {
					torchLight = m_pChunkSection->getBlock(x, y, z).getTorchLight();
					sunlight = m_pChunkSection->getBlock(x, y, z).getSunLight();
					std::array<GLfloat, 8> texCoords;
					BlockDatabase::get().textureAtlas.getTextureCoords(texCoords, data.texTopCoord);

					m_pActiveMesh->addFace(xBlockFaces::xFace1, texCoords, m_pChunkSection->getLocation(), position, torchLight, sunlight);
					m_pActiveMesh->addFace(xBlockFaces::xFace2, texCoords, m_pChunkSection->getLocation(), position, torchLight, sunlight);

					std::array<GLfloat, 8> texCoordsBack{
						texCoords[0], texCoords[1],
						texCoords[6], texCoords[7],
						texCoords[4], texCoords[5],
						texCoords[2], texCoords[3],
					};
					m_pActiveMesh->addFace(xBlockFaces::xFace1back, texCoordsBack, m_pChunkSection->getLocation(), position, torchLight, sunlight);
					m_pActiveMesh->addFace(xBlockFaces::xFace2back, texCoordsBack, m_pChunkSection->getLocation(), position, torchLight, sunlight);
				}
				else if (data.meshType == BlockMeshType::Cactus) {
					directions.update(x, y, z);

					// I don't use AO on cactuses. Uncomment statements below to use AO.
					std::array<GLfloat, 4> ambientOcclusion{ 1.0f,1.0f,1.0f,1.0f };
					// Top
					//ambientOcclusion[0] = block.getAO_Top_LeftBack() * 0.333f;
					//ambientOcclusion[1] = block.getAO_Top_RightBack() * 0.333f;
					//ambientOcclusion[2] = block.getAO_Top_RightFront() * 0.333f;
					//ambientOcclusion[3] = block.getAO_Top_LeftFront() * 0.333f;
					torchLight = m_pChunkSection->getBlock(x, y + 1, z).getTorchLight();
					sunlight = m_pChunkSection->getBlock(x, y + 1, z).getSunLight();
					tryAddFaceToMesh(cactusFaces::bottomFace, data.texBottomCoord, position, directions.down, ambientOcclusion, torchLight, sunlight);
					// Bottom
					//ambientOcclusion[0] = block.getAO_Bottom_LeftBack() * 0.333f;
					//ambientOcclusion[1] = block.getAO_Bottom_RightBack() * 0.333f;
					//ambientOcclusion[2] = block.getAO_Bottom_RightFront() * 0.333f;
					//ambientOcclusion[3] = block.getAO_Bottom_LeftFront() * 0.333f;
					torchLight = m_pChunkSection->getBlock(x, y - 1, z).getTorchLight();
					sunlight = m_pChunkSection->getBlock(x, y - 1, z).getSunLight();
					tryAddFaceToMesh(cactusFaces::topFace, data.texTopCoord, position, directions.up, ambientOcclusion, torchLight, sunlight);
					// Left
					//ambientOcclusion[0] = block.getAO_Left_BottomBack() * 0.333f;
					//ambientOcclusion[1] = block.getAO_Left_BottomFront() * 0.333f;
					//ambientOcclusion[2] = block.getAO_Left_TopFront() * 0.333f;
					//ambientOcclusion[3] = block.getAO_Left_TopBack() * 0.333f;
					torchLight = m_pChunkSection->getBlock(x - 1, y, z).getTorchLight();
					sunlight = m_pChunkSection->getBlock(x - 1, y, z).getSunLight();
					tryAddFaceToMesh(cactusFaces::leftFace, data.texSideCoord, position, directions.left, ambientOcclusion, torchLight, sunlight);
					// Right
					//ambientOcclusion[0] = block.getAO_Right_BottomFront() * 0.333f;
					//ambientOcclusion[1] = block.getAO_Right_BottomBack() * 0.333f;
					//ambientOcclusion[2] = block.getAO_Right_TopBack() * 0.333f;
					//ambientOcclusion[3] = block.getAO_Right_TopFront() * 0.333f;
					torchLight = m_pChunkSection->getBlock(x + 1, y, z).getTorchLight();
					sunlight = m_pChunkSection->getBlock(x + 1, y, z).getSunLight();
					tryAddFaceToMesh(cactusFaces::rightFace, data.texSideCoord, position, directions.right, ambientOcclusion, torchLight, sunlight);
					// Front
					//ambientOcclusion[0] = block.getAO_Front_BottomLeft() * 0.333f;
					//ambientOcclusion[1] = block.getAO_Front_BottomRight() * 0.333f;
					//ambientOcclusion[2] = block.getAO_Front_TopRight() * 0.333f;
					//ambientOcclusion[3] = block.getAO_Front_TopLeft() * 0.333f;
					torchLight = m_pChunkSection->getBlock(x, y, z + 1).getTorchLight();
					sunlight = m_pChunkSection->getBlock(x, y, z + 1).getSunLight();
					tryAddFaceToMesh(cactusFaces::frontFace, data.texSideCoord, position, directions.front, ambientOcclusion, torchLight, sunlight);
					// Back
					//ambientOcclusion[0] = block.getAO_Back_BottomLeft() * 0.333f;
					//ambientOcclusion[1] = block.getAO_Back_BottomRight() * 0.333f;
					//ambientOcclusion[2] = block.getAO_Back_TopRight() * 0.333f;
					//ambientOcclusion[3] = block.getAO_Back_TopLeft() * 0.333f;
					torchLight = m_pChunkSection->getBlock(x, y, z - 1).getTorchLight();
					sunlight = m_pChunkSection->getBlock(x, y, z - 1).getSunLight();
					tryAddFaceToMesh(cactusFaces::backFace, data.texSideCoord, position, directions.back, ambientOcclusion, torchLight, sunlight);
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

void ChunkMeshBuilder::tryAddFaceToMesh(
	const std::array<GLfloat, 12> &blockFace,
	const sf::Vector2i &textureCoords,
	const sf::Vector3i &blockPosition,
	const sf::Vector3i &blockFacing,
	const std::array<GLfloat, 4> &ambientOcclusion,
	GLfloat torchLight,
	GLfloat sunLight)
{
	if (shouldMakeFace(blockFacing, *m_pBlockData)) {
		std::array<GLfloat, 8> texCoords;
		BlockDatabase::get().textureAtlas.getTextureCoords(texCoords, textureCoords);
		m_pActiveMesh->addFace(
			blockFace,
			texCoords,
			m_pChunkSection->getLocation(),
			blockPosition,
			torchLight,
			sunLight,
			ambientOcclusion);
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