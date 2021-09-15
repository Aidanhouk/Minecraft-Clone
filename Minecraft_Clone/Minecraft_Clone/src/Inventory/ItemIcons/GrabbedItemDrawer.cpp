#include "GrabbedItemDrawer.h"

#include "../IconDatabase.h"
#include "../Inventory.h"
#include "World/Block/ChunkBlock.h"
#include "World/Block/BlockData.h"
#include "Renderer/RenderMaster.h"

#include <iostream>

GrabbedItemDrawer::GrabbedItemDrawer()
	: m_indivTextureSize{ IconDatabase::get().textureAtlas.getIndivTextureSize() },
	m_textureAtlasSize{ IconDatabase::get().textureAtlas.getAtlasSize() }
{
	m_atlasTexture.loadFromImage(IconDatabase::get().textureAtlas.getAtlasImage());
	m_grabbedItemSprite.setTexture(m_atlasTexture);

	m_font.loadFromFile("Res/Fonts/MinecraftRegular.otf");
	m_numberOfItemsText.setFont(m_font);
	m_numberOfItemsText.setOutlineThickness(1);
}

void GrabbedItemDrawer::setSizes(float invSlotSize)
{
	m_invSlotSize = invSlotSize;

	float scale = invSlotSize / m_indivTextureSize;
	m_grabbedItemSprite.setScale(scale, scale);
	m_grabbedItemSprite.setOrigin(invSlotSize / 2 / scale, invSlotSize / 2 / scale);

	m_numberOfItemsText.setCharacterSize(invSlotSize / 1.5f);
}

void GrabbedItemDrawer::updateSprite(ItemSlot & grabbedItem)
{
	if (grabbedItem.item.getMaterial().id == Material::Nothing)
		return;
	if (grabbedItem.item.getMaterial().id != m_item.getMaterial().id) {
		m_item = ItemStack(grabbedItem.item.getMaterial(), 0);

		ChunkBlock block(grabbedItem.item.getMaterial().toBlockID());
		auto textureCoords = block.getData().texSideCoord;
		//return { xMax, yMax, xMin, yMax, xMin, yMin, xMax, yMin };
		auto texCoords = IconDatabase::get().textureAtlas.getTextureCoords(textureCoords);

		m_grabbedItemSprite.setTextureRect(sf::IntRect(
			texCoords[0] * m_textureAtlasSize,
			texCoords[7] * m_textureAtlasSize,
			-m_indivTextureSize,
			m_indivTextureSize
		));
	}

	m_grabbedItemSprite.setPosition(
		(float)grabbedItem.position.x,
		(float)grabbedItem.position.y);


	if (m_numberOfItems != grabbedItem.item.getNumInStack()) {
		m_numberOfItems = grabbedItem.item.getNumInStack();

		m_numberOfItemsText.setString(std::to_string(grabbedItem.item.getNumInStack()));

		m_numberOfItemsText.setOrigin(
			m_numberOfItemsText.getLocalBounds().width,
			m_numberOfItemsText.getLocalBounds().top);
	}

	m_numberOfItemsText.setPosition(
		(float)grabbedItem.position.x + m_invSlotSize * 0.5f,
		(float)grabbedItem.position.y + m_invSlotSize * 0.05f);
}

void GrabbedItemDrawer::draw(RenderMaster & master)
{
	master.drawSFMLOverInterface(m_grabbedItemSprite);
	master.drawSFMLOverInterface(m_numberOfItemsText);
}