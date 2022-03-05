#include "DroppedItemsManager.h"

#include "Renderer/RenderMaster.h"
#include "Camera.h"
#include "DroppedItemsBuilder.h"
#include "Audio/SoundMaster.h"
#include "Audio/SoundFunctions.h"

#include <iostream>

DroppedItemsManager::DroppedItemsManager(World * world)
	: m_pWorld(world)
{
}

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
		if (item.position == pos)
			item.startFalling(*m_pWorld);
	}
}

void DroppedItemsManager::update(Player &player, float dt)
{
	checkItemsLifetime();
	checkIfPlayerCanGrabItem(player);
	itemsMove(dt);
	lookForSameItemsNearby();
}

void DroppedItemsManager::addToRender(RenderMaster & renderer)
{
	renderer.drawDroppedItems(m_droppedItemsMesh);
	m_droppedItemsMesh.bufferMesh();
}

void DroppedItemsManager::checkForDroppedItems(const glm::vec3 pos)
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
			item.collisionMove(*m_pWorld);
		}
	}
	updateMesh();
}

void DroppedItemsManager::updateMesh()
{
	DroppedItemsBuilder(*this, m_droppedItemsMesh).buildMesh(m_pWorld);
}

void DroppedItemsManager::checkItemsLifetime()
{
	const static float ITEM_LIFETIME = 60.0f * 5;

	for (auto item = m_items.begin(); item != m_items.end(); ++item) {
		if (item->getExistingTime() > ITEM_LIFETIME) {
			m_items.erase(item);
			updateMesh();
			break;
		}
	}
}

void DroppedItemsManager::checkIfPlayerCanGrabItem(Player & player)
{
	int leftover = 0;
	for (auto item = m_items.begin(); item != m_items.end(); ++item) {

		float diffX = item->position.x - player.position.x;
		float diffY = item->position.y - player.position.y;
		float diffZ = item->position.z - player.position.z;

		if (glm::abs(diffX) <= 1.2f
			&& glm::abs(diffY) <= 1.2f
			&& (glm::abs(diffZ) <= 1.0f)) {
			
			if (!item->canBeGrabbed())
				return;

			leftover = player.addItem(item->getItemStack().getBlockId(), item->getItemStack().getNumInStack());
			if (leftover)
				item->setItemStackNumber(leftover);
			else
				m_items.erase(item);

			//sf::Vector3f diffXYZ{ diffX, diffY, diffZ };
			//sf::Vector3f soundPosition = calculateSoundPosition(diffXYZ, player.rotation.y);
			//
			//g_SoundMaster.play(SoundId::Plop, soundPosition);
			g_SoundMaster.play(SoundId::Plop);
			updateMesh();
			break;
		}
	}
}

void DroppedItemsManager::lookForSameItemsNearby()
{
	for (auto item1 = m_items.begin(); item1 != m_items.end(); ++item1) {
		if (item1->isMoving())
			continue;

		float item1X = item1->position.x;
		float item1Y = item1->position.y;
		float item1Z = item1->position.z;

		for (auto item2 = m_items.begin(); item2 != m_items.end(); ++item2) {
			if (item2->isMoving())
				continue;

			float item2X = item2->position.x;
			float item2Y = item2->position.y;
			float item2Z = item2->position.z;

			if (std::abs(item1X - item2X) <= 2.0f &&
				std::abs(item1Y - item2Y) <= 2.0f &&
				std::abs(item1Z - item2Z) <= 2.0f)
			{
				if (item1->getItemStack().getBlockId() ==
					item2->getItemStack().getBlockId()
					&& item1 != item2)
				{
					int item1Number = item1->getItemStack().getNumInStack();
					int item2Number = item2->getItemStack().getNumInStack();
					int sum = item1Number + item2Number;

					if (sum <= item2->getItemStack().getMaxStackSize()) {
						item2->setItemStackNumber(sum);
						item2->position.y += 0.2f; // item won't be drawn if not change it's position

						m_items.erase(item1);
						updateMesh();
					}
					return;
				}
			}
		}
	}
}

void DroppedItemsManager::itemsMove(float dt)
{
	for (auto item = m_items.begin(); item != m_items.end(); ++item) {
		//if (item->position.y < 0) {
		//	item = m_items.erase(item);
		//	continue;
		//}
		item->move(*m_pWorld, dt);
	}
	
	updateMesh();
}
