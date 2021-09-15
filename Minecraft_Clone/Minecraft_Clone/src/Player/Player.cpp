#include "Player.h"

#include <SFML/Graphics.hpp>

#include <iomanip>
#include <iostream>
#include <sstream>

#include "../Input/Keyboard.h"
#include "../Renderer/RenderMaster.h"
#include "../World/World.h"
#include "../RenderSettings.h"
#include "PlayerInfo.h"
#include "GlobalInfo.h"

#include "World/Generation/Terrain/ClassicOverWorldGenerator.h"

#include <iostream>

Player::Player(const Config &config, Application &app)
	: Entity({ 2500, 125, 2500 }, { 0.0f, 0.0f, 0.0f }, { 0.3f, 1.0f, 0.3f }),
	m_inventoryKey(sf::Keyboard::E),
	m_throwItemKey(sf::Keyboard::Q),
	m_flyKey(sf::Keyboard::F),
	m_creativeKey(sf::Keyboard::Tab),
	m_postProcKey(sf::Keyboard::P),
	m_num1(sf::Keyboard::Num1),
	m_num2(sf::Keyboard::Num2),
	m_num3(sf::Keyboard::Num3),
	m_num4(sf::Keyboard::Num4),
	m_num5(sf::Keyboard::Num5),
	m_num6(sf::Keyboard::Num6),
	m_num7(sf::Keyboard::Num7),
	m_num8(sf::Keyboard::Num8),
	m_num9(sf::Keyboard::Num9),
	m_acceleration(glm::vec3(0.f)),
	m_movementFOV(config),
	m_Inventory(*this, app)
{
    m_font.loadFromFile("Res/Fonts/MinecraftRegular.otf");

	m_playerInfo.setFont(m_font);
	m_playerInfo.setOutlineColor(sf::Color::Black);
	m_playerInfo.setCharacterSize(25);
	m_playerInfo.setPosition(0.0f, 50.0f);
}

int Player::addItem(const Material &material, int number)
{
	return m_Inventory.addItems(material, number);
}

void Player::removeHeldItem(int number)
{
	m_Inventory.removeHeldItem(number);
}

ItemStack &Player::getHeldItems()
{
    return m_Inventory.getHeldItems();
}

void Player::handleInput(const sf::RenderWindow &window, Keyboard &keyboard)
{
	keyboardInput(keyboard);
	mouseInput(window);

	p_info.position = position;

	if (m_inventoryKey.isKeyPressed()) {
		m_Inventory.showOrHideInventory();
	}

	if (p_info.interfaceCursor) {
		m_Inventory.mouseInput(window);
		return;
	}
	
	if (m_flyKey.isKeyPressed()) {
		if (m_creativeMode)
			m_isFlying = !m_isFlying;
	}
	if (m_creativeKey.isKeyPressed()) {
		m_creativeMode = !m_creativeMode;
		m_isFlying = false;
	}
	if (m_postProcKey.isKeyPressed()) {
		g_Config.postProcess = !g_Config.postProcess;
	}

	if (m_throwItemKey.isKeyPressed()) {
		m_Inventory.throwItem(1);
		m_Inventory.removeHeldItem(1);
	}
	
	if (m_num1.isKeyPressed()) {
		m_Inventory.setHeldItem(0);
	}
	else if (m_num2.isKeyPressed()) {
		m_Inventory.setHeldItem(1);
	}
	else if (m_num3.isKeyPressed()) {
		m_Inventory.setHeldItem(2);
	}
	else if (m_num4.isKeyPressed()) {
		m_Inventory.setHeldItem(3);
	}
	else if (m_num5.isKeyPressed()) {
		m_Inventory.setHeldItem(4);
	}
	else if (m_num6.isKeyPressed()) {
		m_Inventory.setHeldItem(5);
	}
	else if (m_num7.isKeyPressed()) {
		m_Inventory.setHeldItem(6);
	}
	else if (m_num8.isKeyPressed()) {
		m_Inventory.setHeldItem(7);
	}
	else if (m_num9.isKeyPressed()) {
		m_Inventory.setHeldItem(8);
	}
}

