#pragma once

#include "SFML/System.hpp"

#include "../ItemStack.h"
#include "Entity.h"

class World;

class DroppedItem : public Entity
{
public:
	DroppedItem(const ItemStack &itemStack, const glm::vec3& pos);
	DroppedItem& operator=(const DroppedItem &item);

	void setRandomAcceleration();
	void setAcceleration(float x, float y);
	void move(World &world, float dt);
	void startMoving() { m_acceleration.y = -0.01f; }
	void collisionMove(World &world);
	void setItemStackNumber(int number);

	float getExistingTime() const { return m_existingTime.getElapsedTime().asSeconds(); }
	ItemStack& getItemStack() { return m_itemStack; }
	bool shouldMove() const { return m_acceleration.y != 0.0f; }
	bool canBeGrabbed() const;
private:
	ItemStack m_itemStack;

	sf::Clock m_existingTime;
	sf::Clock m_unableToGrabTime;

	glm::vec3 m_acceleration;
};