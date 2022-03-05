#pragma once

#include "DroppedItem.h"
#include "DroppedItemsMesh.h"

#include "World/Chunk/Chunk.h"

#include <list>

class RenderMaster;
class Player;
class World;

class DroppedItemsManager
{
public:
	DroppedItemsManager(World * world);

	void addItem(const ItemStack& itemstack, const glm::vec3& pos);
	void addItem(const ItemStack& itemstack, const glm::vec3& pos, const glm::vec3& rotation);
	void blockBrokenUpdate(const glm::vec3& pos, World &world);

	std::list<DroppedItem>& getDroppedItems() { return m_items; }

	void update(Player &player, float dt);
	void addToRender(RenderMaster &renderer);
	void checkForDroppedItems(const glm::vec3 pos);
	void updateMesh();
private:
	void checkItemsLifetime();
	void checkIfPlayerCanGrabItem(Player &player);
	void lookForSameItemsNearby();
	void itemsMove(float dt);

	World * m_pWorld;

	std::list<DroppedItem> m_items;

	DroppedItemsMesh m_droppedItemsMesh;
};