void Player::mouseScrollEvent(int delta)
{
	if (p_info.interfaceCursor)
		return;
	/// Wheel down = next item
	if (delta < 0)
		m_Inventory.nextItem();
	/// Wheel up = previous item
	else
		m_Inventory.previousItem();
}

void Player::update(float dt, World &world)
{
    velocity += m_acceleration;
    m_acceleration = {0, 0, 0};

    if (!m_isFlying) {
        if (!m_isOnGround) {
			if (m_isUnderwater) {
				/// -3.0 is a drowning speed
				if (velocity.y < -3.1f) {
					velocity.y += 80 * dt;
				}
				else if (velocity.y > -2.9f) {
					velocity.y -= 80 * dt;
				}
			}
			else
				velocity.y -= 40 * dt;
        }
		m_isOnGround = false;
    }

    position.x += velocity.x * dt;
    collide(world, {velocity.x, 0, 0});

    position.y += velocity.y * dt;
    collide(world, {0, velocity.y, 0});

    position.z += velocity.z * dt;
    collide(world, {0, 0, velocity.z});

	movementInWater(world);

	box.update(position);

	static double timePassed = 0.0;
	timePassed += dt;
	
	if (world.getBlock(position.x, position.y - 2, position.z).getData().id == BlockId::Ice
		|| world.getBlock(position.x, position.y - 3, position.z).getData().id == BlockId::Ice) {
		m_isOnIce = true;

		if (timePassed > 1.0 / 50.0) {
			timePassed = 0.0;
			velocity.x *= 0.96f;
			velocity.z *= 0.96f;
		}
	}
	else {
		m_isOnIce = false;

		velocity.x *= 0.95f;
		velocity.z *= 0.95f;

		/// hack for low framerates
		if (!m_isMoving) {
			velocity.x *= 0.9f;
			velocity.z *= 0.9f;
		}
	}

    if (m_isFlying) {
        velocity.y *= 0.95f;
    }
}

void Player::collide(World &world, const glm::vec3 &vel)
{
	for (int x = position.x - box.dimensions.x; x < position.x + box.dimensions.x; ++x) {
		for (int y = position.y - box.dimensions.y; y < position.y + 0.97f; ++y) {
			for (int z = position.z - box.dimensions.z; z < position.z + box.dimensions.z; ++z) {
				
				auto block = world.getBlock(x, y, z);

				if (block != 0 && block.getData().isCollidable) {

					if (block.getData().id == BlockId::Cactus && !m_creativeMode) {
						int lastHP = m_hp;
						m_hp -= m_damageMaster.getCactusDamage();

						if (lastHP != m_hp)
							parametersUpdate();
						///@TODO proccess death
						//if (m_hp <= 0)
					}

					if (vel.y > 0) {
						position.y = y - box.dimensions.y;
						velocity.y = 0;
					}
					/// When on ground
					else if (vel.y < 0) {
						if (vel.y < -25.0f && !m_creativeMode) {
							int lastHP = m_hp;
							m_hp -= m_damageMaster.getFallDamage(vel.y);

							if (lastHP != m_hp)
								parametersUpdate();
							///@TODO proccess death
							//if (m_hp <= 0)
						}
						m_isOnGround = true;
						position.y = y + box.dimensions.y + 1;
						velocity.y = 0;
					}

					if (vel.x > 0) {
						position.x = x - box.dimensions.x;
					}
					else if (vel.x < 0) {
						position.x = x + box.dimensions.x + 1;
					}

					if (vel.z > 0) {
						position.z = z - box.dimensions.z;
					}
					else if (vel.z < 0) {
						position.z = z + box.dimensions.z + 1;
					}
				}
			}
		}
	}
}

