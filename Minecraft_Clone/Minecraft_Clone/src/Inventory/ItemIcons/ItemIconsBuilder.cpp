#include "ItemIconsBuilder.h"

#include "../IconsMesh.h"
#include "../Inventory.h"
#include "../IconDatabase.h"

#include "World/Block/ChunkBlock.h"
#include "World/Block/BlockData.h"

#include "RenderSettings.h"

#include <iostream>

const std::array<sf::Vector2i, 10> DigitsTexturePositions{
	sf::Vector2i{0 * 6, 8},
	sf::Vector2i{1 * 6, 8},
	sf::Vector2i{2 * 6, 8},
	sf::Vector2i{3 * 6, 8},
	sf::Vector2i{4 * 6, 8},
	sf::Vector2i{5 * 6, 8},
	sf::Vector2i{6 * 6, 8},
	sf::Vector2i{7 * 6, 8},
	sf::Vector2i{8 * 6, 8},
	sf::Vector2i{9 * 6, 8},
};

ItemIconsBuilder::ItemIconsBuilder(Inventory &inventory, IconsMesh &itemIconsMesh)
	: m_pInventory{ &inventory },
	m_pItemIconsMesh{ &itemIconsMesh }
{
	m_invSlotSize = m_pInventory->getInvSlotSize();
	m_toolbarSlotSize = m_pInventory->getToolbarSlotSize();
}

void ItemIconsBuilder::buildMesh()
{
	auto &invSlots = m_pInventory->getInvSlots();

	if (m_pInventory->isInventoryOpened()) {
		for (int i = 0; i < invSlots.size(); ++i) {
			if (invSlots[i].item.getBlockId() != EMPTY_SLOT_ID) {
				buildIcon(invSlots[i]);
			}
		}
	}
	else {
		for (int i = 0; i < 9; ++i) {
			if (invSlots[i].item.getBlockId() != EMPTY_SLOT_ID) {
				buildToolbarIcon(invSlots[i], m_pInventory->getToolbarSlotsPos()[i]);
			}
		}
	}
}

void ItemIconsBuilder::buildIcon(ItemSlot &slot)
{
	static const float RESX = g_RenderSettings.resolutionX;
	static const float RESY = g_RenderSettings.resolutionY;

	std::array<GLfloat, 12> vertexPos{
		(slot.position.x)					/ RESX,	(RESY - slot.position.y)					/ RESY,	0.0f,
		(slot.position.x + m_invSlotSize)	/ RESX,	(RESY - slot.position.y)					/ RESY,	0.0f,
		(slot.position.x + m_invSlotSize)	/ RESX,	(RESY - slot.position.y + m_invSlotSize)	/ RESY,	0.0f,
		(slot.position.x)					/ RESX,	(RESY - slot.position.y + m_invSlotSize)	/ RESY,	0.0f,
	};

	for (auto & vertex : vertexPos) {
		vertex = vertex * 2 - 1.0f;
	}

	ChunkBlock block(slot.item.getBlockId());
	auto textureCoords = block.getData().texSideCoord;

	std::array<GLfloat, 8> texCoords;
	IconDatabase::get().textureAtlas.getTextureCoords(texCoords, textureCoords);

	m_pItemIconsMesh->addIcon(vertexPos, texCoords);



	int number = slot.item.getNumInStack();
	// Like in Minecraft
	if (number == 1)
		return;

	int bias = m_toolbarSlotSize * 0.05f;

	if (number >= 10) {
		// 1st digit
		buildDigit(number / 10, sf::Vector2i(
			slot.position.x + m_invSlotSize - m_invSlotSize / 2.7f + bias,
			RESY - slot.position.y - bias), m_invSlotSize);
		// 2nd digit
		buildDigit(number % 10, sf::Vector2i(
			slot.position.x + m_invSlotSize + bias,
			RESY - slot.position.y - bias), m_invSlotSize);
	}
	else {
		buildDigit(number, sf::Vector2i(
			slot.position.x + m_invSlotSize + bias,
			RESY - slot.position.y - bias), m_invSlotSize);
	}
}

