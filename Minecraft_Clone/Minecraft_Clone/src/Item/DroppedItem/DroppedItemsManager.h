#pragma once

#include <vector>

#include "DroppedItem.h"
#include "DroppedItemsMesh.h"

class RenderMaster;
class Player;
class World;

class DroppedItemsManager
{
public:
	void addItem(const ItemStack& itemstack, const glm::vec3& pos);
	void addItem(const ItemStack& itemstack, const glm::vec3& pos, const glm::vec3& rotation);
	void blockBrokenUpdate(const glm::vec3& pos);

	std::vector<DroppedItem>& getDroppedItems() { return m_items; }

	void update(Player &player, World &world, float dt);
	void addToRender(RenderMaster &renderer);
	void checkForDroppedItems(const glm::vec3 pos, World &world);
	void updateMesh();
private:
	void checkItemsLifetime();
	void checkIfPlayerCanGrabItem(Player &player);
	void itemsMove(World &world, float dt);

	std::vector<DroppedItem> m_items;

	DroppedItemsMesh m_droppedItemsMesh;
};