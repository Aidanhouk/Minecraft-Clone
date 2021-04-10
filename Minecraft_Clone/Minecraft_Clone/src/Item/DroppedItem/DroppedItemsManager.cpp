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
	m_items.emplace_back(itemstack, glm::vec3(pos.x, pos.y + 0.5f, pos.z));
	updateMesh();
	m_items.back().setAcceleration(rotation.x, rotation.y);
}

void DroppedItemsManager::blockBrokenUpdate(const glm::vec3 & pos)
{
	for (auto & item : m_items) {
		if (floor(item.position.x) == pos.x &&
			floor(item.position.z) == pos.z &&
			floor(item.position.y) - 1 == pos.y) {
			item.startMoving();
		}
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
	for (auto item = m_items.begin(); item != m_items.end(); ++item) {
		if (glm::abs(item->position.x - player.position.x) <= 1.2f
			&& glm::abs(item->position.z - player.position.z) <= 1.2f
			&& (glm::abs(item->position.y - player.position.y) <= 1.0f)) {
			
			if (!item->canBeGrabbed())
				return;

			player.addItem(item->getItemStack().getMaterial(), item->getItemStack().getNumInStack());
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
		if (!item.isMoving()) {
			item.move(world, dt);
			shouldUpdateMesh = true;
		}
	}
	if (shouldUpdateMesh)
		updateMesh();
}
