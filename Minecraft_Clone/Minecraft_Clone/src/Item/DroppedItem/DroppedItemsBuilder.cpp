#include "DroppedItemsBuilder.h"

#include "DroppedItemsManager.h"
#include "DroppedItemsMesh.h"

#include "World/Block/ChunkBlock.h"
#include "World/Block/BlockData.h"
#include "World/Block/BlockDatabase.h"

#include "RenderSettings.h"

#include <iostream>

namespace {
	const float pos = 0.15f;
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
}

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
		std::array<GLfloat, 8> texCoords;

		if (data.meshType == BlockMeshType::X) {
			texCoords = BlockDatabase::get().textureAtlas.getTextureCoords(data.texTopCoord);
			m_pDroppedItemsMesh->addItem(xFace1, texCoords, position);
			m_pDroppedItemsMesh->addItem(xFace2, texCoords, position);
		}
		else {
			texCoords = BlockDatabase::get().textureAtlas.getTextureCoords(data.texBottomCoord);
			m_pDroppedItemsMesh->addItem(bottomFace, texCoords, position);

			texCoords = BlockDatabase::get().textureAtlas.getTextureCoords(data.texTopCoord);
			m_pDroppedItemsMesh->addItem(topFace, texCoords, position);

			texCoords = BlockDatabase::get().textureAtlas.getTextureCoords(data.texSideCoord);
			m_pDroppedItemsMesh->addItem(leftFace, texCoords, position);

			texCoords = BlockDatabase::get().textureAtlas.getTextureCoords(data.texSideCoord);
			m_pDroppedItemsMesh->addItem(rightFace, texCoords, position);

			texCoords = BlockDatabase::get().textureAtlas.getTextureCoords(data.texSideCoord);
			m_pDroppedItemsMesh->addItem(frontFace, texCoords, position);

			texCoords = BlockDatabase::get().textureAtlas.getTextureCoords(data.texSideCoord);
			m_pDroppedItemsMesh->addItem(backFace, texCoords, position);
		}
	}
}