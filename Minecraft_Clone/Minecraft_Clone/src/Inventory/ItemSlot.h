#pragma once

#include "Item/ItemStack.h"
#include <SFML/Graphics.hpp>

enum class SlotId
{
	Toolbar_0,
	Toolbar_1,
	Toolbar_2,
	Toolbar_3,
	Toolbar_4,
	Toolbar_5,
	Toolbar_6,
	Toolbar_7,
	Toolbar_8,

	Inventory_9,
	Inventory_10,
	Inventory_11,
	Inventory_12,
	Inventory_13,
	Inventory_14,
	Inventory_15,
	Inventory_16,
	Inventory_17,

	Inventory_18,
	Inventory_19,
	Inventory_20,
	Inventory_21,
	Inventory_22,
	Inventory_23,
	Inventory_24,
	Inventory_25,
	Inventory_26,

	Inventory_27,
	Inventory_28,
	Inventory_29,
	Inventory_30,
	Inventory_31,
	Inventory_32,
	Inventory_33,
	Inventory_34,
	Inventory_35,

	Grabbed,

	Armor_0,
	Armor_1,
	Armor_2,
	Armor_3,

	Craft_0,
	Craft_1,
	Craft_2,
	Craft_3,
	Craft_4,
};

struct ItemSlot
{
	// left-bottom position
	sf::Vector2i position;
	ItemStack item;
	//SlotId slotId;
};