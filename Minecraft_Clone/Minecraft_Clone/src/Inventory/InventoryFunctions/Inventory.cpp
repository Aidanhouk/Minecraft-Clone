#include "../Inventory.h"

#include "Player/PlayerInfo.h"
#include "GlobalInfo.h"

#include "Renderer/RenderMaster.h"
#include "../ItemIcons/ItemIconsBuilder.h"
#include "../PlayerParametersIcons/ParametersBuilder.h"

#include <iostream>

void Inventory::updateIcons()
{
	if (m_updateIcons) {
		m_iconsMesh.deleteData();
		ItemIconsBuilder(*this, m_iconsMesh).buildMesh();
		if (isInvertoryClosed())
			ParametersBuilder(*this, m_iconsMesh, *m_pPlayer).buildMesh();
		updateToolsDurabilityBar();

		m_updateIcons = false;
	}
}

void Inventory::showOrHideInventory(InterfaceType interfaceType)
{
	if (isInventoryOpened())
		m_interfaceType = InterfaceType::Closed;
	else
		m_interfaceType = interfaceType;

	if (isInventoryOpened()) {
		g_PlayerInfo.darkScreen = true;
		g_PlayerInfo.canMove = false;
		g_PlayerInfo.inventoryCursor = true;
		g_Window->setMouseCursorVisible(true);
		sf::Mouse::setPosition(sf::Vector2i(m_inventoryGUI.getPosition().x, m_inventoryGUI.getPosition().y * 0.95f));
	}
	else {
		g_PlayerInfo.darkScreen = false;
		g_PlayerInfo.canMove = true;
		g_PlayerInfo.inventoryCursor = false;
		g_Window->setMouseCursorVisible(false);

		m_pPointedSlot = nullptr;
		if (!isGrabbedSlotEmpty()) {
			throwItem(m_grabbedSlot.item.getNumInStack(), &m_grabbedSlot);
			m_grabbedSlot.item.clear();
		}
		for (auto & slot : m_craftSlots) {
			if (!isSlotEmpty(&slot)) {
				throwItem(slot.item.getNumInStack(), &slot);
				slot.item.clear();
			}
		}
		for (auto & slot : m_craftingTableSlots) {
			if (!isSlotEmpty(&slot)) {
				throwItem(slot.item.getNumInStack(), &slot);
				slot.item.clear();
			}
		}
	}

	computeCrafting();
	shouldUpdateIcons();
}

void Inventory::draw(RenderMaster & master)
{
	for (auto & maxDurBar : m_toolMaxDurabilityBars)
		master.drawSFMLOverInterface(maxDurBar);
	for (auto & maxDurBar : m_toolDurabilityBars)
		master.drawSFMLOverInterface(maxDurBar);

	if (isInventoryOpened()) {
		if (m_interfaceType == InterfaceType::Inventory)
			master.drawSFML(m_inventoryGUI);
		else
			master.drawSFML(m_craftingTableGUI);

		if (m_grabbedSlot.item.getMaxToolDurability() > 0 &&
			m_grabbedSlot.item.getToolDurability() < m_grabbedSlot.item.getMaxToolDurability()) {

			master.drawSFMLOverInterface(m_toolMaxDurBarGrabbedItem);
			master.drawSFMLOverInterface(m_toolDurBarGrabbedItem);
		}

		if (isGrabbedSlotEmpty()) {
			if (m_pPointedSlot)
				if (!isSlotEmpty(m_pPointedSlot)) {
					master.drawSFMLOverInterface(m_itemTextBackground);
					master.drawSFMLOverInterface(m_inventoryItemText);
				}
		}
		else {
			m_grabbedItemDrawer.draw(master);
		}

		if (m_pPointedSlot)
			master.drawSFML(m_highlightSlotSquare);
	}
	else {
		master.drawSFML(m_toolbar);
		master.drawSFML(m_heldItemFrame);
		if (!isSlotEmpty(&m_slots[m_heldItemIndex]))
			master.drawSFML(m_toolbarItemText);
	}

	master.drawIcons(m_iconsMesh);
	m_iconsMesh.bufferMesh();
}