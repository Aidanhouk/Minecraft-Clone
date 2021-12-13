#pragma once

#include "ItemSlot.h"
#include "IconsMesh.h"
#include "ItemIcons/GrabbedItemDrawer.h"

#include <array>

class Mouse;
class RenderMaster;
class Application;
class Player;
class DroppedItemsManager;

enum class InterfaceType
{
	Closed = 0,
	Inventory = 1,
	CraftingTable = 2,
};

class Inventory
{
public:
	Inventory(Player &player, Application &app);
	void setDroppedItemsManager(DroppedItemsManager* manager) { m_pDroppedItemsManager = manager; }

	ItemStack &getHeldItems();
	float getInvSlotSize() { return m_invSlotSize; }
	float getToolbarSlotSize() { return m_toolbarSlotSize; }
	bool isInventoryOpened() { return m_interfaceType != InterfaceType::Closed; }
	bool isInvertoryClosed() { return m_interfaceType == InterfaceType::Closed; }
	InterfaceType getInterfaceType() { return m_interfaceType; }

	int addItems(BlockId blockId, int number, int callNumber = 1);
	void removeHeldItem(int number);
	// if items were thrown from inventory using mouse
	void throwItem(int number, ItemSlot *thrownSlot);
	// if one item was thrown by pressing threw button (Q by default)
	void throwItem(int number);
	ItemSlot* getPointedItem() { return m_pPointedSlot; }
	ItemSlot* getGrabbedItem() { return &m_grabbedSlot; }
	void loseDurability(int loseDur);

	void setHeldItem(int heldItem);
	void nextItem();
	void previousItem();

	void shouldUpdateIcons();
	void updateIcons();

	void mouseInput(const sf::RenderWindow &window, Mouse &mouse);
	void showOrHideInventory(InterfaceType interfaceType);
	void draw(RenderMaster &master);
private:
	void setInterfacePositions(const Application &app);
	void setSlotsPositions();
	void setInventorySlotsPositions(const sf::Vector2i &inventoryStartPos);
	void setToolbarSlotsPositions(const sf::Vector2i &toolbarStartPos);
	void setArmorSlotsPositions(const sf::Vector2i &armorStartPos);
	void setCraftingSlotsPositions(const sf::Vector2i &craftingStartPos);
	void setCraftingTableSlotsPositions(const sf::Vector2i &craftingTableStartPos);
	void setTextSettings(const Application &app);
	void setToolDurabilityBarSettings();

	void updateHeldItemFrame();
	void updateToolbarText();
	void updateInventoryText(sf::Vector2i &mousePos);
	void updateGrabbedItemIcon();
	void updateToolsDurabilityBar();
	void updateToolsDurBarGrabbedItem();
	void createToolDurabilityBar(ItemSlot * itemSlot);

	void leftMouseWithGrabbedItemUpdate(sf::Vector2i &mousePos);
	void rightMouseWithGrabbedItemUpdate(sf::Vector2i &mousePos);
	void leftMouseNoGrabbedItemUpdate(sf::Vector2i &mousePos);
	void rightMouseNoGrabbedItemUpdate(sf::Vector2i &mousePos);

	ItemSlot* getPointedSlot(sf::Vector2i &mousePos);
	bool isMousePointedOnSlot(sf::Vector2i & mousePos, ItemSlot *itemSlot);
	int getPointedSlotColumn(int mousePosX);

	std::array<ItemSlot, 4 * 9> &getInvSlots() { return m_slots; }
	std::array<sf::Vector2i, 9> &getToolbarSlotsPos() { return m_toolbarItemPos; }
	std::array<ItemSlot, 4> &getArmorSlots() { return m_armorSlots; }
	std::array<ItemSlot, 4> &getCraftSlots() { return m_craftSlots; }
	ItemSlot &getCraftResultSlot() { return m_craftResultSlot; }
	std::array<ItemSlot, 9> &getCraftingTableSlots() { return m_craftingTableSlots; }
	ItemSlot &getCraftingTableResultSlot() { return m_craftingTableResultSlot; }

	bool isSlotEmpty(ItemSlot* slot);
	bool isGrabbedSlotEmpty();

	void tryAddItem(ItemSlot* slotToAddTo, ItemSlot* slotToTakeFrom);
	bool doSlotsHaveSameItem(ItemSlot* slot1, ItemSlot* slot2);
	void tryMovePointedItem(sf::Vector2i &mousePos);
	void craftMaxItems(ItemSlot* craftResultSlot);

	void checkForCraftingCompute();
	void computeCrafting();
	void removeCraftSpentItems(ItemSlot* craftResultSlot);



	Player * m_pPlayer;



	ItemSlot*					m_pPointedSlot = nullptr;

	ItemSlot					m_grabbedSlot;

	std::array<ItemSlot, 4>		m_armorSlots;

	std::array<ItemSlot, 4>		m_craftSlots;
	ItemSlot					m_craftResultSlot;
	std::array<ItemSlot, 9>		m_craftingTableSlots;
	ItemSlot					m_craftingTableResultSlot;

	std::array<ItemSlot, 4 * 9> m_slots;

	std::array<sf::Vector2i, 9> m_toolbarItemPos;
	int							m_heldItemIndex = 0;



	InterfaceType m_interfaceType = InterfaceType::Closed;

	bool m_updateIcons;

	sf::Font				m_font;
	// inventory objects
	sf::RectangleShape		m_inventoryGUI;
	sf::Texture				m_inventoryGUITexture;

	sf::RectangleShape		m_craftingTableGUI;
	sf::Texture				m_craftingTableGUITexture;

	sf::Text				m_inventoryItemText;
	sf::RectangleShape		m_itemTextBackground;
	sf::Texture				m_itemTextBackgroundTexture;
	float					m_backgroundHeightOneString;

	sf::RectangleShape		m_highlightSlotSquare;
	
	float					m_invPixelSize;
	float					m_invSlotSize;
	float					m_invDistanceBetweenSlots;
	// toolbar objects
	sf::RectangleShape		m_toolbar;
	sf::Texture				m_toolbarTexture;
	sf::Text				m_toolbarItemText;
	sf::RectangleShape		m_heldItemFrame;
	sf::Texture				m_heldItemFrameTexture;
	float					m_toolbarSlotSize;
	float					m_toolbarDistanceBetweenSlots;

	std::vector<sf::RectangleShape> m_toolMaxDurabilityBars;
	std::vector<sf::RectangleShape> m_toolDurabilityBars;
	sf::RectangleShape				m_toolMaxDurBarGrabbedItem;
	sf::RectangleShape				m_toolDurBarGrabbedItem;
	sf::Vector2f					m_toolDurBarOrigin;

	IconsMesh				m_iconsMesh;
	GrabbedItemDrawer		m_grabbedItemDrawer;
	DroppedItemsManager*	m_pDroppedItemsManager;

	friend class ItemIconsBuilder;
};