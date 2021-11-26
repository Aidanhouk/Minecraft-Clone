#include "../Inventory.h"

#include "Input/Mouse.h"

void Inventory::mouseInput(const sf::RenderWindow & window, Mouse &mouse)
{
	auto mouseCoords = sf::Mouse::getPosition(window);
	m_pPointedSlot = getPointedSlot(mouseCoords);

	if (!isGrabbedSlotEmpty()) {
		m_grabbedSlot.position = mouseCoords;
		updateGrabbedItemIcon();
		updateToolsDurBarGrabbedItem();

		if (mouse.toggle(sf::Mouse::Left)) {
			if (m_pPointedSlot) {
				if (m_interfaceType == InterfaceType::Inventory)
					leftMouseWithGrabbedItemInventoryUpdate(m_pPointedSlot);
				else
					leftMouseWithGrabbedItemCraftingTableUpdate(m_pPointedSlot);
			}
			// throw itemstack behind interface borders
			else if (
				mouseCoords.x < m_inventoryGUI.getGlobalBounds().left ||
				mouseCoords.x > m_inventoryGUI.getGlobalBounds().left + m_inventoryGUI.getSize().x ||
				mouseCoords.y < m_inventoryGUI.getGlobalBounds().top ||
				mouseCoords.y > m_inventoryGUI.getGlobalBounds().top + m_inventoryGUI.getSize().y)
			{
				throwItem(m_grabbedSlot.item.getNumInStack(), &m_grabbedSlot);
				m_grabbedSlot.item.clear();
				updateGrabbedItemIcon();
			}
		}

		else if (mouse.toggle(sf::Mouse::Right)) {
			if (m_pPointedSlot) {
				if (m_interfaceType == InterfaceType::Inventory)
					rightMouseWithGrabbedItemInventoryUpdate(m_pPointedSlot);
				else
					rightMouseWithGrabbedItemCraftingTableUpdate(m_pPointedSlot);
			}
			// throw 1 item behind interface borders
			else if (
				mouseCoords.x < m_inventoryGUI.getGlobalBounds().left ||
				mouseCoords.x > m_inventoryGUI.getGlobalBounds().left + m_inventoryGUI.getSize().x ||
				mouseCoords.y < m_inventoryGUI.getGlobalBounds().top ||
				mouseCoords.y > m_inventoryGUI.getGlobalBounds().top + m_inventoryGUI.getSize().y)
			{
				throwItem(1, &m_grabbedSlot);
				m_grabbedSlot.item.remove(1);
				if (m_grabbedSlot.item.getNumInStack() == 0)
					m_grabbedSlot.item.clear();
				updateGrabbedItemIcon();
			}
		}
	}
	// grab item
	else {

		if (mouse.toggle(sf::Mouse::Left)) {
			if (!m_pPointedSlot)
				return;
			if (isSlotEmpty(m_pPointedSlot))
				return;
			if (m_interfaceType == InterfaceType::Inventory)
				leftMouseNoGrabbedItemInventoryUpdate(mouseCoords, m_pPointedSlot);
			else
				leftMouseNoGrabbedItemCraftingTableUpdate(mouseCoords, m_pPointedSlot);
		}

		else if (mouse.toggle(sf::Mouse::Right)) {
			if (!m_pPointedSlot)
				return;
			if (isSlotEmpty(m_pPointedSlot))
				return;
			if (m_interfaceType == InterfaceType::Inventory)
				rightMouseNoGrabbedItemInventoryUpdate(mouseCoords, m_pPointedSlot);
			else
				rightMouseNoGrabbedItemCraftingTableUpdate(mouseCoords, m_pPointedSlot);
		}
		updateInventoryText(mouseCoords);

		shouldUpdateIcons();
		updateGrabbedItemIcon();
	}
}

