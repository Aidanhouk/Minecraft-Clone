#include "Inventory.h"

#include "../Renderer/RenderMaster.h"
#include "../Application.h"
#include "GlobalInfo.h"
#include "Player/PlayerInfo.h"
#include "ItemIcons/ItemIconsBuilder.h"
#include "PlayerParametersIcons/ParametersBuilder.h"
#include "Item/DroppedItem/DroppedItemsManager.h"
#include "Item/Material.h"
#include "Input/Keyboard.h"

#include <iostream>

Inventory::Inventory(Player &player, Application &app)
	: m_pPlayer{ &player },
	m_updateIcons{ true }
{
	//m_slots[7].item.setData(BlockId::DiamondSword, 1);
	m_slots[8].item.setData(BlockId::Apple, 5);

	m_inventoryTexture.loadFromFile("Res/Textures/Interface/inventoryInterface.png");
	m_inventory.setTexture(&m_inventoryTexture);
	m_toolbarTexture.loadFromFile("Res/Textures/Interface/toolbar.png");
	m_toolbar.setTexture(&m_toolbarTexture);
	m_heldItemFrameTexture.loadFromFile("Res/Textures/Interface/toolbarHeldItem.png");
	m_heldItemFrame.setTexture(&m_heldItemFrameTexture);

	setInterfacePositions(app);

	setSlotsPositions();

	m_font.loadFromFile("Res/Fonts/MinecraftRegular.otf");
	m_itemTextBackgroundTexture.loadFromFile("Res/Textures/Interface/textBackground.png");
	m_itemTextBackground.setTexture(&m_itemTextBackgroundTexture);
	setTextSettings(app);

	m_grabbedItemDrawer.setSizes(m_invSlotSize);
}

void Inventory::setInterfacePositions(const Application &app)
{
	float inventoryWidth = app.getWindow().getSize().x / 3.6f;
	float aspectRatio = (float)m_inventoryTexture.getSize().y / (float)m_inventoryTexture.getSize().x;
	// inventory height = 165,75px, width = 176,75px
	m_invPixelSize = inventoryWidth / 176.75f;
	m_invSlotSize = m_invPixelSize * 16;
	m_invDistanceBetweenSlots = m_invPixelSize * 18;

	m_inventory.setSize({ inventoryWidth, inventoryWidth * aspectRatio });
	m_inventory.setOrigin(
		m_inventory.getGlobalBounds().width / 2,
		m_inventory.getGlobalBounds().height / 2);
	m_inventory.setPosition(
		app.getWindow().getSize().x / 2,
		app.getWindow().getSize().y / 2);

	aspectRatio = (float)m_toolbarTexture.getSize().y / (float)m_toolbarTexture.getSize().x;
	m_toolbar.setSize({ inventoryWidth, inventoryWidth * aspectRatio });
	m_toolbar.setOrigin(
		m_toolbar.getGlobalBounds().width / 2,
		m_toolbar.getGlobalBounds().height);
	m_toolbar.setPosition(
		app.getWindow().getSize().x / 2,
		app.getWindow().getSize().y);

	aspectRatio = (float)m_heldItemFrameTexture.getSize().y / (float)m_heldItemFrameTexture.getSize().x;
	m_toolSlotSize = m_invSlotSize;
	m_toolDistanceBetweenSlots = inventoryWidth / 9;
	m_heldItemFrame.setSize({
		m_toolSlotSize * 1.4f,
		m_toolSlotSize * 1.4f * aspectRatio });
	m_heldItemFrame.setOrigin(
		m_heldItemFrame.getLocalBounds().width * 0.136f,
		m_heldItemFrame.getGlobalBounds().height);
	m_heldItemFrame.setPosition(
		m_toolbar.getGlobalBounds().left + m_toolDistanceBetweenSlots * 0.073f,
		app.getWindow().getSize().y);
}

