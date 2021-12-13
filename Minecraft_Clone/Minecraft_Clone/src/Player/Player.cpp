#include "Player.h"

#include <SFML/Graphics.hpp>

#include <iomanip>
#include <sstream>
#include <memory>
#include <thread>

#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Input/ToggleKey.h"
#include "Renderer/RenderMaster.h"
#include "World/World/World.h"
#include "RenderSettings.h"
#include "PlayerInfo.h"
#include "GlobalInfo.h"
#include "PlayerHand/Hand.h"
#include "Application.h"
#include "Audio/SoundMaster.h"
#include "Audio/SoundFunctions.h"
#include "PlayerConstants.h"

#include <iostream>

Player::Player(const Config &config, Application &app)
	: Entity({ 2500, 125, 2500 }, { 0.0f, 0.0f, 0.0f }, { 0.3f, 1.0f, 0.3f }),
	m_acceleration(glm::vec3(0.f)),
	m_movementFOV(config),
	m_Inventory(*this, app),
	m_hp(MAX_HP),
	m_hunger(MAX_HUNGER),
	m_oxygen(MAX_OXYGEN),
	m_saturationLevel(SATURATION_DEFAULT)
{
	g_PlayerInfo.player = this;

    m_font.loadFromFile("Res/Fonts/MinecraftRegular.otf");

	m_playerInfo.setFont(m_font);
	m_playerInfo.setOutlineColor(sf::Color::Black);
	m_playerInfo.setCharacterSize(app.getWindow().getSize().x / 100);
	m_playerInfo.setPosition(0.0f, 50.0f * app.getWindow().getSize().x / 2560);
}

int Player::addItem(BlockId blockID, int number)
{
	return m_Inventory.addItems(blockID, number);
}

void Player::removeHeldItem(int number)
{
	m_Inventory.removeHeldItem(number);
}

ItemStack &Player::getHeldItems()
{
    return m_Inventory.getHeldItems();
}

void Player::handleInput(const sf::RenderWindow &window, Mouse &mouse, Keyboard &keyboard, Hand &hand, World &world)
{
	keyboardInput(keyboard, world);
	mouseInput(window);

	if (keyboard.toggle(sf::Keyboard::E)) {
		m_Inventory.showOrHideInventory(InterfaceType::Inventory);
	}
	if (g_PlayerInfo.inventoryCursor) {
		m_Inventory.mouseInput(window, mouse);
		if (keyboard.toggle(sf::Keyboard::Escape))
			m_Inventory.showOrHideInventory(InterfaceType::Closed);
		return;
	}

	if (keyboard.toggle(sf::Keyboard::F) && m_creativeMode) {
		m_isFlying = !m_isFlying;
		m_lastTopPosition = position.y;
	}
	else if (keyboard.toggle(sf::Keyboard::Tab)) {
		m_creativeMode = !m_creativeMode;

		if (m_creativeMode) {
			m_hp = 20;
			m_oxygen = 20;
			m_hunger = 20;
		}
		else {
			m_isFlying = false;
			m_lastTopPosition = position.y;
		}

		m_Inventory.shouldUpdateIcons();
	}
	else if (keyboard.toggle(sf::Keyboard::P)) {
		g_Config.postProcess = !g_Config.postProcess;	
	}
	else if (keyboard.toggle(sf::Keyboard::F3)) {
		g_PlayerInfo.FPS_HUD = !g_PlayerInfo.FPS_HUD;
	}
	else if (keyboard.toggle(sf::Keyboard::F4)) {
		g_Info.fog = !g_Info.fog;
	}
	else if (keyboard.toggle(sf::Keyboard::F5)) {
		g_Info.weather = !g_Info.weather;
	}
	else if (keyboard.toggle(sf::Keyboard::Q)) {
		if (m_Inventory.getHeldItems().getNumInStack() > 0) {
			hand.swing();
			hand.leftMouseHold();
		}
		m_Inventory.throwItem(1);
		m_Inventory.removeHeldItem(1);
	}

	if (keyboard.toggle(sf::Keyboard::Num1)) {
		m_Inventory.setHeldItem(0);
	}
	else if (keyboard.toggle(sf::Keyboard::Num2)) {
		m_Inventory.setHeldItem(1);
	}
	else if (keyboard.toggle(sf::Keyboard::Num3)) {
		m_Inventory.setHeldItem(2);
	}
	else if (keyboard.toggle(sf::Keyboard::Num4)) {
		m_Inventory.setHeldItem(3);
	}
	else if (keyboard.toggle(sf::Keyboard::Num5)) {
		m_Inventory.setHeldItem(4);
	}
	else if (keyboard.toggle(sf::Keyboard::Num6)) {
		m_Inventory.setHeldItem(5);
	}
	else if (keyboard.toggle(sf::Keyboard::Num7)) {
		m_Inventory.setHeldItem(6);
	}
	else if (keyboard.toggle(sf::Keyboard::Num8)) {
		m_Inventory.setHeldItem(7);
	}
	else if (keyboard.toggle(sf::Keyboard::Num9)) {
		m_Inventory.setHeldItem(8);
	}
}

