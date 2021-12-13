#include "../Inventory.h"

#include "Crafting/Crafting.h"

ItemStack & Inventory::getHeldItems()
{
	return m_slots[m_heldItemIndex].item;
}

void Inventory::computeCrafting()
{
	if (m_interfaceType == InterfaceType::Inventory)
		computeCrafting4(m_craftSlots, m_craftResultSlot);
	else
		computeCrafting9(m_craftingTableSlots, m_craftingTableResultSlot);
}

void Inventory::shouldUpdateIcons()
{
	m_updateIcons = true;
}

bool Inventory::isSlotEmpty(ItemSlot * slot)
{
	return slot->item.getBlockId() == EMPTY_SLOT_ID;
}

bool Inventory::isGrabbedSlotEmpty()
{
	return m_grabbedSlot.item.getBlockId() == EMPTY_SLOT_ID;
}

bool Inventory::doSlotsHaveSameItem(ItemSlot * slot1, ItemSlot * slot2)
{
	return slot1->item.getBlockId() == slot2->item.getBlockId();
}