void Player::movementInWater(World & world)
{
	bool breath = true;
	if (world.getBlock(position.x, position.y, position.z).getData().id == BlockId::Water) {
		m_isUnderwater = true;
		m_isSwimming = true;
		/// Underwater, bad vision
		if (position.y < WATER_LEVEL + 0.3f) {
			if (p_info.underwater != true) {
				parametersUpdate();
			}
			p_info.underwater = true;
			if (!m_creativeMode) {
				breath = false;
				if (m_oxygen > 0) {
					int lastOxygen = m_oxygen;
					m_oxygen -= m_damageMaster.getChokeDamage();

					if (lastOxygen != m_oxygen)
						parametersUpdate();
				}
				else {
					int lastHP = m_hp;
					m_hp -= m_damageMaster.getChokeDamage();

					if (lastHP != m_hp)
						parametersUpdate();
					///@TODO proccess death
					//if (m_hp <= 0)
				}
			}
		}
		else {
			p_info.underwater = false;
		}
	}
	else {
		m_isUnderwater = false;
		p_info.underwater = false;
		if (world.getBlock(position.x, position.y - 2, position.z).getData().id == BlockId::Water)
			m_isSwimming = true;
		else {
			m_isSwimming = false;
		}
	}
	if (breath && m_oxygen < 20) {
		int lastOxygen = m_oxygen;
		m_oxygen += m_damageMaster.breath();

		if (lastOxygen != m_oxygen)
			parametersUpdate();
	}
}

void Player::keyboardInput(Keyboard &keyboard)
{
	static float const m_SPEED = 0.16f;
	static bool isRunning = false;
	
	if (!p_info.canMove)
		return;
	
	isRunning = false;
	m_isMoving = false;
	if (keyboard.isKeyDown(sf::Keyboard::W)) {
		m_isMoving = true;
	    float speed = m_SPEED;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
			isRunning = true;
			m_movementFOV.sprint();
			if (m_isFlying)
				speed *= 5;
			else
				speed *= 2;
		}
		if (!m_isFlying && (m_isUnderwater || m_isSwimming))
			speed *= 0.5f;
	    m_acceleration.x += -glm::cos(glm::radians(rotation.y + 90)) * speed;
	    m_acceleration.z += -glm::sin(glm::radians(rotation.y + 90)) * speed;
	}
	if (!isRunning)
		m_movementFOV.walk();
	if (keyboard.isKeyDown(sf::Keyboard::S)) {
		m_isMoving = true;
		float speed = m_SPEED;
		if (!m_isFlying && (m_isUnderwater || m_isSwimming))
			speed *= 0.5f;
	    m_acceleration.x += glm::cos(glm::radians(rotation.y + 90)) * speed;
	    m_acceleration.z += glm::sin(glm::radians(rotation.y + 90)) * speed;
	}
	if (keyboard.isKeyDown(sf::Keyboard::A)) {
		m_isMoving = true;
		float speed = m_SPEED;
		if (!m_isFlying && (m_isUnderwater || m_isSwimming))
			speed *= 0.5f;
	    m_acceleration.x += -glm::cos(glm::radians(rotation.y)) * speed;
	    m_acceleration.z += -glm::sin(glm::radians(rotation.y)) * speed;
	}
	if (keyboard.isKeyDown(sf::Keyboard::D)) {
		m_isMoving = true;
		float speed = m_SPEED;
		if (!m_isFlying && (m_isUnderwater || m_isSwimming))
			speed *= 0.5f;
	    m_acceleration.x += glm::cos(glm::radians(rotation.y)) * speed;
	    m_acceleration.z += glm::sin(glm::radians(rotation.y)) * speed;
	}
	
	if (m_isOnIce) {
		const float VEL = 7.0f;
		if (!isRunning) {
			if (glm::abs(velocity.x) > VEL / 2.0f
				|| glm::abs(velocity.z) > VEL / 2.0f) {
				m_acceleration.x = 0.0f;
				m_acceleration.z = 0.0f;
			}
		}
		else if (glm::abs(velocity.x) > VEL
			|| glm::abs(velocity.z) > VEL) {
			m_acceleration.x = 0.0f;
			m_acceleration.z = 0.0f;
		}
	}
	
	if (keyboard.isKeyDown(sf::Keyboard::Space)) {
		jump();
	}
	else if (keyboard.isKeyDown(sf::Keyboard::LShift) && m_isFlying) {
	    m_acceleration.y -= 0.6;
	}
	
	if (keyboard.isKeyDown(sf::Keyboard::F5)) {
		g_info.weather = Weather::Rain;
	}
	else if (keyboard.isKeyDown(sf::Keyboard::F6)) {
		g_info.weather = Weather::Snowfall;
	}
	else if (keyboard.isKeyDown(sf::Keyboard::F7)) {
		g_info.weather = Weather::None;
	}
}

