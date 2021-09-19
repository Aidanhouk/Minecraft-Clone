#include "DroppedItemsManager.h"

#include "Renderer/RenderMaster.h"
#include "Camera.h"
#include "DroppedItemsBuilder.h"

#include <iostream>

void DroppedItemsManager::addItem(const ItemStack & itemstack, const glm::vec3 & pos)
{
	m_items.emplace_back(itemstack, pos);
	updateMesh();
	m_items.back().setRandomAcceleration();
}

void DroppedItemsManager::addItem(const ItemStack & itemstack, const glm::vec3 & pos, const glm::vec3& rotation)
{
	m_items.emplace_back(itemstack, glm::vec3(pos.x, pos.y + 0.6f, pos.z));
	updateMesh();
	m_items.back().setAcceleration(rotation.x, rotation.y);
}

void DroppedItemsManager::blockBrokenUpdate(const glm::vec3 & pos, World &world)
{
	for (auto & item : m_items) {
		item.startFalling(world);
		//if (floor(item.position.x) == pos.x &&
		//	floor(item.position.z) == pos.z &&
		//	floor(item.position.y) - 1 == pos.y) {
		//	item.startMoving();
		//}
	}
}

void DroppedItemsManager::update(Player &player, World &world, float dt)
{
	//checkItemsLifetime();
	checkIfPlayerCanGrabItem(player);
	itemsMove(world, dt);
}

void DroppedItemsManager::addToRender(RenderMaster & renderer)
{
	renderer.drawDroppedItems(m_droppedItemsMesh);
	m_droppedItemsMesh.bufferMesh();
}

void DroppedItemsManager::checkForDroppedItems(const glm::vec3 pos, World &world)
{
	for (auto &item : m_items) {
		if (
			item.position.x > pos.x
			&& item.position.x < pos.x + 1
			&& item.position.z > pos.z
			&& item.position.z < pos.z + 1
			&& item.position.y > pos.y
			&& item.position.y < pos.y + 1
			)
		{
			item.collisionMove(world);
		}
	}
	updateMesh();
}

void DroppedItemsManager::updateMesh()
{
	m_droppedItemsMesh.deleteData();
	DroppedItemsBuilder(*this, m_droppedItemsMesh).buildMesh();
}

void DroppedItemsManager::checkItemsLifetime()
{
	/// @TODO
}

void DroppedItemsManager::checkIfPlayerCanGrabItem(Player & player)
{
	int leftover = 0;
	for (auto item = m_items.begin(); item != m_items.end(); ++item) {
		if (glm::abs(item->position.x - player.position.x) <= 1.2f
			&& glm::abs(item->position.z - player.position.z) <= 1.2f
			&& (glm::abs(item->position.y - player.position.y) <= 1.0f)) {
			
			if (!item->canBeGrabbed())
				return;

			leftover = player.addItem(item->getItemStack().getMaterial(), item->getItemStack().getNumInStack());
			if (leftover)
				item->setItemStackNumber(leftover);
			else
				m_items.erase(item);
			updateMesh();
			break;
		}
	}
}

void DroppedItemsManager::itemsMove(World &world, float dt)
{
	bool shouldUpdateMesh = false;
	for (auto &item : m_items) {
			item.move(world, dt);
			shouldUpdateMesh = true;
	}
	if (shouldUpdateMesh)
		updateMesh();
}
