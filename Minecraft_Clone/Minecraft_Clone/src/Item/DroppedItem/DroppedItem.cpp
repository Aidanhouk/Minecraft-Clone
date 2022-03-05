#include "DroppedItem.h"

#include "Util/Random.h"
#include "World/World/World.h"
#include "Player/Player.h"

#include "Player/PlayerInfo.h"
#include "Audio/SoundMaster.h"

#include <iostream>

DroppedItem::DroppedItem(const ItemStack &itemStack, const glm::vec3 & pos)
	: Entity(pos, { 0.0f, 0.0f, 0.0f }, { 0.15f, 0.3f, 0.15f }),
	m_itemStack(itemStack)
{
	m_existingTime.restart();
	m_unableToGrabTime.restart();
}

DroppedItem& DroppedItem::operator=(const DroppedItem &item)
{
	position = item.position;
	rotation = item.rotation;
	velocity = item.velocity;

	m_itemStack = item.m_itemStack;
	m_existingTime = item.m_existingTime;

	return *this;
}

void DroppedItem::setRandomAcceleration()
{
	m_acceleration.y = 4.0f;
	do {
		m_acceleration.x = (rand() % 21 - 10) / 10.0f;
		m_acceleration.z = (rand() % 21 - 10) / 10.0f;
	}
	while (m_acceleration.x == 0.0f && m_acceleration.z == 0.0f);
}

void DroppedItem::setAcceleration(float x, float y)
{
	if (y < 90.0f) {
		m_acceleration.x = y;
		m_acceleration.z = y - 90.0f;
	}
	else if (y < 180.0f) {
		m_acceleration.x = 180.0f - y;
		m_acceleration.z = y - 90.0f;
	}
	else if (y < 270.0f) {
		m_acceleration.x = 180.0f - y;
		m_acceleration.z = 270.0f - y;
	}
	else if (y < 360.0f) {
		m_acceleration.x = y - 360.0f;
		m_acceleration.z = 270.0f - y;
	}

	m_acceleration.x /= 15.0f;
	m_acceleration.z /= 15.0f;

	if (x < -30.0f)
		m_acceleration.y = 30.0f / 15.0f;
	else if (x > 60.0f)
		m_acceleration.y = -60.0f / 15.0f;
	else
		m_acceleration.y = -x / 15.0f;
}

void DroppedItem::move(World &world, float dt)
{
	// if player is far from item then item isn't moving
	if (
		glm::pow(position.x - g_PlayerInfo.player->position.x, 2) +
		glm::pow(position.y - g_PlayerInfo.player->position.y, 2) +
		glm::pow(position.z - g_PlayerInfo.player->position.z, 2) >
		glm::pow(std::max(g_Config.renderDistance - 3, 1) * 16, 2)
		) {
		return;
	}

	// if the item hasn't stopped moving
	if (m_acceleration.y != 0.0f) {
		// if the item is underwater
		if (world.getBlock(position.x, position.y, position.z).getData().shaderType == BlockShaderType::Liquid) {
			if (m_isInWater == false) {
				g_SoundMaster.addSound(SoundId::WaterSplashNormal, this);
			}
			m_isInWater = true;

			position.x += m_acceleration.x * dt / 5.0f;
			position.z += m_acceleration.z * dt / 5.0f;
			position.y += m_acceleration.y * dt / 5.0f;

			m_acceleration.y -= 15.0f * dt / 5.0f;
		}
		else {
			m_isInWater = false;

			position.x += m_acceleration.x * dt;
			position.z += m_acceleration.z * dt;
			position.y += m_acceleration.y * dt;

			m_acceleration.y -= 15.0f * dt;
		}
	}

	// if the item hasn't collided any other block
	if (m_acceleration.x != 0.0f || m_acceleration.z != 0.0f) {

		for (int x = position.x - box.dimensions.x; x < position.x + box.dimensions.x; ++x) {
			for (int y = position.y - box.dimensions.y; y < position.y + box.dimensions.y; ++y) {
				for (int z = position.z - box.dimensions.z; z < position.z + box.dimensions.z; ++z) {

					if (world.getBlock(x, y, z).getData().isCollidable) {
						position.x -= m_acceleration.x * dt;
						position.z -= m_acceleration.z * dt;
						
						m_acceleration.x = 0.0f;
						m_acceleration.z = 0.0f;
					}
				}
			}
		}
	}
	else {

		auto blockBelow = world.getBlock(position.x, position.y - box.dimensions.y, position.z);
		if (blockBelow.getData().isCollidable) {
			m_acceleration.y = 0.0f;
			position.y += 0.001f;
		}
		else if (m_acceleration.y == 0.0f)
			m_acceleration.y = -0.1f;

		auto blockAbove = world.getBlock(position.x, position.y + 0.6f * box.dimensions.y, position.z);
		if (blockAbove.getData().isCollidable) {
			position.y -= 2 * m_acceleration.y * dt;
			m_acceleration.y = -0.01f;
		}
	}
}

void DroppedItem::startFalling(World & world)
{
	if (m_acceleration.x == 0.0f && m_acceleration.z == 0.0f) {
		auto blockBelow = world.getBlock(position.x, position.y - box.dimensions.y, position.z);
		if (blockBelow != 0 && blockBelow.getData().isCollidable) {
			m_acceleration.y = 0.0f;
		}
		else if (m_acceleration.y == 0.0f)
			m_acceleration.y = -0.1f;
	}
}

void DroppedItem::collisionMove(World &world)
{
	int x = position.x, y = position.y, z = position.z;

	if (world.getBlock(x + 1, y, z) == 0) {
		// ceil(x) in these cases might not work
		// e.g. if x == 10.0 we need 11.0, but ceil will return 10.0
		position.x = floor(x) + 1 + box.dimensions.x;
	}
	else if (world.getBlock(x - 1, y, z) == 0) {
		position.x = floor(x) - box.dimensions.x;
	}

	else if (world.getBlock(x, y, z + 1) == 0) {
		position.z = floor(z) + 1 + box.dimensions.z;
	}
	else if (world.getBlock(x, y, z - 1) == 0) {
		position.z = floor(z) - box.dimensions.z;
	}

	else if (world.getBlock(x, y + 1, z) == 0) {
		position.y = floor(y) + 1 + box.dimensions.y;
	}
	else if (world.getBlock(x, y - 1, z) == 0) {
		position.y = floor(y) - box.dimensions.y;
	}

	m_acceleration.x = 0.0f;
	m_acceleration.z = 0.0f;
	m_acceleration.y = -0.01f;
}

void DroppedItem::setItemStackNumber(int number)
{
	m_itemStack.setData(m_itemStack.getBlockId(), number);
}

bool DroppedItem::isMoving() const
{
	return (
		m_acceleration.x != 0.0f ||
		m_acceleration.y != 0.0f ||
		m_acceleration.z != 0.0f
		);
}

bool DroppedItem::shouldMove() const
{
	return (m_acceleration.x != 0.0f || m_acceleration.z != 0.0f);
}

bool DroppedItem::canBeGrabbed() const
{
	return m_unableToGrabTime.getElapsedTime().asSeconds() > 0.6f;
}
