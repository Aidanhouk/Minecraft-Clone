#include "../Inventory.h"

#include "Player/PlayerInfo.h"

#include "Player/Player.h"
#include "Item/DroppedItem/DroppedItemsManager.h"
#include "Audio/SoundMaster.h"

#include <iostream>

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

void Inventory::checkForCraftingCompute()
{
	if (m_interfaceType == InterfaceType::Inventory) {
		for (auto & slot : m_craftSlots)
			if (&slot == m_pPointedSlot) {
				computeCrafting();
				break;
			}
	}
	else {
		for (auto & slot : m_craftingTableSlots)
			if (&slot == m_pPointedSlot) {
				computeCrafting();
				break;
			}
	}
}

void Inventory::tryAddItem(ItemSlot* slotToAddTo, ItemSlot* slotToTakeFrom)
{
	if (doSlotsHaveSameItem(slotToAddTo, slotToTakeFrom)) {
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
		}
	}
	else {
		if (!isSlotEmpty(slotToTakeFrom))
			std::swap(slotToAddTo->item, slotToTakeFrom->item);
	}
}

void Inventory::removeCraftSpentItems(ItemSlot* craftResultSlot)
{
	if (craftResultSlot == &m_craftResultSlot) {
		for (auto & slot : m_craftSlots)
			if (slot.item.getNumInStack() > 0)
				slot.item.remove(1);
	}
	else { // craftResultSlot == &m_craftingTableResultSlot
		for (auto & slot : m_craftingTableSlots)
			if (slot.item.getNumInStack() > 0)
				slot.item.remove(1);
	}
}

void Inventory::tryMovePointedItem(sf::Vector2i &mousePos)
{
	// move to inventory
	if (mousePos.y >= m_slots[0].position.y - m_invSlotSize ||
		mousePos.y < m_slots[9].position.y - m_invSlotSize)
	{
		for (int i = 9; i < 36; ++i) {
			if (m_pPointedSlot->item.getNumInStack() > 0) {
				if (doSlotsHaveSameItem(m_pPointedSlot, &m_slots[i])) {
					tryAddItem(&m_slots[i], m_pPointedSlot);
				}
			}
			else
				break;
		}

		if (m_pPointedSlot->item.getNumInStack() <= 0)
			return;

		for (int i = 9; i < 36; ++i) {
			if (isSlotEmpty(&m_slots[i])) {
				tryAddItem(&m_slots[i], m_pPointedSlot);
				break;
			}
		}
	}
	// move to toolbar
	else {
		for (int i = 0; i < 9; ++i) {
			if (m_pPointedSlot->item.getNumInStack() > 0) {
				if (doSlotsHaveSameItem(m_pPointedSlot, &m_slots[i])) {
					tryAddItem(&m_slots[i], m_pPointedSlot);
				}
			}
			else
				break;
		}

		if (m_pPointedSlot->item.getNumInStack() <= 0)
			return;

		for (int i = 0; i < 9; ++i) {
			if (isSlotEmpty(&m_slots[i])) {
				tryAddItem(&m_slots[i], m_pPointedSlot);
				break;
			}
		}
	}
}

void Inventory::craftMaxItems(ItemSlot* craftResultSlot)
{
	if (isSlotEmpty(craftResultSlot))
		return;

	if (doSlotsHaveSameItem(&m_grabbedSlot, craftResultSlot) || isGrabbedSlotEmpty()) {
		while (craftResultSlot->item.getNumInStack() + m_grabbedSlot.item.getNumInStack() <=
			craftResultSlot->item.getMaxStackSize())
		{
			tryAddItem(&m_grabbedSlot, craftResultSlot);
			removeCraftSpentItems(craftResultSlot);
	
			computeCrafting();
			if (isSlotEmpty(craftResultSlot))
				break;
		}
	}
}
