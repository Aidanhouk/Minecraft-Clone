#include "../Inventory.h"

#include "Item/Material.h"
#include "World/Block/ChunkBlock.h"
#include "World/Block/BlockData.h"
#include "World/Block/BlockDatabase.h"

#include <iostream>
#include <string>

void Inventory::updateHeldItemFrame()
{
	m_heldItemFrame.setPosition(

		m_toolbar.getGlobalBounds().left
		+ m_toolbarDistanceBetweenSlots * 0.063f
		+ m_heldItemIndex * m_toolbarDistanceBetweenSlots,

		m_heldItemFrame.getPosition().y);
}

void Inventory::updateToolbarText()
{
	auto & material = Material::toMaterial(m_slots[m_heldItemIndex].item.getBlockId());
	m_toolbarItemText.setString(material.name);
	m_toolbarItemText.setOrigin(
		m_toolbarItemText.getGlobalBounds().width / 2,
		m_toolbarItemText.getGlobalBounds().height);
}

void Inventory::updateInventoryText(sf::Vector2i &mousePos)
{
	if (!m_pPointedSlot)
		return;

	BlockId itemId = m_pPointedSlot->item.getBlockId();
	auto & material = Material::toMaterial(itemId);
	auto &blockData = ChunkBlock(itemId).getData();

	if (blockData.itemType == ItemType::Tool) {
		float efficiency = ChunkBlock(itemId).getData().effieciencyCoef;
		int efficiencyInt = (int)efficiency;
		int efficiencyDecimal = (efficiency - efficiencyInt) * 100;

		std::string str;
		if (BlockDatabase::get().isSword(itemId))
			str = "Attack Damage: ";
		else
			str = "Mining Speed: x";

		m_inventoryItemText.setString(material.name
			+ "\n\n" + str + std::to_string(efficiencyInt) + "." + std::to_string(efficiencyDecimal)
			+ "\nDurability: " + std::to_string(m_pPointedSlot->item.getToolDurability())
			+ " / " + std::to_string(m_pPointedSlot->item.getMaxToolDurability())
		);
		m_inventoryItemText.setPosition(
			//mousePos.x + 15 * m_invPixelSize, mousePos.y - 19.0f * m_invPixelSize // for 3 strings of text
			mousePos.x + 15 * m_invPixelSize, mousePos.y - 17.5f * m_invPixelSize
		);

		m_itemTextBackground.setPosition(
			mousePos.x + 8 * m_invPixelSize, mousePos.y - 23 * m_invPixelSize
		);
		m_itemTextBackground.setSize(sf::Vector2f(
			55.0f * m_invPixelSize / 4.023f + m_inventoryItemText.getLocalBounds().width,
			4 * m_backgroundHeightOneString));
	}
	else {
		m_inventoryItemText.setString(material.name);
		m_inventoryItemText.setPosition(
			mousePos.x + 15 * m_invPixelSize, mousePos.y - 22 * m_invPixelSize
		);

		m_itemTextBackground.setPosition(
			mousePos.x + 8 * m_invPixelSize, mousePos.y - 23 * m_invPixelSize
		);
		m_itemTextBackground.setSize(sf::Vector2f(
			55.0f * m_invPixelSize / 4.023f + m_inventoryItemText.getLocalBounds().width,
			m_backgroundHeightOneString));
	}
}

void Inventory::updateGrabbedItemIcon()
{
	m_grabbedItemDrawer.updateSprite(m_grabbedSlot);
}

