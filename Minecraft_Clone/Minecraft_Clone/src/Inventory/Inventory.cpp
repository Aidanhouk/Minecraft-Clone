#include "Inventory.h"

#include "../Renderer/RenderMaster.h"
#include "../Application.h"
#include "GlobalInfo.h"
#include "Player/PlayerInfo.h"
#include "ItemIcons/ItemIconsBuilder.h"
#include "PlayerParametersIcons/ParametersBuilder.h"
#include "Item/DroppedItem/DroppedItemsManager.h"

#include <iostream>

Inventory::Inventory(Player &player, Application &app)
	: m_pPlayer{ &player }
{
	m_inventoryTexture.loadFromFile("Res/Textures/Interface/inventory.png");
	m_inventory.setTexture(&m_inventoryTexture);
	m_toolbarTexture.loadFromFile("Res/Textures/Interface/toolbar.png");
	m_toolbar.setTexture(&m_toolbarTexture);
	m_heldItemTexture.loadFromFile("Res/Textures/Interface/toolbarHeldItem.png");
	m_heldItemFrame.setTexture(&m_heldItemTexture);

	setInterfacePositions(app);

	setSlotsPositions();

	m_font.loadFromFile("Res/Fonts/MinecraftRegular.otf");
	m_itemTextBackgroundTexture.loadFromFile("Res/Textures/Interface/textBackground.png");
	m_itemTextBackground.setTexture(&m_itemTextBackgroundTexture);
	setTextSettings(app);

	m_clickTimer.restart();

	m_grabbedItemDrawer.setSizes(m_invSlotSize);
	updateIcons();
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

	aspectRatio = (float)m_heldItemTexture.getSize().y / (float)m_heldItemTexture.getSize().x;
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
		app.getWindow().getSize().y * 0.895f);

	updateToolbarText();
}



ItemStack & Inventory::getHeldItems()
{
	return m_slots[m_heldItem].item;
}

void Inventory::addItems(const Material & material, int number, int callNumber)
{
	for (auto &itemSlot : m_slots) {
		if (material.id == itemSlot.item.getMaterial().id &&
			itemSlot.item.getNumInStack() < itemSlot.item.getMaterial().maxStackSize) {
			
			int leftOver = itemSlot.item.add(number);
			if (leftOver > 0)
				addItems(material, leftOver, callNumber + 1);
			else
				updateToolbarText();

			if (callNumber == 1)
				updateIcons();
			return;
		}
	}
	for (int i = 0; i < m_slots.size(); ++i) {
		if (isSlotEmpty(&m_slots[i])) {

			m_slots[i].item = { material, number };
			if (i == m_heldItem)
				updateToolbarText();

			if (callNumber == 1)
				updateIcons();
			return;
		}
	}
}

void Inventory::removeHeldItem(int number)
{
	if (isSlotEmpty(&m_slots[m_heldItem]))
		return;
	m_slots[m_heldItem].item.remove(number);
	updateIcons();
	updateToolbarText();
}

void Inventory::throwItem(int number, ItemSlot *thrownSlot)
{
	if (thrownSlot) {
		m_pDroppedItemsManager->addItem(thrownSlot->item,
			m_pPlayer->position, m_pPlayer->rotation);
	}
	else {
		if (isSlotEmpty(&m_slots[m_heldItem]))
			return;
		m_pDroppedItemsManager->addItem({ m_slots[m_heldItem].item.getMaterial(), 1 },
			m_pPlayer->position, m_pPlayer->rotation);
	}
}

void Inventory::setHeldItem(int heldItem)
{
	m_heldItem = heldItem;

	updateHeldItemFrame();
	updateToolbarText();
}

void Inventory::nextItem()
{
	if (m_heldItem == 8)
		m_heldItem = 0;
	else
		++m_heldItem;

	updateHeldItemFrame();
	updateToolbarText();
}

void Inventory::previousItem()
{
	if (m_heldItem == 0)
		m_heldItem = 8;
	else
		--m_heldItem;

	updateHeldItemFrame();
	updateToolbarText();
}

void Inventory::updateHeldItemFrame()
{
	m_heldItemFrame.setPosition(

		m_toolbar.getGlobalBounds().left
		+ m_toolDistanceBetweenSlots * 0.063f
		+ m_heldItem * m_toolDistanceBetweenSlots,

		m_heldItemFrame.getPosition().y);
}

void Inventory::updateToolbarText()
{
	m_toolbarItemText.setString(
		m_slots[m_heldItem].item.getMaterial().name
	);
	m_toolbarItemText.setOrigin(
		m_toolbarItemText.getGlobalBounds().width / 2,
		m_toolbarItemText.getGlobalBounds().height);
}

