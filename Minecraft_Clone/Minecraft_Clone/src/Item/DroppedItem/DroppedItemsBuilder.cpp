#include "DroppedItemsBuilder.h"

#include "DroppedItemsManager.h"
#include "DroppedItemsMesh.h"

#include "Coordinates.h"

#include "World/Block/ChunkBlock.h"
#include "World/Block/BlockData.h"
#include "World/Block/BlockDatabase.h"
#include "World/World/World.h"

#include "RenderSettings.h"
#include "Config.h"
#include "Player/PlayerInfo.h"
#include "Player/Player.h"

#include <iostream>

DroppedItemsBuilder::DroppedItemsBuilder(DroppedItemsManager &droppedItemsManager, DroppedItemsMesh &droppedItemsMesh)
	: m_pDroppedItemsManager{ &droppedItemsManager },
	m_pDroppedItemsMesh{ &droppedItemsMesh }
{
}

void DroppedItemsBuilder::buildMesh(World * world)
{
	for (auto &item : m_pDroppedItemsManager->getDroppedItems()) {
		auto position = item.position;
		// if player is far from item then item isn't being drawn
		if (
			glm::pow(position.x - g_PlayerInfo.player->position.x, 2) +
			glm::pow(position.y - g_PlayerInfo.player->position.y, 2) +
			glm::pow(position.z - g_PlayerInfo.player->position.z, 2) >
			glm::pow(std::max(g_Config.renderDistance - 3, 1) * 16, 2)
			) {
			continue;
		}

		ChunkBlock blockForLighting = world->getBlock(position.x, position.y, position.z);
		float torchLight = blockForLighting.getTorchLight();
		float sunLight = blockForLighting.getSunLight();

		// add shadow texture
		if (item.getAcceleration().y == 0.0f) {
			std::array<GLfloat, 8> texCoords;
			BlockDatabase::get().textureAtlas.getTextureCoords(texCoords, { 0, 15 });
			m_pDroppedItemsMesh->addFace(shadow, texCoords, position, torchLight, sunLight);
		}
		else if (world) {
			float newY = position.y + 0.3f;
			for (int i = position.y - 1; i >= 0; --i) {
				if (world->getBlock(position.x, i, position.z).getData().isCollidable) {
					newY = i + 1.3f;
					break;
				}
			}

			std::array<GLfloat, 8> texCoords;
			BlockDatabase::get().textureAtlas.getTextureCoords(texCoords, { 0, 15 });
			m_pDroppedItemsMesh->addFace(shadow, texCoords, { position.x, newY, position.z }, torchLight, sunLight);
		}

		ChunkBlock block(item.getItemStack().getBlockId());

		if (block.getData().meshType == BlockMeshType::Cube) {
			buildCubeMesh(block, position, torchLight, sunLight);
		}
		else if (block.getData().meshType == BlockMeshType::Cactus) {
			buildCactusMesh(block, position, torchLight, sunLight);
		}
		else { // X or Default
			buildDefaultItemMesh(block, position, torchLight, sunLight);
		}
	}
}

void DroppedItemsBuilder::buildCubeMesh(
	ChunkBlock& block,
	glm::vec3& position,
	float torchLight,
	float sunlight)
{
	auto &data = block.getData();

	std::array<GLfloat, 8> texCoords;

	BlockDatabase::get().textureAtlas.getTextureCoords(texCoords, data.texBottomCoord);
	m_pDroppedItemsMesh->addFace(cubeFaces::bottomFace, texCoords, position, torchLight, sunlight);
	BlockDatabase::get().textureAtlas.getTextureCoords(texCoords, data.texTopCoord);
	m_pDroppedItemsMesh->addFace(cubeFaces::topFace, texCoords, position, torchLight, sunlight);
	BlockDatabase::get().textureAtlas.getTextureCoords(texCoords, data.texSideCoord);
	m_pDroppedItemsMesh->addFace(cubeFaces::leftFace, texCoords, position, torchLight, sunlight);
	BlockDatabase::get().textureAtlas.getTextureCoords(texCoords, data.texSideCoord);
	m_pDroppedItemsMesh->addFace(cubeFaces::rightFace, texCoords, position, torchLight, sunlight);
	BlockDatabase::get().textureAtlas.getTextureCoords(texCoords, data.texSideCoord);
	m_pDroppedItemsMesh->addFace(cubeFaces::frontFace, texCoords, position, torchLight, sunlight);
	BlockDatabase::get().textureAtlas.getTextureCoords(texCoords, data.texSideCoord);
	m_pDroppedItemsMesh->addFace(cubeFaces::backFace, texCoords, position, torchLight, sunlight);
}

