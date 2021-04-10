#include "DroppedItem.h"

#include "Util/Random.h"
#include "World/World.h"

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

	Random rand;
	do {
		m_acceleration.x = (float)rand.intInRange(-10, 10) / 10.0f;
		m_acceleration.z = (float)rand.intInRange(-10, 10) / 10.0f;
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
	position.x += m_acceleration.x * dt;
	position.z += m_acceleration.z * dt;
	position.y += m_acceleration.y * dt;

	m_acceleration.y -= 15.0f * dt;

	/// if the block hasn't collided any other block
	if (m_acceleration.x != 0.0f && m_acceleration.z != 0.0f) {
		for (int x = position.x - box.dimensions.x; x < position.x + box.dimensions.x; ++x) {
			for (int y = position.y - box.dimensions.y; y < position.y + box.dimensions.y; ++y) {
				for (int z = position.z - box.dimensions.z; z < position.z + box.dimensions.z; ++z) {

					auto block = world.getBlock(x, y, z);

					if (block != 0 && block.getData().isCollidable) {
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
		if (blockBelow != 0 && blockBelow.getData().isCollidable) {
			m_acceleration.y = 0.0f;
		}

		auto blockAbove = world.getBlock(position.x, position.y + 0.6f * box.dimensions.y, position.z);
		if (blockAbove != 0 && blockAbove.getData().isCollidable) {
			m_acceleration.y = -0.01f;
		}
	}
}

bool DroppedItem::canBeGrabbed() const
{
	return m_unableToGrabTime.getElapsedTime().asSeconds() > 1.0f;
}
