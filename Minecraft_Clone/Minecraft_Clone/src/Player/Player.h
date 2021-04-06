#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <vector>

#include "../Entity.h"
#include "../Input/ToggleKey.h"
#include "DamageMaster.h"
#include "MovementFOV.h"
#include "Inventory/Inventory.h"

class Keyboard;
class World;
class RenderMaster;
class Application;

class Player : public Entity {
public:
	Player(const Config &config, Application &app);
	
	void handleInput(const sf::RenderWindow &window, Keyboard &keyboard);
	void mouseScrollEvent(int delta);
	
	void update(float dt, World &wolrd);

	void addItem(const Material &material, int number);
	void removeHeldItem(int number);
	ItemStack &getHeldItems();

	int getHP() const { return m_hp; }
	int getOxygen() const { return m_oxygen; }

	void drawGUI(RenderMaster &master);
	void drawInventory(RenderMaster &master);
private:
	void collide(World &world, const glm::vec3 &vel, float dt);
	void movementInWater(World &world);
	
	void keyboardInput(Keyboard &keyboard);
	void mouseInput(const sf::RenderWindow &window);
	
	void jump();

	inline void parametersUpdate();
	
	bool m_creativeMode = true;
	bool m_isOnGround = false;
	bool m_isFlying = false;
	bool m_isMoving = false;
	// technically not underwater
	bool m_isUnderwater = false;
	bool m_isSwimming = false;
	bool m_isOnIce = false;
	
	int m_hp = 20;
	int m_oxygen = 20;
	
	Inventory m_Inventory;
	sf::Text m_playerInfo;
	sf::Font m_font;

	ToggleKey m_flyKey;
	ToggleKey m_creativeKey;
	ToggleKey m_postProcKey;
	
	ToggleKey m_inventoryKey;
	ToggleKey m_throwItemKey;
	
	ToggleKey m_num1;
	ToggleKey m_num2;
	ToggleKey m_num3;
	ToggleKey m_num4;
	ToggleKey m_num5;
	ToggleKey m_num6;
	ToggleKey m_num7;
	ToggleKey m_num8;
	ToggleKey m_num9;
	
	glm::vec3 m_acceleration;
	
	DamageMaster m_damageMaster;
	MovementFOV m_movementFOV;
};

#endif // PLAYER_H_INCLUDED