void Player::mouseScrollEvent(int delta)
{
	if (g_PlayerInfo.inventoryCursor || g_PlayerInfo.gameState != GameState::PLAYING)
		return;
	// Wheel down = next item
	if (delta < 0)
		m_Inventory.nextItem();
	// Wheel up = previous item
	else
		m_Inventory.previousItem();
}

void Player::update(float dt, World &world)
{
	const float MIN_LIGHT_VALUE = 0.1f;
	const float	LIGHT_MULTIPLY_COEF = (1.0f - MIN_LIGHT_VALUE) / 15.0f; // 15 here is max light value

	//VectorXZ chunkPos = { (int)position.x / CHUNK_SIZE, (int)position.z / CHUNK_SIZE };
	//Chunk &chunk = world.getChunkManager().getChunk(chunkPos.x, chunkPos.z);
	//VectorXZ blockPos = { (int)position.x % CHUNK_SIZE, (int)position.z % CHUNK_SIZE };
	//ChunkBlock playerBlock = chunk.getBlock(blockPos.x, position.y, blockPos.z);
	//
	//if (position.y > chunk.getHeightAt(blockPos.x, blockPos.z)) {
	//	g_PlayerInfo.playerLighting = std::max(
	//		15 * g_Info.lighting,
	//		(float)playerBlock.getTorchLight()) * LIGHT_MULTIPLY_COEF + MIN_LIGHT_VALUE;
	//}
	//else {
	//	g_PlayerInfo.playerLighting = std::max(
	//		playerBlock.getSunLight() * g_Info.lighting,
	//		(float)playerBlock.getTorchLight()) * LIGHT_MULTIPLY_COEF + MIN_LIGHT_VALUE;
	//}
	ChunkBlock playerBlock = world.getBlock(position.x, position.y, position.z);
	g_PlayerInfo.playerLighting = std::max(
		playerBlock.getSunLight() * g_Info.lighting,
		(float)playerBlock.getTorchLight()) * LIGHT_MULTIPLY_COEF + MIN_LIGHT_VALUE;

	if (g_PlayerInfo.gameState != GameState::PLAYING)
		return;

	if (!sneakyFallCheck(dt, world))
		velocity += m_acceleration;
	m_acceleration = { 0, 0, 0 };

	if (!m_isFlying) {
		if (!m_isOnGround) {
			if (m_isSwimming) {
				if (position.y >= WATER_LEVEL + 0.3f)
					reachCertainSpeed(DROWNING_SPEED_ON_SURFACE, 80 * dt);
				else
					reachCertainSpeed(DROWNING_SPEED_UNDERWATER, 80 * dt);

				if (m_isDivnigDown)
					reachCertainSpeed(-SWIMMING_VERTICAL_SPEED, 100 * dt);
			}
			else {
				velocity.y -= 40 * dt;

				if (position.y > m_lastPositionY || position.y == m_lastPositionY)
					m_lastTopPosition = position.y;
				m_lastPositionY = position.y;
			}
		}
		m_isOnGround = false;
	}

	//static bool r = true;
	//static ToggleKey R(sf::Keyboard::R);
	//if (R.isKeyPressed())
	//	r = !r;

	position.x += velocity.x * dt;
	//if (r)
	collide(world, { velocity.x, 0, 0 });
	if (position.x < 0)
		position.x = 0;

	position.z += velocity.z * dt;
	//if (r)
	collide(world, { 0, 0, velocity.z });
	if (position.z < 0)
		position.z = 0;

	position.y += velocity.y * dt;
	//if (r)
	collide(world, { 0, velocity.y, 0 });
	if (position.y < 0)
		position.y = 0;


	movementInWater(world);

	box.update(position);

	movementStops(dt, world);

	if (!m_isMoving)
		m_cameraShaking.defaultPosition();

	if (!m_creativeMode)
		statsUpdate(dt);

	parametersUpdate();
}