void Inventory::setSlotsPositions()
{
	int invStartPosX = m_inventory.getGlobalBounds().left;
	int invStartPosY = m_inventory.getGlobalBounds().top + m_inventory.getGlobalBounds().height;
	setInventorySlotsPositions(sf::Vector2i(invStartPosX, invStartPosY));

	int toolStartPosX = m_toolbar.getGlobalBounds().left;
	int toolStartPosY = m_toolbar.getGlobalBounds().top + m_toolbar.getGlobalBounds().height;
	setToolbarSlotsPositions(sf::Vector2i(toolStartPosX, toolStartPosY));
}

void Inventory::setInventorySlotsPositions(const sf::Vector2i &inventoryStartPos)
{
	sf::Vector2i startSlotPos = {
		inventoryStartPos.x + (int)(8 * m_invPixelSize),
		inventoryStartPos.y - (int)(8 * m_invPixelSize) };

	// toolbar slots in inventory
	for (int j = 0; j < 9; ++j) {
		m_slots[j].position.x = startSlotPos.x + j * m_invDistanceBetweenSlots;
		m_slots[j].position.y = startSlotPos.y;
	}

	startSlotPos.y -= 4 * m_invPixelSize + 3 * m_invDistanceBetweenSlots;

	// rest of the slots
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 9; ++j) {
			m_slots[(i + 1) * 9 + j].position.x = startSlotPos.x + j * m_invDistanceBetweenSlots;
			m_slots[(i + 1) * 9 + j].position.y = startSlotPos.y + i * m_invDistanceBetweenSlots;
		}
	}
}

void Inventory::setToolbarSlotsPositions(const sf::Vector2i &toolbarStartPos)
{
	sf::Vector2i startSlotPos = {
		toolbarStartPos.x + (int)(m_toolDistanceBetweenSlots * 0.1f),
		toolbarStartPos.y - (int)(m_toolDistanceBetweenSlots * 0.081f) };

	for (int i = 0; i < 9; ++i) {
		m_toolbarItemPos[i] = sf::Vector2i(
			startSlotPos.x + i * m_toolDistanceBetweenSlots,
			startSlotPos.y);
	}
}

void Inventory::setTextSettings(const Application &app)
{
	m_inventoryItemText.setFont(m_font);
	m_inventoryItemText.setCharacterSize(40 * app.getWindow().getSize().x / 2560.0f);

	m_itemTextBackground.setSize(sf::Vector2f(0.0f, 60.0f * app.getWindow().getSize().x / 2560.0f));

	m_toolbarItemText.setFont(m_font);
	m_toolbarItemText.setCharacterSize(40 * app.getWindow().getSize().x / 2560.0f);
	m_toolbarItemText.setPosition(
		app.getWindow().getSize().x / 2,
		app.getWindow().getSize().y * 0.865f);

	updateToolbarText();
}



ItemStack & Inventory::getHeldItems()
{
	return m_slots[m_heldItemIndex].item;
}

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

