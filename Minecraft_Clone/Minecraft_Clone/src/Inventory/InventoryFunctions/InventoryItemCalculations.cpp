#include "../Inventory.h"

#include "Player/PlayerInfo.h"

#include "Player/Player.h"
#include "Item/DroppedItem/DroppedItemsManager.h"
#include "Audio/SoundMaster.h"

int Inventory::addItems(BlockId blockId, int number, int callNumber)
{
	for (auto &itemSlot : m_slots) {
		if (blockId == itemSlot.item.getBlockId() &&
			itemSlot.item.getNumInStack() < itemSlot.item.getMaxStackSize()) {

			int leftOver = itemSlot.item.add(number);
			if (leftOver > 0)
				addItems(blockId, leftOver, callNumber + 1);
			else
				updateToolbarText();

			if (callNumber == 1)
				shouldUpdateIcons();
			return 0;
		}
	}
	for (int i = 0; i < m_slots.size(); ++i) {
		if (isSlotEmpty(&m_slots[i])) {

			m_slots[i].item = { blockId, number };
			if (i == m_heldItemIndex)
				updateToolbarText();

			if (callNumber == 1)
				shouldUpdateIcons();
			return 0;
		}
	}
	return number;
}

void Inventory::removeHeldItem(int number)
{
	if (isSlotEmpty(&m_slots[m_heldItemIndex]))
		return;
	m_slots[m_heldItemIndex].item.remove(number);
	shouldUpdateIcons();
	updateToolbarText();
}

void Inventory::throwItem(int number, ItemSlot *thrownSlot)
{
	auto thrownItemStack = ItemStack(thrownSlot->item.getBlockId(), number);
	m_pDroppedItemsManager->addItem(std::move(thrownItemStack),
		m_pPlayer->position, m_pPlayer->rotation);
}

void Inventory::throwItem(int number)
{
	if (isSlotEmpty(&m_slots[m_heldItemIndex]))
		return;
	auto thrownItemStack = ItemStack(m_slots[m_heldItemIndex].item.getBlockId(), 1);
	m_pDroppedItemsManager->addItem(std::move(thrownItemStack),
		m_pPlayer->position, m_pPlayer->rotation);
}

void Inventory::loseDurability(int loseDur)
{
	auto & item = m_slots[m_heldItemIndex].item;

	if (item.getToolDurability() > 0) {
		if (item.loseDurability(loseDur)) {
			item.clear();
			
			g_SoundMaster.play(SoundId::ItemBreak);
		}

		shouldUpdateIcons();
	}
}

void Inventory::setHeldItem(int heldItem)
{
	m_heldItemIndex = heldItem;

	updateHeldItemFrame();
	updateToolbarText();
}

void Inventory::nextItem()
{
	if (m_heldItemIndex == 8)
		m_heldItemIndex = 0;
	else
		++m_heldItemIndex;

	updateHeldItemFrame();
	updateToolbarText();
}

void Inventory::previousItem()
{
	if (m_heldItemIndex == 0)
		m_heldItemIndex = 8;
	else
		--m_heldItemIndex;

	updateHeldItemFrame();
	updateToolbarText();
}

void Inventory::tryAddItem(ItemSlot* slotToAddTo, ItemSlot* slotToTakeFrom)
{
	int sum = slotToAddTo->item.getNumInStack() + slotToTakeFrom->item.getNumInStack();
	if (sum > slotToAddTo->item.getMaxStackSize()) {
		int toAdd = slotToAddTo->item.getMaxStackSize() - slotToAddTo->item.getNumInStack();
		slotToAddTo->item.add(toAdd);
		slotToTakeFrom->item.remove(toAdd);
	}
	else {
		int toAdd = slotToTakeFrom->item.getNumInStack();
		slotToAddTo->item.add(toAdd);
		slotToTakeFrom->item.remove(toAdd);

		if (slotToTakeFrom == &m_craftResultSlot)
			removeCraftSpentItems();
		else if (slotToTakeFrom == &m_craftingTableResultSlot)
			removeCraftingTableSpentItems();
	}
}

void Inventory::removeCraftSpentItems()
{
	for (auto & slot : m_craftSlots)
		if (slot.item.getNumInStack() > 0)
			slot.item.remove(1);
}

void Inventory::removeCraftingTableSpentItems()
{
	for (auto & slot : m_craftingTableSlots)
		if (slot.item.getNumInStack() > 0)
			slot.item.remove(1);
}