ItemSlot* Inventory::getPointedSlot(sf::Vector2i & mousePos)
{
	//if (mousePos.x < m_inventory.getGlobalBounds().left ||
	//	mousePos.x > m_inventory.getGlobalBounds().left + m_inventory.getGlobalBounds().width ||
	//	mousePos.y < m_inventory.getGlobalBounds().top ||
	//	mousePos.y > m_inventory.getGlobalBounds().top + m_inventory.getGlobalBounds().height)
	//{
	//	return nullptr;
	//}
	if (mousePos.x < m_slots[0].position.x ||
		mousePos.x > m_slots[8].position.x + m_invSlotSize ||
		mousePos.y < m_armorSlots[0].position.y - m_invSlotSize ||
		mousePos.y > m_slots[0].position.y)
	{
		return nullptr;
	}

	if (mousePos.y > m_slots[0].position.y - m_invSlotSize)
		return &m_slots[getPointedSlotColumn(mousePos.x)];
	else if (mousePos.y < m_slots[9 * 3].position.y) {
		if (mousePos.y > m_slots[9 * 3].position.y - m_invSlotSize)
			return &m_slots[9 * 3 + getPointedSlotColumn(mousePos.x)];
		else if (mousePos.y > m_slots[9 * 2].position.y - m_invSlotSize)
			return &m_slots[9 * 2 + getPointedSlotColumn(mousePos.x)];
		else if (mousePos.y > m_slots[9 * 1].position.y - m_invSlotSize)
			return &m_slots[9 * 1 + getPointedSlotColumn(mousePos.x)];
	}

	if (m_interfaceType == InterfaceType::Inventory) {
		for (auto & slot : m_craftSlots)
			if (isMousePointedOnSlot(mousePos, &slot))
				return &slot;
		if (isMousePointedOnSlot(mousePos, &m_craftResultSlot))
			return &m_craftResultSlot;

		// For the future
		//for (auto & slot : m_armorSlots)
		//	if (isMousePointedOnSlot(mousePos, &slot))
		//		return &slot;
	}
	else {
		for (auto & slot : m_craftingTableSlots)
			if (isMousePointedOnSlot(mousePos, &slot))
				return &slot;
		if (isMousePointedOnSlot(mousePos, &m_craftingTableResultSlot))
			return &m_craftingTableResultSlot;
	}

	return nullptr;
}

bool Inventory::isMousePointedOnSlot(sf::Vector2i & mousePos, ItemSlot * itemSlot)
{
	if (mousePos.x > itemSlot->position.x &&
		mousePos.x < itemSlot->position.x + m_invSlotSize &&
		mousePos.y > itemSlot->position.y - m_invSlotSize &&
		mousePos.y < itemSlot->position.y)
	{
		return true;
	}
	else
		return false;
}

int Inventory::getPointedSlotColumn(int mousePosX)
{
	if (mousePosX >= m_slots[8].position.x)
		return 8;
	else if (mousePosX >= m_slots[7].position.x)
		return 7;
	else if (mousePosX >= m_slots[6].position.x)
		return 6;
	else if (mousePosX >= m_slots[5].position.x)
		return 5;
	else if (mousePosX >= m_slots[4].position.x)
		return 4;
	else if (mousePosX >= m_slots[3].position.x)
		return 3;
	else if (mousePosX >= m_slots[2].position.x)
		return 2;
	else if (mousePosX >= m_slots[1].position.x)
		return 1;
	else// if (mousePosX > m_slots[0].position.x)
		return 0;
}






void Inventory::leftMouseWithGrabbedItemInventoryUpdate(ItemSlot * pointedSlot)
{
	if (m_pPointedSlot != &m_craftResultSlot) {

		if (isSlotEmpty(m_pPointedSlot)) {
			std::swap(m_grabbedSlot.item, m_pPointedSlot->item);
		}
		else {
			if (doSlotsHaveSameItem(&m_grabbedSlot, m_pPointedSlot)) {
				tryAddItem(m_pPointedSlot, &m_grabbedSlot);
			}
			else {
				std::swap(m_grabbedSlot.item, m_pPointedSlot->item);
			}
		}

		for (auto & slot : m_craftSlots)
			if (&slot == m_pPointedSlot) {
				computeCrafting();
				break;
			}
	}
	// if pointed at craft result slot
	else {
		if (isGrabbedSlotEmpty()) {
			std::swap(m_grabbedSlot.item, m_pPointedSlot->item);
			removeCraftSpentItems();
		}
		else if (doSlotsHaveSameItem(&m_grabbedSlot, m_pPointedSlot)) {
			tryAddItem(&m_grabbedSlot, m_pPointedSlot);
		}
		computeCrafting();
	}
	shouldUpdateIcons();
}