void Player::statsUpdate(float dt)
{
	if (m_regenerationTimer.getElapsedTime().asSeconds() > 0.5f
		&& m_hp < MAX_HP && m_hp > 0 && m_hunger == 20 && m_saturationLevel > 0.0f) {
		++m_hp;
		m_Inventory.shouldUpdateIcons();
		m_exhaustionLevel += 6.0f;
		m_regenerationTimer.restart();
	}

	if (m_regenerationTimer.getElapsedTime().asSeconds() > 4.0f
		&& m_hp < MAX_HP && m_hp > 0 && m_hunger >= 18) {
		++m_hp;
		m_Inventory.shouldUpdateIcons();
		m_exhaustionLevel += 6.0f;
		m_regenerationTimer.restart();
	}

	static sf::Clock hungerAffectHealthTimer;
	if (hungerAffectHealthTimer.getElapsedTime().asSeconds() > 4.0f
		&& m_hunger == 0 && m_hp > 1) {
		--m_hp;
		m_Inventory.shouldUpdateIcons();

		g_SoundMaster.play(SoundId::PlayerWasHit);
		hungerAffectHealthTimer.restart();
	}

	if (m_isRunning)
		m_exhaustionLevel += dt / 7.0f * 4.0f
		/ 2;

	if (m_exhaustionLevel >= 4.0f) {
		m_exhaustionLevel = 0.0f;
		decreaseSaturation();
	}
}

void Player::decreaseSaturation()
{
	if (m_saturationLevel > 0.0f) {
		--m_saturationLevel;
		if (m_saturationLevel < 0.0f)
			m_saturationLevel = 0.0f;
	}
	else if (m_hunger > 0) {
		--m_hunger;
		m_Inventory.shouldUpdateIcons();
	}
}

bool Player::canEat()
{
	return m_hunger < MAX_HUNGER;
}

void Player::eatFood(float hunger, float saturation)
{
	int lastHunger = m_hunger;

	m_hunger += hunger;
	if (m_hunger > MAX_HUNGER)
		m_hunger = MAX_HUNGER;

	if (lastHunger != m_hunger)
		m_Inventory.shouldUpdateIcons();

	m_saturationLevel += saturation;
	if (m_saturationLevel > m_hunger)
		m_saturationLevel = m_hunger;
}

void Player::loseDurability(int loseDur)
{
	m_Inventory.loseDurability(loseDur);
}

void Player::openCraftingTable()
{
	m_Inventory.showOrHideInventory(InterfaceType::CraftingTable);
}

void Player::reachCertainSpeed(float neededSpeed, float howFastReachIt)
{
	if (velocity.y < neededSpeed - 0.1f)
		velocity.y += howFastReachIt;
	else if (velocity.y > neededSpeed + 0.1f)
		velocity.y -= howFastReachIt;
}

