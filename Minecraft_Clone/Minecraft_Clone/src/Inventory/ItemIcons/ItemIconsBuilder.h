#pragma once

#include "Texture/TextureAtlas.h"

class IconsMesh;
class Inventory;
class ItemSlot;

class ItemIconsBuilder
{
public:
	ItemIconsBuilder(Inventory &inventory, IconsMesh &itemIconsMesh);

	void buildmesh();
private:
	void buildIcon(ItemSlot &slot);
	void buildToolbarIcon(ItemSlot &slot, sf::Vector2i &toolbarSlotPos);

	void buildDigit(int digit, sf::Vector2i rightBottomPos, float slotSize);

	Inventory *m_pInventory = nullptr;
	IconsMesh *m_pItemIconsMesh = nullptr;

	float m_invSlotSize;
	float m_toolbarSlotSize;
};