void Inventory::updateToolsDurabilityBar()
{
	m_toolMaxDurabilityBars.clear();
	m_toolDurabilityBars.clear();

	if (m_interfaceType == InterfaceType::Closed) {
		for (int i = 0; i < 9; ++i) {
			if (m_slots[i].item.getToolDurability() > 0 &&
				m_slots[i].item.getToolDurability() < m_slots[i].item.getMaxToolDurability()) {
				sf::RectangleShape toolMaxDurabilityBar;
				toolMaxDurabilityBar.setFillColor(sf::Color::Black);
				toolMaxDurabilityBar.setSize({ m_invSlotSize * 0.8f, m_invSlotSize / 8.0f });
				toolMaxDurabilityBar.setOrigin(m_toolDurBarOrigin);
				toolMaxDurabilityBar.setPosition(m_toolbarItemPos[i].x, m_toolbarItemPos[i].y);

				m_toolMaxDurabilityBars.emplace_back(std::move(toolMaxDurabilityBar));

				int dur = m_slots[i].item.getToolDurability();
				float ratio = (float)dur / m_slots[i].item.getMaxToolDurability();

				sf::RectangleShape toolDurabilityBar;
				sf::Color color(255 - ratio * 255, ratio * 255, 0, 255);
				toolDurabilityBar.setFillColor(color);
				toolDurabilityBar.setSize({ m_invSlotSize * 0.8f * ratio, m_invSlotSize / 8.0f * 0.5f });
				toolDurabilityBar.setOrigin(m_toolDurBarOrigin);
				toolDurabilityBar.setPosition(m_toolbarItemPos[i].x, m_toolbarItemPos[i].y);

				m_toolDurabilityBars.emplace_back(std::move(toolDurabilityBar));
			}
		}
	}
	else if (m_interfaceType == InterfaceType::Inventory) {
		for (int i = 0; i < 36; ++i)
			createToolDurabilityBar(&m_slots[i]);
		// For later
		//for (int i = 0; i < 4; ++i)
		//	createToolDurabilityBar(&m_armorSlots[i]);
		for (int i = 0; i < 4; ++i)
			createToolDurabilityBar(&m_craftSlots[i]);
		createToolDurabilityBar(&m_craftResultSlot);
	}
	else {
		for (int i = 0; i < 36; ++i)
			createToolDurabilityBar(&m_slots[i]);
		for (int i = 0; i < 9; ++i)
			createToolDurabilityBar(&m_craftingTableSlots[i]);
		createToolDurabilityBar(&m_craftingTableResultSlot);
	}
}

void Inventory::updateToolsDurBarGrabbedItem()
{
	if (m_grabbedSlot.item.getToolDurability() > 0 &&
		m_grabbedSlot.item.getToolDurability() < m_grabbedSlot.item.getMaxToolDurability()) {

		m_toolMaxDurBarGrabbedItem.setSize({ m_invSlotSize * 0.8f, m_invSlotSize / 8.0f });
		m_toolMaxDurBarGrabbedItem.setPosition(m_grabbedSlot.position.x, m_grabbedSlot.position.y);

		float ratio = (float)m_grabbedSlot.item.getToolDurability() / m_grabbedSlot.item.getMaxToolDurability();

		sf::Color color(255 - ratio * 255, ratio * 255, 0, 255);
		m_toolDurBarGrabbedItem.setFillColor(color);
		m_toolDurBarGrabbedItem.setSize({ m_invSlotSize * 0.8f * ratio, m_invSlotSize / 8.0f * 0.5f });
		m_toolDurBarGrabbedItem.setPosition(m_grabbedSlot.position.x, m_grabbedSlot.position.y);
	}
}

void Inventory::createToolDurabilityBar(ItemSlot * itemSlot)
{
	if (itemSlot->item.getToolDurability() > 0 &&
		itemSlot->item.getToolDurability() < itemSlot->item.getMaxToolDurability()) {
		sf::RectangleShape toolMaxDurabilityBar;
		toolMaxDurabilityBar.setFillColor(sf::Color::Black);
		toolMaxDurabilityBar.setSize({ m_invSlotSize * 0.8f, m_invSlotSize / 8.0f });
		toolMaxDurabilityBar.setOrigin(m_toolDurBarOrigin);
		toolMaxDurabilityBar.setPosition(itemSlot->position.x, itemSlot->position.y);

		m_toolMaxDurabilityBars.emplace_back(std::move(toolMaxDurabilityBar));

		int dur = itemSlot->item.getToolDurability();
		float ratio = (float)dur / itemSlot->item.getMaxToolDurability();

		sf::RectangleShape toolDurabilityBar;
		sf::Color color(255 - ratio * 255, ratio * 255, 0, 255);
		toolDurabilityBar.setFillColor(color);
		toolDurabilityBar.setSize({ m_invSlotSize * 0.8f * ratio, m_invSlotSize / 8.0f * 0.5f });
		toolDurabilityBar.setOrigin(m_toolDurBarOrigin);
		toolDurabilityBar.setPosition(itemSlot->position.x, itemSlot->position.y);

		m_toolDurabilityBars.emplace_back(std::move(toolDurabilityBar));
	}
}