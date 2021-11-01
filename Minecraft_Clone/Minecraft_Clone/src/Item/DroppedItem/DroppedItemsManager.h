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
	void addItem(const ItemStack& itemstack, const glm::vec3& pos);
	void addItem(const ItemStack& itemstack, const glm::vec3& pos, const glm::vec3& rotation);
	void blockBrokenUpdate(const glm::vec3& pos, World &world);

	std::list<DroppedItem>& getDroppedItems() { return m_items; }

	void update(Player &player, World &world, float dt);
	void addToRender(RenderMaster &renderer);
	void checkForDroppedItems(const glm::vec3 pos, World &world);
	void updateMesh(World *world = nullptr);
private:
	void checkItemsLifetime();
	void checkIfPlayerCanGrabItem(Player &player);
	void lookForSameItemsNearby(World &world);
	void itemsMove(World &world, float dt);

	std::list<DroppedItem> m_items;

	DroppedItemsMesh m_droppedItemsMesh;
};