void Inventory::rightMouseWithGrabbedItemInventoryUpdate(ItemSlot * pointedSlot)
{
	if (m_pPointedSlot != &m_craftResultSlot) {

		if (isSlotEmpty(m_pPointedSlot)) {
			m_pPointedSlot->item.setData(m_grabbedSlot.item.getBlockId(), 1);
			m_grabbedSlot.item.remove(1);
		}
		else {
			// if the same material
			if (doSlotsHaveSameItem(&m_grabbedSlot, m_pPointedSlot)) {
				if (m_pPointedSlot->item.getNumInStack() != m_pPointedSlot->item.getMaxStackSize()) {
					m_pPointedSlot->item.add(1);
					m_grabbedSlot.item.remove(1);
					if (m_grabbedSlot.item.getNumInStack() == 0)
						m_grabbedSlot.item.clear();
				}
			}
			else {
				std::swap(m_grabbedSlot.item, m_pPointedSlot->item);
			}
		}

		for (auto & slot : m_craftSlots)
			if (&slot == m_pPointedSlot) {
				computeCrafting();
				break;
			}
	}
	// if pointed at craft result slot
	else {
		if (isGrabbedSlotEmpty()) {
			std::swap(m_grabbedSlot.item, m_pPointedSlot->item);
			removeCraftSpentItems();
		}
		else if (doSlotsHaveSameItem(&m_grabbedSlot, m_pPointedSlot)) {
			tryAddItem(&m_grabbedSlot, m_pPointedSlot);
		}
		computeCrafting();
	}

	shouldUpdateIcons();
}

void Inventory::leftMouseWithGrabbedItemCraftingTableUpdate(ItemSlot * pointedSlot)
{
	if (m_pPointedSlot != &m_craftingTableResultSlot) {

		if (isSlotEmpty(m_pPointedSlot)) {
			std::swap(m_grabbedSlot.item, m_pPointedSlot->item);
		}
		else {
			if (doSlotsHaveSameItem(&m_grabbedSlot, m_pPointedSlot)) {
				tryAddItem(m_pPointedSlot, &m_grabbedSlot);
			}
			else {
				std::swap(m_grabbedSlot.item, m_pPointedSlot->item);
			}
		}

		for (auto & slot : m_craftingTableSlots)
			if (&slot == m_pPointedSlot) {
				computeCrafting();
				break;
			}
	}
	// if pointed at craft result slot
	else {
		if (isGrabbedSlotEmpty()) {
			std::swap(m_grabbedSlot.item, m_pPointedSlot->item);
			removeCraftingTableSpentItems();
		}
		else if (doSlotsHaveSameItem(&m_grabbedSlot, m_pPointedSlot)) {
			tryAddItem(&m_grabbedSlot, m_pPointedSlot);
		}
		computeCrafting();
	}
	shouldUpdateIcons();
}

void Inventory::rightMouseWithGrabbedItemCraftingTableUpdate(ItemSlot * pointedSlot)
{
	if (m_pPointedSlot != &m_craftingTableResultSlot) {

		if (isSlotEmpty(m_pPointedSlot)) {
			m_pPointedSlot->item.setData(m_grabbedSlot.item.getBlockId(), 1);
			m_grabbedSlot.item.remove(1);
		}
		else {
			// if the same material
			if (doSlotsHaveSameItem(&m_grabbedSlot, m_pPointedSlot)) {
				if (m_pPointedSlot->item.getNumInStack() != m_pPointedSlot->item.getMaxStackSize()) {
					m_pPointedSlot->item.add(1);
					m_grabbedSlot.item.remove(1);
					if (m_grabbedSlot.item.getNumInStack() == 0)
						m_grabbedSlot.item.clear();
				}
			}
			else {
				std::swap(m_grabbedSlot.item, m_pPointedSlot->item);
			}
		}

		for (auto & slot : m_craftingTableSlots)
			if (&slot == m_pPointedSlot) {
				computeCrafting();
				break;
			}
	}
	// if pointed at craft result slot
	else {
		if (isGrabbedSlotEmpty()) {
			std::swap(m_grabbedSlot.item, m_pPointedSlot->item);
			removeCraftingTableSpentItems();
		}
		else if (doSlotsHaveSameItem(&m_grabbedSlot, m_pPointedSlot)) {
			tryAddItem(&m_grabbedSlot, m_pPointedSlot);
		}
		computeCrafting();
	}

	shouldUpdateIcons();
}