void Inventory::updateHeldItemFrame()
{
	m_heldItemFrame.setPosition(

		m_toolbar.getGlobalBounds().left
		+ m_toolDistanceBetweenSlots * 0.063f
		+ m_heldItemIndex * m_toolDistanceBetweenSlots,

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

	auto & material = Material::toMaterial(m_pPointedSlot->item.getBlockId());
	m_inventoryItemText.setString(material.name);
	m_inventoryItemText.setPosition(
		mousePos.x + 15 * m_invPixelSize, mousePos.y - 22 * m_invPixelSize
	);


	m_itemTextBackground.setPosition(
		mousePos.x + 8 * m_invPixelSize, mousePos.y - 23 * m_invPixelSize
	);
	m_itemTextBackground.setSize(sf::Vector2f(
		55.0f * m_invPixelSize / 4.023f + m_inventoryItemText.getLocalBounds().width,
		m_itemTextBackground.getSize().y));
}

ItemSlot* Inventory::getPointedSlot(sf::Vector2i & mousePos)
{
	if (mousePos.x < m_slots[0].position.x ||
		mousePos.x > m_slots[8].position.x + m_invSlotSize ||
		mousePos.y > m_slots[0].position.y ||
		mousePos.y < m_slots[9].position.y - m_invSlotSize)
		return nullptr;
	if (mousePos.y < m_slots[18].position.y) { // 9-17 or 18-26

		if (mousePos.y < m_slots[9].position.y) { // 9-17
			int col = getPointedSlotColumn(mousePos.x);
			return &m_slots[9 + col];
		}
		else { // 18-26
			int col = getPointedSlotColumn(mousePos.x);
			return &m_slots[18 + col];
		}
	}
	else { // 0-8 or 27-35 or nullptr

		if (mousePos.y < m_slots[27].position.y) { // 27-35
			int col = getPointedSlotColumn(mousePos.x);
			return &m_slots[27 + col];
		}
		else {
			if (mousePos.y > m_slots[0].position.y - m_invSlotSize) { // 0-8
				int col = getPointedSlotColumn(mousePos.x);
				return &m_slots[col];
			}
			else { // space above the toolbar slots 
				return nullptr;
			}
		}
	}
}

int Inventory::getPointedSlotColumn(int mousePosX)
{
	if (mousePosX > m_slots[4].position.x) { // 4-8
		if (mousePosX > m_slots[6].position.x) { // 6-8
			if (mousePosX > m_slots[7].position.x) { // 7-8
				if (mousePosX > m_slots[8].position.x) {
					return 8;
				}
				else
					return 7;
			}
			else {
				return 6;
			}
		}
		else { // 4-5
			if (mousePosX > m_slots[5].position.x) { // 5
				return 5;
			}
			else {
				return 4;
			}
		}
	}
	else { // 0-3
		if (mousePosX > m_slots[2].position.x) { // 2-3
			if (mousePosX > m_slots[3].position.x) {
				return 3;
			}
			else
				return 2;
		}
		else { // 0-1
			if (mousePosX > m_slots[1].position.x) {
				return 1;
			}
			else
				return 0;
		}
	}
}

void Inventory::shouldUpdateIcons()
{
	m_updateIcons = true;
}

void Inventory::updateIcons()
{
	if (m_updateIcons) {
		m_iconsMesh.deleteData();
		ItemIconsBuilder(*this, m_iconsMesh).buildMesh();
		if (!m_isOpened)
			ParametersBuilder(*this, m_iconsMesh, *m_pPlayer).buildMesh();
		m_updateIcons = false;
	}
}

void Inventory::updateGrabbedItemIcon()
{
	m_grabbedItemDrawer.updateSprite(m_grabbedSlot);
}



bool Inventory::isSlotEmpty(ItemSlot * slot)
{
	return (slot->item.getBlockId() == EMPTY_SLOT_ID);
}

bool Inventory::isGrabbedSlotEmpty()
{
	return m_grabbedSlot.item.getBlockId() == EMPTY_SLOT_ID;
}

void Inventory::showOrHideInventory()
{
	m_isOpened = !m_isOpened;

	if (m_isOpened) {
		g_PlayerInfo.darkScreen = true;
		g_PlayerInfo.canMove = false;
		g_PlayerInfo.inventoryCursor = true;
		g_Window->setMouseCursorVisible(true);
		sf::Mouse::setPosition(sf::Vector2i(m_inventory.getPosition().x, m_inventory.getPosition().y));
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
	}

	shouldUpdateIcons();
}

void Inventory::mouseInput(const sf::RenderWindow & window, Mouse &mouse)
{
	auto mouseCoords = sf::Mouse::getPosition(window);
	m_pPointedSlot = getPointedSlot(mouseCoords);

	if (!isGrabbedSlotEmpty()) {
		m_grabbedSlot.position = mouseCoords;
		updateGrabbedItemIcon();

		if (mouse.toggle(sf::Mouse::Left)) {

			if (m_pPointedSlot) {

				if (isSlotEmpty(m_pPointedSlot)) {
					std::swap(m_grabbedSlot.item, m_pPointedSlot->item);
				}
				else {
					// if the same material
					if (m_pPointedSlot->item.getBlockId() == m_grabbedSlot.item.getBlockId()) {
						int sum = m_pPointedSlot->item.getNumInStack() + m_grabbedSlot.item.getNumInStack();
						if (sum > m_pPointedSlot->item.getMaxStackSize()) {
							int toAdd = m_pPointedSlot->item.getMaxStackSize() - m_pPointedSlot->item.getNumInStack();
							m_pPointedSlot->item.add(toAdd);
							m_grabbedSlot.item.remove(toAdd);
						}
						else {
							int toAdd = m_grabbedSlot.item.getNumInStack();
							m_pPointedSlot->item.add(toAdd);
							m_grabbedSlot.item.remove(toAdd);
							if (m_grabbedSlot.item.getNumInStack() == 0)
								m_grabbedSlot.item.clear();
						}
					}
					else {
						std::swap(m_grabbedSlot.item, m_pPointedSlot->item);
					}
				}

				shouldUpdateIcons();
			}
			// throw itemstack behind interface borders
			else if (mouseCoords.x < m_inventory.getGlobalBounds().left ||
				mouseCoords.x > m_inventory.getGlobalBounds().left + m_inventory.getSize().x ||
				mouseCoords.y < m_inventory.getGlobalBounds().top ||
				mouseCoords.y > m_inventory.getGlobalBounds().top + m_inventory.getSize().y)
			{
				throwItem(m_grabbedSlot.item.getNumInStack(), &m_grabbedSlot);
				m_grabbedSlot.item.clear();
				updateGrabbedItemIcon();
			}
		}



		else if (mouse.toggle(sf::Mouse::Right)) {

			if (m_pPointedSlot) {

				if (isSlotEmpty(m_pPointedSlot)) {
					m_pPointedSlot->item.setData(m_grabbedSlot.item.getBlockId(), 1);
					m_grabbedSlot.item.remove(1);
				}
				else {
					// if the same material
					if (m_pPointedSlot->item.getBlockId() == m_grabbedSlot.item.getBlockId()) {
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

				shouldUpdateIcons();
			}
			// throw 1 item behind interface borders
			else if (mouseCoords.x < m_inventory.getGlobalBounds().left ||
				mouseCoords.x > m_inventory.getGlobalBounds().left + m_inventory.getSize().x ||
				mouseCoords.y < m_inventory.getGlobalBounds().top ||
				mouseCoords.y > m_inventory.getGlobalBounds().top + m_inventory.getSize().y)
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
		if (!m_pPointedSlot)
			return;
		if (isSlotEmpty(m_pPointedSlot))
			return;
		updateInventoryText(mouseCoords);
		if (mouse.toggle(sf::Mouse::Left)) {
			std::swap(m_grabbedSlot.item, m_pPointedSlot->item);
			m_grabbedSlot.position = mouseCoords;
		}
		else if (mouse.toggle(sf::Mouse::Right)) {
			int leftStack = m_pPointedSlot->item.getNumInStack() / 2;
			int grabbedStack = m_pPointedSlot->item.getNumInStack() - leftStack;

			m_grabbedSlot.item.setData(m_pPointedSlot->item.getBlockId(), grabbedStack);
			m_grabbedSlot.position = mouseCoords;

			m_pPointedSlot->item.remove(grabbedStack);
			if (m_pPointedSlot->item.getNumInStack() == 0)
				m_pPointedSlot->item.clear();
		}
		shouldUpdateIcons();
		updateGrabbedItemIcon();
	}
}

void Inventory::draw(RenderMaster & master)
{
	if (m_isOpened) {
		master.drawSFML(m_inventory);
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