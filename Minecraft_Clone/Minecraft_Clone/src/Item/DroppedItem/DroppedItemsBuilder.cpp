#include "DroppedItemsBuilder.h"

#include "DroppedItemsManager.h"
#include "DroppedItemsMesh.h"

#include "World/Block/ChunkBlock.h"
#include "World/Block/BlockData.h"
#include "World/Block/BlockDatabase.h"

#include "RenderSettings.h"
#include "Config.h"
#include "Player/PlayerInfo.h"

#include <iostream>

const float pos = 0.15f;

namespace cubeFaces {

	const std::array<GLfloat, 12> frontFace{
		-pos, -pos, +pos,
		+pos, -pos, +pos,
		+pos, +pos, +pos,
		-pos, +pos, +pos,
	};
	const std::array<GLfloat, 12> backFace{
		+pos, -pos, -pos,
		-pos, -pos, -pos,
		-pos, +pos, -pos,
		+pos, +pos, -pos,
	};
	const std::array<GLfloat, 12> leftFace{
		-pos, -pos, -pos,
		-pos, -pos, +pos,
		-pos, +pos, +pos,
		-pos, +pos, -pos,
	};
	const std::array<GLfloat, 12> rightFace{
		+pos, -pos, +pos,
		+pos, -pos, -pos,
		+pos, +pos, -pos,
		+pos, +pos, +pos,
	};
	const std::array<GLfloat, 12> topFace{
		-pos, +pos, +pos,
		+pos, +pos, +pos,
		+pos, +pos, -pos,
		-pos, +pos, -pos,
	};
	const std::array<GLfloat, 12> bottomFace{
		-pos, -pos, -pos,
		+pos, -pos, -pos,
		+pos, -pos, +pos,
		-pos, -pos, +pos,
	};
} // namespace cubeFaces

namespace xBlockFaces {

	const std::array<GLfloat, 12> xFace1{
		-pos, -pos, -pos,
		+pos, -pos, +pos,
		+pos, +pos, +pos,
		-pos, +pos, -pos,
	};
	const std::array<GLfloat, 12> xFace2{
		-pos, -pos, +pos,
		+pos, -pos, -pos,
		+pos, +pos, -pos,
		-pos, +pos, +pos,
	};
} // namespace xBlockFaces

namespace cactusFaces {

	const static float bias = 1 / 16.0f * 0.3f;

	const std::array<GLfloat, 12> frontFace{
		-pos, -pos, +pos - bias,
		+pos, -pos, +pos - bias,
		+pos, +pos, +pos - bias,
		-pos, +pos, +pos - bias,
	};
	const std::array<GLfloat, 12> backFace{
		+pos, -pos, -pos + bias,
		-pos, -pos, -pos + bias,
		-pos, +pos, -pos + bias,
		+pos, +pos, -pos + bias,
	};
	const std::array<GLfloat, 12> leftFace{
		-pos + bias, -pos, -pos,
		-pos + bias, -pos, +pos,
		-pos + bias, +pos, +pos,
		-pos + bias, +pos, -pos,
	};
	const std::array<GLfloat, 12> rightFace{
		+pos - bias, -pos, +pos,
		+pos - bias, -pos, -pos,
		+pos - bias, +pos, -pos,
		+pos - bias, +pos, +pos,
	};
	const std::array<GLfloat, 12> topFace{
		-pos, +pos, +pos,
		+pos, +pos, +pos,
		+pos, +pos, -pos,
		-pos, +pos, -pos,
	};
	const std::array<GLfloat, 12> bottomFace{
		-pos, -pos, -pos,
		+pos, -pos, -pos,
		+pos, -pos, +pos,
		-pos, -pos, +pos,
	};
} // namespace cactusFaces

namespace {

	const std::array<GLfloat, 12> shadow{
		-pos, -1.9 * pos, -pos,
		+pos, -1.9 * pos, -pos,
		+pos, -1.9 * pos, +pos,
		-pos, -1.9 * pos, +pos,
	};
} // namespace

DroppedItemsBuilder::DroppedItemsBuilder(DroppedItemsManager &droppedItemsManager, DroppedItemsMesh &droppedItemsMesh)
	: m_pDroppedItemsManager{ &droppedItemsManager },
	m_pDroppedItemsMesh{ &droppedItemsMesh }
{
}

