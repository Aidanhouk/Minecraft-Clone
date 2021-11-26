#include "Crafting.h"

#include "CraftRecipes.h"

bool checkRecipe4Match(const std::array<ItemSlot, 4>& slots, const std::array<CraftingElement, 4>& recipe);
bool checkRecipe9Match(const std::array<ItemSlot, 9>& slots, const std::array<CraftingElement, 9>& recipe);

void computeCrafting4(std::array<ItemSlot, 4>& slots, ItemSlot & result)
{
	for (const auto & recipe : Recipe4::ALL_RESIPES) {
		if (checkRecipe4Match(slots, recipe->input)) {
			result.item = recipe->output;
			return;
		}
	}

	result.item.clear();
}

void computeCrafting9(std::array<ItemSlot, 9>& slots, ItemSlot & result)
{
	for (const auto & recipe : Recipe9::ALL_RESIPES) {
		if (checkRecipe9Match(slots, recipe->input)) {
			result.item = recipe->output;
			return;
		}
	}

	std::array<ItemSlot, 4> slots4;

	if (
		slots[2].item.getBlockId() == EMPTY_SLOT_ID &&
		slots[5].item.getBlockId() == EMPTY_SLOT_ID &&
		slots[6].item.getBlockId() == EMPTY_SLOT_ID &&
		slots[7].item.getBlockId() == EMPTY_SLOT_ID &&
		slots[8].item.getBlockId() == EMPTY_SLOT_ID
		) {
		slots4[0] = slots[0];
		slots4[1] = slots[1];
		slots4[2] = slots[3];
		slots4[3] = slots[4];
		for (const auto & recipe : Recipe4::ALL_RESIPES) {
			if (checkRecipe4Match(slots4, recipe->input)) {
				result.item = recipe->output;
				return;
			}
		}
	}

	if (
		slots[0].item.getBlockId() == EMPTY_SLOT_ID &&
		slots[3].item.getBlockId() == EMPTY_SLOT_ID &&
		slots[6].item.getBlockId() == EMPTY_SLOT_ID &&
		slots[7].item.getBlockId() == EMPTY_SLOT_ID &&
		slots[8].item.getBlockId() == EMPTY_SLOT_ID
		) {
		slots4[0] = slots[1];
		slots4[1] = slots[2];
		slots4[2] = slots[4];
		slots4[3] = slots[5];
		for (const auto & recipe : Recipe4::ALL_RESIPES) {
			if (checkRecipe4Match(slots4, recipe->input)) {
				result.item = recipe->output;
				return;
			}
		}
	}

	if (
		slots[0].item.getBlockId() == EMPTY_SLOT_ID &&
		slots[1].item.getBlockId() == EMPTY_SLOT_ID &&
		slots[2].item.getBlockId() == EMPTY_SLOT_ID &&
		slots[5].item.getBlockId() == EMPTY_SLOT_ID &&
		slots[8].item.getBlockId() == EMPTY_SLOT_ID
		) {
		slots4[0] = slots[3];
		slots4[1] = slots[4];
		slots4[2] = slots[6];
		slots4[3] = slots[7];
		for (const auto & recipe : Recipe4::ALL_RESIPES) {
			if (checkRecipe4Match(slots4, recipe->input)) {
				result.item = recipe->output;
				return;
			}
		}
	}

	if (
		slots[0].item.getBlockId() == EMPTY_SLOT_ID &&
		slots[1].item.getBlockId() == EMPTY_SLOT_ID &&
		slots[2].item.getBlockId() == EMPTY_SLOT_ID &&
		slots[3].item.getBlockId() == EMPTY_SLOT_ID &&
		slots[6].item.getBlockId() == EMPTY_SLOT_ID
		) {
		slots4[0] = slots[4];
		slots4[1] = slots[5];
		slots4[2] = slots[7];
		slots4[3] = slots[8];
		for (const auto & recipe : Recipe4::ALL_RESIPES) {
			if (checkRecipe4Match(slots4, recipe->input)) {
				result.item = recipe->output;
				return;
			}
		}
	}

	result.item.clear();
}

bool checkRecipe4Match(const std::array<ItemSlot, 4>& slots, const std::array<CraftingElement, 4>& recipe)
{
	if (slots[0].item.getBlockId() == recipe[0] &&
		slots[1].item.getBlockId() == recipe[1] &&
		slots[2].item.getBlockId() == recipe[2] &&
		slots[3].item.getBlockId() == recipe[3])
	{
		return true;
	}
	else if (
		slots[1].item.getBlockId() == recipe[0] &&
		slots[3].item.getBlockId() == recipe[1] &&
		slots[2].item.getBlockId() == recipe[2] &&
		slots[0].item.getBlockId() == recipe[3])
	{
		return true;
	}
	else if (
		slots[3].item.getBlockId() == recipe[0] &&
		slots[2].item.getBlockId() == recipe[1] &&
		slots[0].item.getBlockId() == recipe[2] &&
		slots[1].item.getBlockId() == recipe[3])
	{
		return true;
	}
	else if (
		slots[2].item.getBlockId() == recipe[0] &&
		slots[0].item.getBlockId() == recipe[1] &&
		slots[1].item.getBlockId() == recipe[2] &&
		slots[3].item.getBlockId() == recipe[3])
	{
		return true;
	}
	else {
		return false;
	}
}

bool checkRecipe9Match(const std::array<ItemSlot, 9>& slots, const std::array<CraftingElement, 9>& recipe)
{
	if (slots[0].item.getBlockId() == recipe[0] &&
		slots[1].item.getBlockId() == recipe[1] &&
		slots[2].item.getBlockId() == recipe[2] &&
		slots[3].item.getBlockId() == recipe[3] &&
		slots[4].item.getBlockId() == recipe[4] &&
		slots[5].item.getBlockId() == recipe[5] &&
		slots[6].item.getBlockId() == recipe[6] &&
		slots[7].item.getBlockId() == recipe[7] &&
		slots[8].item.getBlockId() == recipe[8])
	{
		return true;
	}
	else {
		return false;
	}
}