void Player::mouseInput(const sf::RenderWindow &window)
{
	static sf::Vector2i lastMousePosition = sf::Mouse::getPosition(window);
	
	static bool useMouse = true;
	
	static ToggleKey useMouseKey(sf::Keyboard::L);
	if (useMouseKey.isKeyPressed()) {
	    useMouse = !useMouse;
	}
	if (!useMouse || p_info.interfaceCursor) {
		lastMousePosition = sf::Mouse::getPosition(window);
	    return;
	}
	
	auto change = sf::Mouse::getPosition() - lastMousePosition;
	
	rotation.y += change.x * 0.05f;
	rotation.x += change.y * 0.05f;

	/// if degree > 87.1 then the ray which detects the block player aiming at, doesn't work correctly
	static float const BOUND = 87.1f;
	if (rotation.x > BOUND)
	    rotation.x = BOUND;
	else if (rotation.x < -BOUND)
	    rotation.x = -BOUND;
	
	if (rotation.y > 360.0f)
	    rotation.y = 0.0f;
	else if (rotation.y < 0.0f)
	    rotation.y = 360.0f;
	
	//auto cx = static_cast<int>(window.getSize().x / 2);
	//auto cy = static_cast<int>(window.getSize().y / 2);
	//
	//sf::Mouse::setPosition({cx, cy}, window);
	//
	//lastMousePosition = sf::Mouse::getPosition();

	static sf::Vector2i center = {
		g_renderSettings.resolutionX >> 1,
		g_renderSettings.resolutionY >> 1
	};

	auto windowSize = window.getSize();
	int maxRadius = glm::min(windowSize.x, windowSize.y) / 3;
	glm::vec2 delta = glm::vec2(lastMousePosition.x, lastMousePosition.y) - glm::vec2(center.x, center.y);
	float len = glm::length(delta);
	if (len > maxRadius) {
		sf::Mouse::setPosition(sf::Vector2i((int)center.x, (int)center.y), window);
		lastMousePosition = center;
	}
	else {
		lastMousePosition = sf::Mouse::getPosition(window);
	}
}

void Player::jump()
{
	if (!m_isFlying) {
		if (m_isUnderwater) {
			m_isOnGround = false;
			if (velocity.y > -4.6)
				velocity.y = 5.0f;
		}
		else if (m_isOnGround) {
			m_isOnGround = false;
			m_acceleration.y += 10.0f;
		}
	}
	else {
		m_acceleration.y = 0.6f;
	}
}

inline void Player::parametersUpdate()
{
	m_Inventory.updateIcons();
}

void Player::drawGUI(RenderMaster &master)
{
    std::ostringstream stream;
	stream
		<< " X: " << (int)position.x
		<< " Y: " << (int)position.y
		<< " Z: " << (int)position.z
		<< "\n Close HUD <F3>"
		<< std::boolalpha
		<< "\n Creative mode <Tab> - " << m_creativeMode
		<< "\n Flying (in creative mode) <F> - " << m_isFlying
		<< "\n Post processing <P> - " << g_Config.postProcess
		<< "\n Weather <F5/F6/F7> - ";

	switch (g_info.weather)
	{
	case Weather::None:
		stream << "Clear";
		break;
	case Weather::Rain:
		stream << "Rain";
		break;
	case Weather::Snowfall:
		stream << "Snowfall";
		break;
	}

	stream << "\n Time <+/-> : ";
	if (g_info.dayTime >= 18000) {
		stream << g_info.dayTime / 1000 - 18 << " AM";
	}
	else if (g_info.dayTime < 6000) {
		stream << g_info.dayTime / 1000 + 6 << " AM";
	}
	else {
		stream << g_info.dayTime / 1000 - 6 << " PM";
	}

	m_playerInfo.setString(stream.str());

    master.drawSFML(m_playerInfo);
}

void Player::drawInventory(RenderMaster & master)
{
	m_Inventory.draw(master);
}

void Player::setDroppedItemsManager(DroppedItemsManager * manager)
{
	m_Inventory.setDroppedItemsManager(manager);
}
