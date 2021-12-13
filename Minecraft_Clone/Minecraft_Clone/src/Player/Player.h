#pragma once

#include <SFML/Graphics.hpp>

#include "../Entity.h"
#include "Inventory/Inventory.h"
#include "World/Block/ChunkBlock.h"
#include "Calculations/DamageMaster.h"
#include "Calculations/MovementFOV.h"
#include "Calculations/SneakingCamera.h"
#include "Calculations/CameraShaking.h"

#include <vector>

class Mouse;
class Keyboard;
class World;
class RenderMaster;
class Application;
class DroppedItemsManager;
class Hand;

class Player : public Entity {
public:
	Player(const Config &config, Application &app);
	
	void handleInput(const sf::RenderWindow &window, Mouse &mouse, Keyboard &keyboard, Hand &hand, World &world);
	void mouseScrollEvent(int delta);
	
	void update(float dt, World &wolrd);

	int addItem(BlockId blockID, int number);
	void removeHeldItem(int number);
	ItemStack &getHeldItems();

	int getHP() const { return m_hp; }
	int getOxygen() const { return m_oxygen; }
	int getHunger() const { return m_hunger; }
	bool isInCreativeMove() const { return m_creativeMode; }
	bool isFlying() const { return m_isFlying; }
	bool isRunning() const { return m_isRunning; }

	void drawGUI(RenderMaster &master);
	void drawInventory(RenderMaster &master);

	void setDroppedItemsManager(DroppedItemsManager* manager);

	bool canEat();
	void eatFood(float hunger, float saturation);

	void loseDurability(int loseDur);

	void openCraftingTable();
private:
	void statsUpdate(float dt);
	void decreaseSaturation();
	void reachCertainSpeed(float neededSpeed, float howFastReachIt);
	void collide(World &world, const glm::vec3 &vel);
	bool sneakyFallCheck(float dt, World &world);
	void movementStops(float dt, World &world);
	void movementInWater(World &world);
	void movementOnIce();

	void processStepSound(World &world);
	void processFallSound(World &world);
	
	void keyboardInput(Keyboard &keyboard, World &world);
	void mouseInput(const sf::RenderWindow &window);
	
	void jump(World &world);

	void parametersUpdate();
	
	bool m_mouseLocked = false;
	bool m_creativeMode = false;
	bool m_isFlying = false;
	bool m_isOnIce = false;
	bool m_isMoving = false;
	bool m_isRunning = false;
	bool m_isSneaking = false;
	// not really on ground, use m_isReallyOnGround instead
	bool m_isOnGround = false;
	// use this variable to know if the player stands on the ground
	bool m_isReallyOnGround = false;

	bool m_isSwimming = false;
	bool m_isOnWaterSurface = false;
	bool m_isDivnigDown = false;

	float m_lastTopPosition = 0.0f;
	float m_lastPositionY = 0.0f;

	int m_hp;
	int m_oxygen;
	int m_hunger;
	float m_saturationLevel;
	float m_exhaustionLevel = 0.0f;
	sf::Clock m_regenerationTimer;

	sf::Clock m_jumpDelayTimer;
	sf::Clock m_fallDelayTimer;
	
	Inventory m_Inventory;
	sf::Text m_playerInfo;
	sf::Font m_font;
	
	glm::vec3 m_acceleration;
	
	DamageMaster m_damageMaster;
	MovementFOV m_movementFOV;
	SneakingCamera m_sneakingCamera;
	CameraShaking m_cameraShaking;
};