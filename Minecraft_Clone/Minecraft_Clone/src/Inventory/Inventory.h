#pragma once

#include <array>
#include <SFML/Graphics.hpp>

#include "Item/ItemStack.h"
#include "IconsMesh.h"
#include "ItemIcons/GrabbedItemDrawer.h"

class RenderMaster;
class Application;
class Player;
class DroppedItemsManager;

struct ItemSlot
{
	// left-bottom position
	sf::Vector2i position;
	ItemStack item;
};

class Inventory
{
public:
	Inventory(Player &player, Application &app);

	ItemStack &getHeldItems();
	std::array<ItemSlot, 4 * 9> &getInvSlots() { return m_slots; }
	std::array<sf::Vector2i, 9> &getToolbarSlotsPos() { return m_toolbarItemPos; }
	float getInvSlotSize() { return m_invSlotSize; }
	float getToolbarSlotSize() { return m_toolSlotSize; }
	bool isInventoryOpened() { return m_isOpened; }

	int addItems(const Material &material, int number, int callNumber = 1);
	void removeHeldItem(int number);
	// finish it after creating physics
	void throwItem(int number, ItemSlot *thrownSlot = nullptr);
	ItemSlot* getPointedItem() { return m_pPointedSlot; }
	ItemSlot* getGrabbedItem() { return &m_grabbedSlot; }
	void setDroppedItemsManager(DroppedItemsManager* manager) { m_pDroppedItemsManager = manager; }

	void setHeldItem(int heldItem);
	void nextItem();
	void previousItem();

	void updateIcons();

	void mouseInput(const sf::RenderWindow &window);
	void showOrHideInventory();
	void draw(RenderMaster &master);
private:
	void setInterfacePositions(const Application &app);
	void setSlotsPositions();
	void setInventorySlotsPositions(const sf::Vector2i &inventoryStartPos);
	void setToolbarSlotsPositions(const sf::Vector2i &toolbarStartPos);
	void setTextSettings(const Application &app);

	void updateHeldItemFrame();
	void updateToolbarText();
	void updateInventoryText(sf::Vector2i &mousePos);
	ItemSlot* getPointedSlot(sf::Vector2i &mousePos);
	int getPointedSlotColumn(int mousePosX);

	void updateGrabbedItemIcon();

	bool isSlotEmpty(ItemSlot* slot);



	Player * m_pPlayer;

	std::array<ItemSlot, 4 * 9> m_slots;
	std::array<sf::Vector2i, 9> m_toolbarItemPos;
	int m_heldItem = 0;

	bool m_isOpened = false;

	sf::Font				m_font;
	// inventory objects
	sf::RectangleShape		m_inventory;
	sf::Texture				m_inventoryTexture;
	sf::Text				m_inventoryItemText;
	sf::RectangleShape		m_itemTextBackground;
	sf::Texture				m_itemTextBackgroundTexture;
	// Kind of in-game pixel's size. Multiply by it when using pixels to be not dependent on resolution
	float					m_invPixelSize;
	float					m_invSlotSize;
	float					m_invDistanceBetweenSlots;
	// toolbar objects
	sf::RectangleShape		m_toolbar;
	sf::Texture				m_toolbarTexture;
	sf::Text				m_toolbarItemText;
	sf::RectangleShape		m_heldItemFrame;
	sf::Texture				m_heldItemTexture;
	float					m_toolSlotSize;
	float					m_toolDistanceBetweenSlots;

	sf::Clock				m_clickTimer;
	ItemSlot				m_grabbedSlot;
	ItemSlot*				m_pPointedSlot = nullptr;

	IconsMesh				m_iconsMesh;
	GrabbedItemDrawer		m_grabbedItemDrawer;
	DroppedItemsManager*	m_pDroppedItemsManager;
};