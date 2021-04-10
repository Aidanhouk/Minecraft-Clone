#pragma once

#include "SFML/Graphics.hpp"

class IconsMesh;
class Inventory;
class ItemSlot;

class ItemIconsBuilder
{
public:
	ItemIconsBuilder(Inventory &inventory, IconsMesh &itemIconsMesh);

	void buildMesh();
private:
	void buildIcon(ItemSlot &slot);
	void buildToolbarIcon(ItemSlot &slot, sf::Vector2i &toolbarSlotPos);

	void buildDigit(int digit, sf::Vector2i rightBottomPos, float slotSize);

	Inventory *m_pInventory = nullptr;
	IconsMesh *m_pItemIconsMesh = nullptr;

	float m_invSlotSize;
	float m_toolbarSlotSize;
};