void Inventory::updateInventoryText(sf::Vector2i &mousePos)
{
	if (!m_pPointedSlot)
		return;

	m_inventoryItemText.setString(
		m_pPointedSlot->item.getMaterial().name
	);
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

void Inventory::updateIcons()
{
	m_iconsMesh.deleteData();
	ItemIconsBuilder(*this, m_iconsMesh).buildMesh();
	if (!m_isOpened)
		ParametersBuilder(*this, m_iconsMesh, *m_pPlayer).buildMesh();
}

void Inventory::updateGrabbedItemIcon()
{
	m_grabbedItemDrawer.updateSprite(m_grabbedSlot);
}



bool Inventory::isSlotEmpty(ItemSlot * slot)
{
	return (slot->item.getMaterial().id == Material::ID::Nothing);
}

void Inventory::showOrHideInventory()
{
	m_isOpened = !m_isOpened;

	if (m_isOpened) {
		p_info.darkScreen = true;
		p_info.canMove = false;
		p_info.interfaceCursor = true;
		g_window->setMouseCursorVisible(true);
		sf::Mouse::setPosition(sf::Vector2i(m_inventory.getPosition().x, m_inventory.getPosition().y));
	}
	else {
		p_info.darkScreen = false;
		p_info.canMove = true;
		p_info.interfaceCursor = false;
		g_window->setMouseCursorVisible(false);

		m_pPointedSlot = nullptr;
		if (!isSlotEmpty(&m_grabbedSlot)) {
			throwItem(m_grabbedSlot.item.getNumInStack(), &m_grabbedSlot);
			m_grabbedSlot = ItemSlot();
		}
	}

	updateIcons();
}

void Inventory::mouseInput(const sf::RenderWindow & window)
{
	auto mouseCoords = sf::Mouse::getPosition(window);
	m_pPointedSlot = getPointedSlot(mouseCoords);

	if (!isSlotEmpty(&m_grabbedSlot)) {
		m_grabbedSlot.position = mouseCoords;
		updateGrabbedItemIcon();

		if (!(sf::Mouse::isButtonPressed(sf::Mouse::Left)))
			return;
		if (m_clickTimer.getElapsedTime().asSeconds() <= 0.3f)
			return;

		if (m_pPointedSlot) {
			m_clickTimer.restart();

			if (isSlotEmpty(m_pPointedSlot)) {
				std::swap(m_grabbedSlot.item, m_pPointedSlot->item);
			}
			else {
				// if the same material
				if (m_pPointedSlot->item.getMaterial().id == m_grabbedSlot.item.getMaterial().id) {
					int sum = m_pPointedSlot->item.getNumInStack() + m_grabbedSlot.item.getNumInStack();
					int toAdd = m_pPointedSlot->item.getMaterial().maxStackSize - m_pPointedSlot->item.getNumInStack();
					if (sum > m_pPointedSlot->item.getMaterial().maxStackSize) {
						m_pPointedSlot->item.add(toAdd);
						m_grabbedSlot.item.remove(toAdd);
					}
					else {
						m_pPointedSlot->item.add(toAdd);
						m_grabbedSlot.item.remove(toAdd);
						if (m_grabbedSlot.item.getNumInStack() == 0)
							m_grabbedSlot = ItemSlot();
					}
				}
				else {
					std::swap(m_grabbedSlot.item, m_pPointedSlot->item);
				}
			}

			updateIcons();
		}
		// throw itemstack behind interface borders
		else if (mouseCoords.x < m_inventory.getGlobalBounds().left ||
			mouseCoords.x > m_inventory.getGlobalBounds().left + m_inventory.getSize().x ||
			mouseCoords.y < m_inventory.getGlobalBounds().top ||
			mouseCoords.y > m_inventory.getGlobalBounds().top + m_inventory.getSize().y)
		{
			m_clickTimer.restart();

			throwItem(m_grabbedSlot.item.getNumInStack(), &m_grabbedSlot);
			m_grabbedSlot = ItemSlot();
			updateGrabbedItemIcon();
		}
	}
	// grab item
	else {
		if (!m_pPointedSlot)
			return;
		if (isSlotEmpty(m_pPointedSlot))
			return;
		updateInventoryText(mouseCoords);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			if (m_clickTimer.getElapsedTime().asSeconds() > 0.3f) {
				m_clickTimer.restart();
				std::swap(m_grabbedSlot.item, m_pPointedSlot->item);
				m_grabbedSlot.position = mouseCoords;
				updateIcons();
				updateGrabbedItemIcon();
			}
	}
}

void Inventory::draw(RenderMaster & master)
{
	if (m_isOpened) {
		master.drawSFML(m_inventory);
		if (isSlotEmpty(&m_grabbedSlot)) {
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
		if (!isSlotEmpty(&m_slots[m_heldItem]))
			master.drawSFML(m_toolbarItemText);
	}

	master.drawIcons(m_iconsMesh);
	m_iconsMesh.bufferMesh();
}