void Player::collide(World &world, const glm::vec3 &vel)
{
	for (int x = position.x - box.dimensions.x; x < position.x + box.dimensions.x; ++x) {
		for (int y = position.y - box.dimensions.y; y < position.y + 0.99f; ++y) {
			for (int z = position.z - box.dimensions.z; z < position.z + box.dimensions.z; ++z) {

				auto block = world.getBlock(x, y, z);

				if (block.getData().isCollidable) {

					bool gotDamage = false;
					if (block.getData().id == BlockId::Cactus && !m_creativeMode) {
						int lastHP = m_hp;
						m_hp -= m_damageMaster.getCactusDamage();

						if (lastHP != m_hp) {
							g_SoundMaster.play(SoundId::ClassicHurt);

							gotDamage = true;
						}
						//@TODO proccess death
						if (m_hp <= 0) {
							g_PlayerInfo.gameState = GameState::DIED;
							g_PlayerInfo.playerState = PlayerState::NOT_MOVING;
						}
					}

					if (vel.y > 0) {
						position.y = y - box.dimensions.y;
						velocity.y = 0;
					}
					// When on ground
					else if (vel.y < 0) {

						if (m_fallDelayTimer.getElapsedTime().asSeconds() > 0.1f) {
							m_fallDelayTimer.restart();

							if (vel.y < -15.0f && !m_creativeMode) {
								int lastHP = m_hp;
								if (m_isSwimming)
									// not reaaly good solution
									m_hp -= m_damageMaster.getFallDamage((m_lastTopPosition - position.y) * 0.5f);
								else
									m_hp -= m_damageMaster.getFallDamage(m_lastTopPosition - position.y);
								if (lastHP != m_hp) {
									processFallSound(world);
									gotDamage = true;
								}
								else {
									processStepSound(world);
								}
								//@TODO proccess death
								if (m_hp <= 0) {
									g_PlayerInfo.gameState = GameState::DIED;
									g_PlayerInfo.playerState = PlayerState::NOT_MOVING;
								}
							}
							else if (vel.y < -3.0f) {
								processStepSound(world);
							}
						}
						m_isOnGround = true;
						m_isReallyOnGround = true;
						position.y = y + box.dimensions.y + 1;
						velocity.y = 0;
					}
					if (gotDamage) {
						m_Inventory.shouldUpdateIcons();
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
				else if (block.getId() == Block_t(BlockId::Water))
					if (!m_isSwimming && position.y < (int)y + 1.94f && velocity.y < -5.2f) {
						g_SoundMaster.play(SoundId::WaterSplashNormal);
					}
			}
		}
	}
}

bool Player::sneakyFallCheck(float dt, World & world)
{
	if (m_isSneaking && m_isReallyOnGround && !m_isFlying) {

		auto newPosition = position + m_acceleration;

		float boxX = box.dimensions.x * 0.9f;
		float boxZ = box.dimensions.z * 0.9f;

		std::array<ChunkBlock, 4> blocks;
		blocks[0] = world.getBlock(newPosition.x - boxX, newPosition.y - 1.5f, newPosition.z - boxZ);
		blocks[1] = world.getBlock(newPosition.x - boxX, newPosition.y - 1.5f, newPosition.z + boxZ);
		blocks[2] = world.getBlock(newPosition.x + boxX, newPosition.y - 1.5f, newPosition.z - boxZ);
		blocks[3] = world.getBlock(newPosition.x + boxX, newPosition.y - 1.5f, newPosition.z + boxZ);

		bool playerDoesNotFall = false;
		for (int i = 0; i < 4; ++i) {
			if (blocks[i].getData().isCollidable)
				playerDoesNotFall = true;
		}

		if (playerDoesNotFall)
			return false;
		else {
			blocks[0] = world.getBlock(position.x - boxX, position.y - 1.5f, position.z - boxZ);
			blocks[1] = world.getBlock(position.x - boxX, position.y - 1.5f, position.z + boxZ);
			blocks[2] = world.getBlock(position.x + boxX, position.y - 1.5f, position.z - boxZ);
			blocks[3] = world.getBlock(position.x + boxX, position.y - 1.5f, position.z + boxZ);

			std::array<bool, 4> existingBlocks{ false };
			for (int i = 0; i < 4; ++i) {
				if (blocks[i].getData().isCollidable)
					existingBlocks[i] = true;
			}

			if (m_acceleration.x < 0) {
				if (m_acceleration.z < 0) {
					if (!existingBlocks[1] && existingBlocks[2])
						velocity.z += m_acceleration.z;
					else if (!existingBlocks[2] && existingBlocks[1])
						velocity.x += m_acceleration.x;
				}
				else if (m_acceleration.z > 0) {
					if (!existingBlocks[0] && existingBlocks[3])
						velocity.z += m_acceleration.z;
					else if (!existingBlocks[3] && existingBlocks[0])
						velocity.x += m_acceleration.x;
				}
			}
			else if (m_acceleration.x > 0) {
				if (m_acceleration.z < 0) {
					if (!existingBlocks[0] && existingBlocks[3])
						velocity.x += m_acceleration.x;
					else if (!existingBlocks[3] && existingBlocks[0])
						velocity.z += m_acceleration.z;
				}
				else if (m_acceleration.z > 0) {
					if (!existingBlocks[1] && existingBlocks[2])
						velocity.x += m_acceleration.x;
					else if (!existingBlocks[2] && existingBlocks[1])
						velocity.z += m_acceleration.z;
				}
			}

			return true;
		}
	}

	return false;
}

void Player::movementStops(float dt, World &world)
{
	static double timePassed = 0.0;
	timePassed += dt;

	if ((getBlockEntityStandsOn(world) == BlockId::Ice) && !m_isFlying) {
		m_isOnIce = true;

		if (m_isSneaking) {
			velocity.x *= 0.95f;
			velocity.z *= 0.95f;
		}
		if (timePassed > 1.0 / 50.0) {
			timePassed = 0.0;

			velocity.x *= 0.96f;
			velocity.z *= 0.96f;
		}
	}
	// jumped from ice
	else if ((!m_isReallyOnGround && !m_isOnWaterSurface && !m_isSwimming)
		&& m_isOnIce && !m_isFlying)
	{
		if (m_isSneaking) {
			velocity.x *= 0.95f;
			velocity.z *= 0.95f;
		}
		if (timePassed > 1.0 / 50.0) {
			timePassed = 0.0;

			velocity.x *= 0.96f;
			velocity.z *= 0.96f;
		}
	}
	else {
		//if (m_isReallyOnGround || m_isOnWaterSurface || m_isSwimming)
		m_isOnIce = false;

		if (m_isFlying) {
			velocity.x *= 0.95f;
			velocity.z *= 0.95f;
		}
		else {
			velocity.x *= 0.95f;
			velocity.z *= 0.95f;
			// hack for low framerates
			if (!m_isMoving) {
				velocity.x *= 0.9f;
				velocity.z *= 0.9f;
			}
		}
	}

	if (m_isFlying) {
		velocity.y *= 0.95f;
	}
}

void Player::movementInWater(World & world)
{
	bool breath = true;
	if (world.getBlock(position.x, position.y, position.z).getData().id == BlockId::Water) {
		//&& position.y <= WATER_LEVEL + 1) {
		m_isSwimming = true;
		static sf::Clock underwaterSoundsTimer;
		// Underwater, bad vision
		//if (position.y - floor(position.y) < 0.3f) {
		if (position.y < WATER_LEVEL + 0.3f) {
			if (g_PlayerInfo.underwater != true) {
				m_Inventory.shouldUpdateIcons();
				g_SoundMaster.playRandom(SoundSet::EnterWaterSet);
			}
			g_SoundMaster.keepPlaying(SoundId::UnderwaterAmbience);
			g_PlayerInfo.underwater = true;
			if (underwaterSoundsTimer.getElapsedTime().asSeconds() > 5.0f) {
				underwaterSoundsTimer.restart();
				g_SoundMaster.playUnderwaterSound();
			}
			if (!m_creativeMode) {
				breath = false;
				if (m_oxygen > 0) {
					int lastOxygen = m_oxygen;
					m_oxygen -= m_damageMaster.getChokeDamage();

					if (lastOxygen != m_oxygen)
						m_Inventory.shouldUpdateIcons();
				}
				else {
					int lastHP = m_hp;
					m_hp -= m_damageMaster.getChokeDamage();

					if (lastHP != m_hp && g_PlayerInfo.gameState != GameState::DIED) {
						g_SoundMaster.playRandom(SoundSet::DrownDamageSet);
						m_Inventory.shouldUpdateIcons();
					}
					//@TODO proccess death
					if (m_hp <= 0) {
						g_PlayerInfo.gameState = GameState::DIED;
						g_PlayerInfo.playerState = PlayerState::NOT_MOVING;
					}
				}
			}
		}
		else {
			if (g_PlayerInfo.underwater) {
				g_SoundMaster.stop(SoundId::UnderwaterAmbience);
				g_SoundMaster.playRandom(SoundSet::ExitWaterSet);
			}
			g_PlayerInfo.underwater = false;
			underwaterSoundsTimer.restart();
		}
	}
	else {
		g_PlayerInfo.underwater = false;
		if (world.getBlock(position.x, position.y - 1, position.z).getData().id == BlockId::Water
			&& position.y <= WATER_LEVEL + 1.93f) {
			m_isSwimming = true;
		}
		else {
			m_isSwimming = false;
		}
	}
	if (breath && m_oxygen < 20) {
		int lastOxygen = m_oxygen;
		m_oxygen += m_damageMaster.breath();

		if (lastOxygen != m_oxygen)
			m_Inventory.shouldUpdateIcons();
	}
}

void Player::movementOnIce()
{
	if (m_isOnIce) {
		if (!m_isFlying) {
			if (!m_isRunning) {
				if (glm::abs(velocity.x) > ICE_SPEED
					|| glm::abs(velocity.z) > ICE_SPEED) {
					m_acceleration.x = 0.0f;
					m_acceleration.z = 0.0f;
				}
			}
			else if (glm::abs(velocity.x) > ICE_SPEED * ICE_SPRINTING_COEF
				|| glm::abs(velocity.z) > ICE_SPEED * ICE_SPRINTING_COEF) {
				m_acceleration.x = 0.0f;
				m_acceleration.z = 0.0f;
			}
		}
		//else {
		//	if (!m_isRunning) {
		//		if (glm::abs(velocity.x) > ICE_SPEED * FLYING_COEF
		//			|| glm::abs(velocity.z) > ICE_SPEED * FLYING_COEF) {
		//			m_acceleration.x = 0.0f;
		//			m_acceleration.z = 0.0f;
		//		}
		//	}
		//	else if (glm::abs(velocity.x) > ICE_SPEED * FLYING_COEF * SPRINT_FLYING_COEF
		//		|| glm::abs(velocity.z) > ICE_SPEED * FLYING_COEF * SPRINT_FLYING_COEF) {
		//		m_acceleration.x = 0.0f;
		//		m_acceleration.z = 0.0f;
		//	}
		//}
	}
}

void Player::processStepSound(World & world)
{
	if (m_isSwimming)
		return;

	makeStepSound(getBlockEntityStandsOn(world));
}

void Player::processFallSound(World & world)
{
	g_SoundMaster.play(SoundId::FallDamage);

	//if (m_isSwimming)
	//	return;
	//
	//makeFallSound(getBlockEntityStandsOn(world));
}

void Player::keyboardInput(Keyboard &keyboard, World &world)
{
	if (!m_isSwimming && isEntityOnWaterSurface(world))
		m_isOnWaterSurface = true;
	else
		m_isOnWaterSurface = false;

	if (!g_PlayerInfo.canMove) {
		g_PlayerInfo.playerState = PlayerState::NOT_MOVING;
		return;
	}

	m_isRunning = false;
	m_isMoving = false;
	m_isDivnigDown = false;

	if (keyboard.isKeyDown(sf::Keyboard::LShift)) {
		m_isSneaking = true;
		m_sneakingCamera.sneak();
		if (m_isFlying)
			m_acceleration.y -= 0.6f;
	}
	else {
		m_isSneaking = false;
		m_sneakingCamera.stand();
	}
	if (keyboard.isKeyDown(sf::Keyboard::W)
		&& !keyboard.isKeyDown(sf::Keyboard::S)) {
		m_isMoving = true;

		float speed = WALKING_SPEED;
		if (m_isFlying) 
			speed *= FLYING_COEF;

		if (keyboard.isKeyDown(sf::Keyboard::LControl)) {
			if (m_isFlying) {
				m_isRunning = true;
				m_movementFOV.sprint();
				speed *= SPRINT_FLYING_COEF;
			}
			else if (!m_isSneaking && m_hunger > 6) {
				m_isRunning = true;
				m_movementFOV.sprint();
				speed *= SPRINTING_COEF;
			}
		}

		if (m_isSneaking && !m_isFlying)
			speed *= SNEAKING_COEF;
		if ((m_isSwimming || m_isOnWaterSurface) && !m_isFlying)
			speed *= SWIMMING_COEF;
		m_acceleration.x += -glm::cos(glm::radians(rotation.y + 90)) * speed;
		m_acceleration.z += -glm::sin(glm::radians(rotation.y + 90)) * speed;
	}



	//m_isMoving = true;
	//float speed = WALKING_SPEED;
	//if (m_isFlying)
	//	speed *= FLYING_COEF;
	//
	//if (m_isFlying) {
	//	m_isRunning = true;
	//	m_movementFOV.sprint();
	//	speed *= SPRINT_FLYING_COEF;
	//}
	//else if (!m_isSneaking && m_hunger > 6) {
	//	m_isRunning = true;
	//	m_movementFOV.sprint();
	//	speed *= SPRINTING_COEF;
	//}
	//if (m_isSneaking && !m_isFlying)
	//	speed *= SNEAKING_COEF;
	//if ((m_isSwimming || m_isOnWaterSurface) && !m_isFlying)
	//	speed *= SWIMMING_COEF;
	//m_acceleration.x += -glm::cos(glm::radians(rotation.y + 90)) * speed;
	//m_acceleration.z += -glm::sin(glm::radians(rotation.y + 90)) * speed;



	if (!m_isRunning)
		m_movementFOV.walk();
	if (keyboard.isKeyDown(sf::Keyboard::S)
		&& !keyboard.isKeyDown(sf::Keyboard::W)) {
		m_isMoving = true;
		float speed = WALKING_SPEED;
		if (m_isFlying)
			speed *= FLYING_COEF;
		if (m_isSneaking && !m_isFlying)
			speed *= SNEAKING_COEF;
		if (!m_isFlying && (m_isSwimming || m_isOnWaterSurface))
			speed *= SWIMMING_COEF;
		m_acceleration.x += glm::cos(glm::radians(rotation.y + 90)) * speed;
		m_acceleration.z += glm::sin(glm::radians(rotation.y + 90)) * speed;
	}
	if (keyboard.isKeyDown(sf::Keyboard::A)
		&& !keyboard.isKeyDown(sf::Keyboard::D)) {
		m_isMoving = true;
		float speed = WALKING_SPEED;
		if (m_isFlying)
			speed *= FLYING_COEF;
		if (m_isSneaking && !m_isFlying)
			speed *= SNEAKING_COEF;
		if (!m_isFlying && (m_isSwimming || m_isOnWaterSurface))
			speed *= SWIMMING_COEF;
		m_acceleration.x += -glm::cos(glm::radians(rotation.y)) * speed;
		m_acceleration.z += -glm::sin(glm::radians(rotation.y)) * speed;
	}
	if (keyboard.isKeyDown(sf::Keyboard::D)
		&& !keyboard.isKeyDown(sf::Keyboard::A)) {
		m_isMoving = true;
		float speed = WALKING_SPEED;
		if (m_isFlying)
			speed *= FLYING_COEF;
		if (m_isSneaking && !m_isFlying)
			speed *= SNEAKING_COEF;
		if (!m_isFlying && (m_isSwimming || m_isOnWaterSurface))
			speed *= SWIMMING_COEF;
		m_acceleration.x += glm::cos(glm::radians(rotation.y)) * speed;
		m_acceleration.z += glm::sin(glm::radians(rotation.y)) * speed;
	}

	if (keyboard.isKeyDown(sf::Keyboard::Space)) {
		if (m_isFlying)
			m_acceleration.y += 0.6f;
		else {
			jump(world);
		}
	}
	else if (m_isSneaking && !m_isFlying) {
		if (m_isSwimming)
			m_isDivnigDown = true;
	}

	movementOnIce();

	bool makeStep = false;
	if (m_isMoving && !m_isFlying && m_isReallyOnGround) {
		if (m_isSneaking) {
			g_PlayerInfo.playerState = PlayerState::SNEAKING;
		}
		else if (m_isRunning) {
			g_PlayerInfo.playerState = PlayerState::SPRINTING;
			if (m_isReallyOnGround)
				m_cameraShaking.sprint(makeStep);
		}
		else {
			g_PlayerInfo.playerState = PlayerState::WALKING;
			if (m_isReallyOnGround)
				m_cameraShaking.walk(makeStep);
		}
	}
	else {
		g_PlayerInfo.playerState = PlayerState::NOT_MOVING;
	}

	if (makeStep)
		processStepSound(world);
}

void Player::mouseInput(const sf::RenderWindow &window)
{
	if (g_PlayerInfo.gameState == GameState::NOT_STARTED) {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)
			|| sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			g_PlayerInfo.gameState = GameState::PLAYING;
			g_PlayerInfo.darkScreen = false;
		}
		if (g_PlayerInfo.gameState == GameState::NOT_STARTED)
			return;
	}

	static sf::Vector2i lastMousePosition = sf::Mouse::getPosition(window);

	static bool useMouse = true;
	static ToggleKey useMouseKey(sf::Keyboard::L);
	if (useMouseKey.isKeyPressed()) {
		useMouse = !useMouse;
	}

	if (!useMouse || g_PlayerInfo.inventoryCursor) {
		lastMousePosition = sf::Mouse::getPosition(window);
		return;
	}

	auto change = sf::Mouse::getPosition() - lastMousePosition;

	rotation.y += change.x * 0.01f * g_Config.mouseSensitivity;
	rotation.x += change.y * 0.01f * g_Config.mouseSensitivity;

	// @TODO fix this
	// in windowed mode the game detects strange mouse movements on my computer
	if (!g_Config.isFullscreen) {
		static bool f = 0;
		static float rotationYFix;
		static float rotationXFix;
		if (f == 0) {
			f = 1;
			rotationYFix = -rotation.y;
			rotationXFix = -rotation.x;
		}
		rotation.y += rotationYFix;
		rotation.x += rotationXFix;
	}

	// if degree > BOUND then the ray which detects the block player aiming at, doesn't work correctly
	static float const BOUND = 87.1f;
	if (rotation.x > BOUND)
		rotation.x = BOUND;
	else if (rotation.x < -BOUND)
		rotation.x = -BOUND;

	if (rotation.y > 360.0f)
		rotation.y = 0.0f;
	else if (rotation.y < 0.0f)
		rotation.y = 360.0f;

	static sf::Vector2i center = {
		g_RenderSettings.resolutionX >> 1,
		g_RenderSettings.resolutionY >> 1
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

void Player::jump(World &world)
{
	if (m_isSwimming) {
		m_isOnGround = false;
		m_isReallyOnGround = false;
		if (velocity.y > -4.6)
			velocity.y = SWIMMING_VERTICAL_SPEED;
	}
	else if (m_isOnGround) {
		if (m_jumpDelayTimer.getElapsedTime().asSeconds() < 0.3f)
			return;
		m_jumpDelayTimer.restart();

		m_isOnGround = false;
		m_isReallyOnGround = false;
		m_acceleration.y += 10.0f;

		if (m_isRunning)
			m_exhaustionLevel += 0.2f
			/ 2;
		else
			m_exhaustionLevel += 0.05f
			/ 2;
	}
}

void Player::drawGUI(RenderMaster &master)
{
	std::ostringstream stream;
	stream
		<< " <F3> Hide debug info"
		<< "\n"
		<< "\n XYZ:  " <<
		(int)position.x << " / " <<
		(int)position.y << " / " <<
		(int)position.z
		<< "\n Chunk:  " <<
		(int)position.x % CHUNK_SIZE << " / " <<
		(int)position.y % CHUNK_SIZE << " / " <<
		(int)position.z % CHUNK_SIZE
		<< std::boolalpha
		<< "\n <F4>   Fog - " << g_Info.fog
		<< "\n <F5>   Weather - " << g_Info.weather
		//<< "\n <LCtrl +/->  Render distance - " << g_Config.renderDistance
		<< "\n <+/-> Time : ";
	if (g_Info.dayTime >= 18000)
		stream << g_Info.dayTime / 1000 - 18 << " AM";
	else if (g_Info.dayTime < 6000)
		stream << g_Info.dayTime / 1000 + 6 << " AM";
	else
		stream << g_Info.dayTime / 1000 - 6 << " PM";

	stream
		<< "\n <Tab> Creative mode - " << m_creativeMode
		<< "\n <F>    Flying (in creative mode) - " << m_isFlying
		<< "\n <P>    Post processing - " << g_Config.postProcess;

	m_playerInfo.setString(stream.str());

	master.drawSFML(m_playerInfo);
}

void Player::parametersUpdate()
{
	m_Inventory.updateIcons();
}

void Player::drawInventory(RenderMaster & master)
{
	m_Inventory.draw(master);
}

void Player::setDroppedItemsManager(DroppedItemsManager * manager)
{
	m_Inventory.setDroppedItemsManager(manager);
}
