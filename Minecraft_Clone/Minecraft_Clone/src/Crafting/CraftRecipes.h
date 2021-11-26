#pragma once

#include "../Util/NonCopyable.h"
#include "Item/ItemStack.h"

#include <array>
#include <vector>

struct CraftingElement
{
	BlockId id;

	friend bool operator== (const CraftingElement &element1, const BlockId &element2);
	friend bool operator== (const BlockId &element2, const CraftingElement &element1);
};

struct Recipe : public NonCopyable
{
	ItemStack output;

	Recipe(ItemStack output);
};

struct Recipe4 : public Recipe
{
	std::array<CraftingElement, 4> input;

	Recipe4(std::array<CraftingElement, 4>&& input, ItemStack output);

	const static std::vector<const Recipe4*> ALL_RESIPES;

	const static Recipe4
		OAK_PLANKS, BIRCH_PLANKS, PALM_PLANKS, SPRUCE_PLANKS,
		STICK, CRAFTING_TABLE
		;
};

struct Recipe9 : public Recipe
{
	std::array<CraftingElement, 9> input;

	Recipe9(std::array<CraftingElement, 9>&& input, ItemStack output);

	const static std::vector<const Recipe9*> ALL_RESIPES;

	const static Recipe9
		WOODEN_SWORD, WOODEN_PICKAXE, WOODEN_SHOVEL, WOODEN_AXE1, WOODEN_AXE2,
		STONE_SWORD, STONE_PICKAXE, STONE_SHOVEL, STONE_AXE1, STONE_AXE2,
		IRON_SWORD, IRON_PICKAXE, IRON_SHOVEL, IRON_AXE1, IRON_AXE2,
		GOLD_SWORD, GOLD_PICKAXE, GOLD_SHOVEL, GOLD_AXE1, GOLD_AXE2,
		DIAMOND_SWORD, DIAMOND_PICKAXE, DIAMOND_SHOVEL, DIAMOND_AXE1, DIAMOND_AXE2
		;
};