void DroppedItemsBuilder::buildCactusMesh(
	ChunkBlock& block,
	glm::vec3& position,
	float torchLight,
	float sunlight)
{
	auto &data = block.getData();

	std::array<GLfloat, 8> texCoords;

	BlockDatabase::get().textureAtlas.getTextureCoords(texCoords, data.texBottomCoord);
	m_pDroppedItemsMesh->addFace(cactusFaces::bottomFace, texCoords, position, torchLight, sunlight);
	BlockDatabase::get().textureAtlas.getTextureCoords(texCoords, data.texTopCoord);
	m_pDroppedItemsMesh->addFace(cactusFaces::topFace, texCoords, position, torchLight, sunlight);
	BlockDatabase::get().textureAtlas.getTextureCoords(texCoords, data.texSideCoord);
	m_pDroppedItemsMesh->addFace(cactusFaces::leftFace, texCoords, position, torchLight, sunlight);
	BlockDatabase::get().textureAtlas.getTextureCoords(texCoords, data.texSideCoord);
	m_pDroppedItemsMesh->addFace(cactusFaces::rightFace, texCoords, position, torchLight, sunlight);
	BlockDatabase::get().textureAtlas.getTextureCoords(texCoords, data.texSideCoord);
	m_pDroppedItemsMesh->addFace(cactusFaces::frontFace, texCoords, position, torchLight, sunlight);
	BlockDatabase::get().textureAtlas.getTextureCoords(texCoords, data.texSideCoord);
	m_pDroppedItemsMesh->addFace(cactusFaces::backFace, texCoords, position, torchLight, sunlight);
}

void DroppedItemsBuilder::buildDefaultItemMesh(
	ChunkBlock& block,
	glm::vec3& position,
	float torchLight,
	float sunlight)
{
	auto &data = block.getData();

	std::array<GLfloat, 8> texCoords;

	position.y += 0.1f;

	BlockDatabase::get().textureAtlas.getTextureCoords(texCoords, data.texTopCoord);
	m_pDroppedItemsMesh->addFace(defaultItemFaces::mainFace1, texCoords, position, torchLight, sunlight);
	m_pDroppedItemsMesh->addFace(defaultItemFaces::mainFace2, texCoords, position, torchLight, sunlight);

	auto pixels = BlockDatabase::get().textureAtlas.getIndivTexturePixels(data.texTopCoord);
	position.x -= HALF_SIZE * defaultItemFaces::ENLARGEMENT_COEF;
	position.y += HALF_SIZE * defaultItemFaces::ENLARGEMENT_COEF;
	position.z -= defaultItemFaces::PIXEL_SIZE / 2.0f;

	int indivTextureSize = BlockDatabase::get().textureAtlas.getIndivTextureSize();
	float pixelSize = (1.0f / 16) / indivTextureSize; // 16 hardcored here

	for (int y = 0; y < indivTextureSize; ++y) {
		for (int x = 0; x < indivTextureSize; ++x) {
			int index = y * indivTextureSize + x;
			if (pixels[index]->a == 0.0f)
				continue;

			auto pixelTexCoords = texCoords;
			//\
					xMin 0 6\
					xMax 2 4\
					yMin 5 7\
					yMax 1 3\
					return { xMin, yMin, xMax, yMin, xMax, yMax, xMin, yMax };
			pixelTexCoords[0] = pixelTexCoords[6] = pixelTexCoords[0] + x * pixelSize; // xMin
			pixelTexCoords[2] = pixelTexCoords[4] = pixelTexCoords[0] + pixelSize; // xMax
			pixelTexCoords[5] = pixelTexCoords[7] = pixelTexCoords[5] + y * pixelSize; // yMin
			pixelTexCoords[1] = pixelTexCoords[3] = pixelTexCoords[5] + pixelSize; // yMax

			auto pos = position;
			pos.x += x * defaultItemFaces::PIXEL_SIZE;
			pos.y -= (y + 1) * defaultItemFaces::PIXEL_SIZE;

			if (x == 0)
				m_pDroppedItemsMesh->addFace(defaultItemFaces::sideFaceLeft, pixelTexCoords, pos, torchLight, sunlight);
			else if (pixels[index - 1]->a == 0.0f)
				m_pDroppedItemsMesh->addFace(defaultItemFaces::sideFaceLeft, pixelTexCoords, pos, torchLight, sunlight);

			if (x == indivTextureSize - 1)
				m_pDroppedItemsMesh->addFace(defaultItemFaces::sideFaceRight, pixelTexCoords, pos, torchLight, sunlight);
			else if (pixels[index + 1]->a == 0.0f)
				m_pDroppedItemsMesh->addFace(defaultItemFaces::sideFaceRight, pixelTexCoords, pos, torchLight, sunlight);

			if (y == 0)
				m_pDroppedItemsMesh->addFace(defaultItemFaces::sideFaceTop, pixelTexCoords, pos, torchLight, sunlight);
			else if (pixels[index - indivTextureSize]->a == 0.0f)
				m_pDroppedItemsMesh->addFace(defaultItemFaces::sideFaceTop, pixelTexCoords, pos, torchLight, sunlight);

			if (y == indivTextureSize - 1)
				m_pDroppedItemsMesh->addFace(defaultItemFaces::sideFaceBottom, pixelTexCoords, pos, torchLight, sunlight);
			else if (pixels[index + indivTextureSize]->a == 0.0f)
				m_pDroppedItemsMesh->addFace(defaultItemFaces::sideFaceBottom, pixelTexCoords, pos, torchLight, sunlight);

			//m_pDroppedItemsMesh->addFace(defaultItemFaces::sideFaceLeft, pixelTexCoords, pos);
			//m_pDroppedItemsMesh->addFace(defaultItemFaces::sideFaceRight, pixelTexCoords, pos);
			//m_pDroppedItemsMesh->addFace(defaultItemFaces::sideFaceTop, pixelTexCoords, pos);
			//m_pDroppedItemsMesh->addFace(defaultItemFaces::sideFaceBottom, pixelTexCoords, pos);
		}
	}
}