void ItemIconsBuilder::buildToolbarIcon(ItemSlot & slot, sf::Vector2i &toolbarSlotPos)
{
	static const float RESX = g_RenderSettings.resolutionX;
	static const float RESY = g_RenderSettings.resolutionY;

	std::array<GLfloat, 12> vertexPos{
		(toolbarSlotPos.x)						/ RESX, (RESY - toolbarSlotPos.y)						/ RESY,	0.0f,
		(toolbarSlotPos.x + m_toolbarSlotSize)	/ RESX, (RESY - toolbarSlotPos.y)						/ RESY,	0.0f,
		(toolbarSlotPos.x + m_toolbarSlotSize)	/ RESX, (RESY - toolbarSlotPos.y + m_toolbarSlotSize)	/ RESY,	0.0f,
		(toolbarSlotPos.x)						/ RESX, (RESY - toolbarSlotPos.y + m_toolbarSlotSize)	/ RESY,	0.0f,
	};

	for (auto & vertex : vertexPos) {
		vertex = vertex * 2 - 1.0f;
	}

	ChunkBlock block(slot.item.getBlockId());
	auto textureCoords = block.getData().texSideCoord;

	std::array<GLfloat, 8> texCoords;
	IconDatabase::get().textureAtlas.getTextureCoords(texCoords, textureCoords);

	m_pItemIconsMesh->addIcon(vertexPos, texCoords);



	int number = slot.item.getNumInStack();
	// Like in Minecraft
	if (number == 1)
		return;

	int bias = m_toolbarSlotSize * 0.05f;

	if (number >= 10) {
		// 1st digit
		buildDigit(number / 10, sf::Vector2i(
			toolbarSlotPos.x + m_toolbarSlotSize - m_toolbarSlotSize / 2.7f + bias,
			RESY - toolbarSlotPos.y - bias), m_toolbarSlotSize);
		// 2nd digit
		buildDigit(number % 10, sf::Vector2i(
			toolbarSlotPos.x + m_toolbarSlotSize + bias,
			RESY - toolbarSlotPos.y - bias), m_toolbarSlotSize);
	}
	else {
		buildDigit(number, sf::Vector2i(
			toolbarSlotPos.x + m_toolbarSlotSize + bias,
			RESY - toolbarSlotPos.y - bias), m_toolbarSlotSize);
	}
}

void ItemIconsBuilder::buildDigit(int digit, sf::Vector2i rightBottomPos, float slotSize)
{
	static const float RESX = g_RenderSettings.resolutionX;
	static const float RESY = g_RenderSettings.resolutionY;

	float digitWidth = slotSize / 2.7f;
	float digitHeight = digitWidth * 8 / 6;

	std::array<GLfloat, 12> vertexPos{
		(rightBottomPos.x - digitWidth)	/ RESX, (rightBottomPos.y)					/ RESY,	0.0f,
		(rightBottomPos.x)				/ RESX, (rightBottomPos.y)					/ RESY,	0.0f,
		(rightBottomPos.x)				/ RESX, (rightBottomPos.y + digitHeight)	/ RESY,	0.0f,
		(rightBottomPos.x - digitWidth)	/ RESX, (rightBottomPos.y + digitHeight)	/ RESY,	0.0f,
	};

	for (auto & vertex : vertexPos) {
		vertex = vertex * 2 - 1.0f;
	}

	sf::Vector2i textureStarPos = DigitsTexturePositions[digit];
	std::array<GLfloat, 8U> texCoords{
		textureStarPos.x,		textureStarPos.y,
		textureStarPos.x + 6,	textureStarPos.y,
		textureStarPos.x + 6,	textureStarPos.y - 8,
		textureStarPos.x,		textureStarPos.y - 8,
	};

	for (auto & textCoord : texCoords)
		textCoord /= IconDatabase::get().textureAtlas.getAtlasSize();

	m_pItemIconsMesh->addIcon(vertexPos, texCoords);
}