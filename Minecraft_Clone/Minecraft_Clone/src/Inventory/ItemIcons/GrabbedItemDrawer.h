#pragma once

#include <SFML/Graphics.hpp>
#include "Item/ItemStack.h"

class ItemSlot;
class RenderMaster;

class GrabbedItemDrawer
{
public:
	GrabbedItemDrawer();

	void setSizes(float invSlotSize);

	void updateSprite(ItemSlot &grabbedItem);

	void draw(RenderMaster & master);
private:
	sf::Texture		m_atlasTexture;
	sf::Sprite		m_grabbedItemSprite;
	sf::Font		m_font;
	sf::Text		m_numberOfItemsText;

	ItemStack		m_item;
	int				m_numberOfItems;

	int				m_indivTextureSize;
	int				m_textureAtlasSize;
	float			m_invSlotSize;
};