void DroppedItemsBuilder::buildMesh()
{
	auto &items = m_pDroppedItemsManager->getDroppedItems();

	for (auto &item : items) {
		ChunkBlock block(item.getItemStack().getMaterial().toBlockID());
		auto &data = block.getData();
		auto position = item.position;

		if (
			glm::pow(position.x - p_info.position.x, 2) +
			glm::pow(position.y - p_info.position.y, 2) +
			glm::pow(position.z - p_info.position.z, 2) >
			glm::pow(g_Config.renderDistance * 16, 2)
			) {
			continue;
		}
		std::array<GLfloat, 8> texCoords;

		if (data.meshType == BlockMeshType::Cube) {
			texCoords = BlockDatabase::get().textureAtlas.getTextureCoords(data.texBottomCoord);
			m_pDroppedItemsMesh->addItem(cubeFaces::bottomFace, texCoords, position);

			texCoords = BlockDatabase::get().textureAtlas.getTextureCoords(data.texTopCoord);
			m_pDroppedItemsMesh->addItem(cubeFaces::topFace, texCoords, position);

			texCoords = BlockDatabase::get().textureAtlas.getTextureCoords(data.texSideCoord);
			m_pDroppedItemsMesh->addItem(cubeFaces::leftFace, texCoords, position);

			texCoords = BlockDatabase::get().textureAtlas.getTextureCoords(data.texSideCoord);
			m_pDroppedItemsMesh->addItem(cubeFaces::rightFace, texCoords, position);

			texCoords = BlockDatabase::get().textureAtlas.getTextureCoords(data.texSideCoord);
			m_pDroppedItemsMesh->addItem(cubeFaces::frontFace, texCoords, position);

			texCoords = BlockDatabase::get().textureAtlas.getTextureCoords(data.texSideCoord);
			m_pDroppedItemsMesh->addItem(cubeFaces::backFace, texCoords, position);



			//if (item.getAcceleration().y == 0.0f) {
			//	texCoords = BlockDatabase::get().textureAtlas.getTextureCoords(sf::Vector2i(1, 15));
			//	m_pDroppedItemsMesh->addItem(shadow, texCoords, position);
			//}
		}
		else if (data.meshType == BlockMeshType::X) {
			texCoords = BlockDatabase::get().textureAtlas.getTextureCoords(data.texTopCoord);
			m_pDroppedItemsMesh->addItem(xBlockFaces::xFace1, texCoords, position);
			m_pDroppedItemsMesh->addItem(xBlockFaces::xFace2, texCoords, position);



			//if (item.getAcceleration().y == 0.0f) {
			//	texCoords = BlockDatabase::get().textureAtlas.getTextureCoords(sf::Vector2i(2, 15));
			//	m_pDroppedItemsMesh->addItem(shadow, texCoords, position);
			//}
		}
		else if (data.meshType == BlockMeshType::Cactus) {
			texCoords = BlockDatabase::get().textureAtlas.getTextureCoords(data.texBottomCoord);
			m_pDroppedItemsMesh->addItem(cactusFaces::bottomFace, texCoords, position);

			texCoords = BlockDatabase::get().textureAtlas.getTextureCoords(data.texTopCoord);
			m_pDroppedItemsMesh->addItem(cactusFaces::topFace, texCoords, position);

			texCoords = BlockDatabase::get().textureAtlas.getTextureCoords(data.texSideCoord);
			m_pDroppedItemsMesh->addItem(cactusFaces::leftFace, texCoords, position);

			texCoords = BlockDatabase::get().textureAtlas.getTextureCoords(data.texSideCoord);
			m_pDroppedItemsMesh->addItem(cactusFaces::rightFace, texCoords, position);

			texCoords = BlockDatabase::get().textureAtlas.getTextureCoords(data.texSideCoord);
			m_pDroppedItemsMesh->addItem(cactusFaces::frontFace, texCoords, position);

			texCoords = BlockDatabase::get().textureAtlas.getTextureCoords(data.texSideCoord);
			m_pDroppedItemsMesh->addItem(cactusFaces::backFace, texCoords, position);



			//if (item.getAcceleration().y == 0.0f) {
			//	texCoords = BlockDatabase::get().textureAtlas.getTextureCoords(sf::Vector2i(1, 15));
			//	m_pDroppedItemsMesh->addItem(shadow, texCoords, position);
			//}
		}
		if (item.getAcceleration().y == 0.0f) {
			texCoords = BlockDatabase::get().textureAtlas.getTextureCoords(sf::Vector2i(0, 15));
			m_pDroppedItemsMesh->addItem(shadow, texCoords, position);
		}
	}
}