void Inventory::leftMouseNoGrabbedItemInventoryUpdate(sf::Vector2i & mouseCoords, ItemSlot * pointedSlot)
{
	if (m_pPointedSlot != &m_craftResultSlot) {
		std::swap(m_grabbedSlot.item, m_pPointedSlot->item);
		m_grabbedSlot.position = mouseCoords;

		for (auto & slot : m_craftSlots)
			if (&slot == m_pPointedSlot) {
				computeCrafting();
				break;
			}
	}
	else {
		std::swap(m_grabbedSlot.item, m_pPointedSlot->item);
		removeCraftSpentItems();

		m_grabbedSlot.position = mouseCoords;
		computeCrafting();
	}
}

void Inventory::rightMouseNoGrabbedItemInventoryUpdate(sf::Vector2i & mouseCoords, ItemSlot * pointedSlot)
{
	if (m_pPointedSlot != &m_craftResultSlot) {
		int leftStack = m_pPointedSlot->item.getNumInStack() / 2;
		int grabbedStack = m_pPointedSlot->item.getNumInStack() - leftStack;

		m_grabbedSlot.item.setData(m_pPointedSlot->item.getBlockId(), grabbedStack);
		m_grabbedSlot.position = mouseCoords;

		m_pPointedSlot->item.remove(grabbedStack);
		if (m_pPointedSlot->item.getNumInStack() == 0)
			m_pPointedSlot->item.clear();

		for (auto & slot : m_craftSlots)
			if (&slot == m_pPointedSlot) {
				computeCrafting();
				break;
			}
	}
	else {
		std::swap(m_grabbedSlot.item, m_pPointedSlot->item);
		removeCraftSpentItems();

		m_grabbedSlot.position = mouseCoords;
		computeCrafting();
	}
}

void Inventory::leftMouseNoGrabbedItemCraftingTableUpdate(sf::Vector2i & mouseCoords, ItemSlot * pointedSlot)
{
	if (m_pPointedSlot != &m_craftingTableResultSlot) {
		std::swap(m_grabbedSlot.item, m_pPointedSlot->item);
		m_grabbedSlot.position = mouseCoords;

		for (auto & slot : m_craftingTableSlots)
			if (&slot == m_pPointedSlot) {
				computeCrafting();
				break;
			}
	}
	else {
		std::swap(m_grabbedSlot.item, m_pPointedSlot->item);
		removeCraftingTableSpentItems();

		m_grabbedSlot.position = mouseCoords;
		computeCrafting();
	}
}

void Inventory::rightMouseNoGrabbedItemCraftingTableUpdate(sf::Vector2i & mouseCoords, ItemSlot * pointedSlot)
{
	if (m_pPointedSlot != &m_craftingTableResultSlot) {
		int leftStack = m_pPointedSlot->item.getNumInStack() / 2;
		int grabbedStack = m_pPointedSlot->item.getNumInStack() - leftStack;

		m_grabbedSlot.item.setData(m_pPointedSlot->item.getBlockId(), grabbedStack);
		m_grabbedSlot.position = mouseCoords;

		m_pPointedSlot->item.remove(grabbedStack);
		if (m_pPointedSlot->item.getNumInStack() == 0)
			m_pPointedSlot->item.clear();

		for (auto & slot : m_craftingTableSlots)
			if (&slot == m_pPointedSlot) {
				computeCrafting();
				break;
			}
	}
	else {
		std::swap(m_grabbedSlot.item, m_pPointedSlot->item);
		removeCraftingTableSpentItems();

		m_grabbedSlot.position = mouseCoords;
		computeCrafting();
	}
}
