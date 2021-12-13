#include "PlayingState.h"

#include "../Application.h"
#include "../Maths/Ray.h"
#include "../Renderer/RenderMaster.h"
#include "../World/Event/PlayerDigEvent.h"
#include "Maths/glm.h"
#include "Player/PlayerInfo.h"
#include "World/Generation/Biome/Biome.h"
#include "Item/Material.h"
#include "World/Block/BlockDatabase.h"
#include "Audio/SoundMaster.h"
#include "Audio/SoundFunctions.h"

#include <iostream>

std::shared_ptr<SkyManager> m_sky;

StatePlaying::StatePlaying(Application &app, const Config &config)
	: StateBase(app)
	, m_player(config, app)
	, m_world(app.getCamera(), config, m_player)
{
	app.getCamera().hookEntity(m_player);

	setTextSettings(app);

	m_chTexture.loadFromFile("Res/Textures/Interface/crosshair.png");
	m_crosshair.setTexture(&m_chTexture);
	m_crosshair.setSize({ 21, 21 });
	m_crosshair.setOrigin(m_crosshair.getGlobalBounds().width / 2,
		m_crosshair.getGlobalBounds().height / 2);
	m_crosshair.setPosition(app.getWindow().getSize().x / 2,
		app.getWindow().getSize().y / 2);

	m_tickManager = std::make_unique<TickManager>();
	m_tickThread = std::make_unique<std::thread>(std::bind(&TickManager::run, m_tickManager.get()));

	m_sky = std::make_unique<SkyManager>();
	m_tickManager->add(m_sky);

	m_player.setDroppedItemsManager(&m_world.getDroppedItemsManager());

	m_placeBlockTimer.restart();
}

void StatePlaying::setTextSettings(Application &app)
{
	m_font.loadFromFile("Res/Fonts/MinecraftRegular.otf");

	m_startText.setFont(m_font);
	m_startText.setCharacterSize(app.getWindow().getSize().x / 25);
	m_startText.setString("Click mouse to start");
	m_startText.setOrigin(
		m_startText.getGlobalBounds().width / 2,
		m_startText.getGlobalBounds().height / 2
	);
	m_startText.setPosition(
		app.getWindow().getSize().x / 2,
		app.getWindow().getSize().y / 5
	);

	m_pauseText.setFont(m_font);
	m_pauseText.setCharacterSize(app.getWindow().getSize().x / 35);
	m_pauseText.setString("\t   PAUSE\n\Enter to continue\n  Escape to exit");
	m_pauseText.setOrigin(
		m_pauseText.getGlobalBounds().width / 2,
		m_pauseText.getGlobalBounds().height / 2
	);
	m_pauseText.setPosition(
		app.getWindow().getSize().x / 2,
		app.getWindow().getSize().y / 5
	);

	m_deathText.setFont(m_font);
	m_deathText.setFillColor(sf::Color::Red);
	m_deathText.setCharacterSize(app.getWindow().getSize().x / 25);
	m_deathText.setString("YOU DIED");
	m_deathText.setOrigin(
		m_deathText.getGlobalBounds().width / 2,
		m_deathText.getGlobalBounds().height / 2
	);
	m_deathText.setPosition(
		app.getWindow().getSize().x / 2,
		app.getWindow().getSize().y / 5
	);
}

void StatePlaying::handleEvent(sf::Event e)
{
	m_keyboard.update(e);
	m_mouse.update(e);
}

StatePlaying::~StatePlaying()
{
	m_tickThread->join();
}

void StatePlaying::handleInput()
{
	if (g_PlayerInfo.gameState == GameState::DIED)
		return;

	m_player.handleInput(m_pApplication->getWindow(), m_mouse, m_keyboard, m_hand, m_world);

	if (g_PlayerInfo.inventoryCursor)
		return;

	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
	//	if (m_keyboard.toggle(sf::Keyboard::Add)) {
	//		++g_Config.renderDistance;
	//		m_world.reloadChunks();
	//	}
	//	if (m_keyboard.toggle(sf::Keyboard::Subtract)) {
	//		--g_Config.renderDistance;
	//		if (g_Config.renderDistance < 4)
	//			g_Config.renderDistance = 4;
	//		m_world.reloadChunks();
	//	}
	//}
	if (m_keyboard.toggle(sf::Keyboard::C)) {
		m_world.reloadChunks();
	}

	glm::vec3 lastPosition{ 0 };

	static sf::Clock eatingTimer;
	BlockId heldItemId = m_player.getHeldItems().getBlockId();
	ChunkBlock heldItem(heldItemId);

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Right)) {

		ChunkBlock block;
		Ray ray({ m_player.position.x, m_player.position.y + g_PlayerInfo.cameraPosition, m_player.position.z }
		, m_player.rotation);

		for (;
			ray.getLength() < 5;
			ray.step(0.05f)) {

			int x = (int)ray.getEnd().x;
			int y = (int)ray.getEnd().y;
			int z = (int)ray.getEnd().z;

			block = m_world.getBlock(x, y, z);

			// found block
			if (block != 0 && block.getData().id != BlockId::Water) {
				//g_PlayerInfo.delineatedBlock = { 0, -1, 0 };
				break;
			}
			lastPosition = ray.getEnd();
		}
		if (block != 0 && block.getData().id != BlockId::Water) {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				m_hand.leftMouseHold();
				m_hand.blockHit();

				eatingTimer.restart();
				m_hand.stopEating();

				if (m_player.isInCreativeMove()) {
					m_blockBreaker._break(m_player.getHeldItems().getBlockId(), ray.getEnd(), 0.0f);

					static sf::Clock creativeMoveBreak;
					if (creativeMoveBreak.getElapsedTime().asSeconds() >= 0.21f) {
						creativeMoveBreak.restart();
						m_world.addEvent<PlayerDigEvent>(sf::Mouse::Left, ray.getEnd(), m_player, m_hand);
					}
				}
				else {
					if (m_makeHitSound) {
						makeHitSound(block.getData().id);
						m_makeHitSound = false;
					}
					float hardness = block.getData().hardness;
					ChunkBlock heldItem(m_player.getHeldItems().getBlockId());

					switch (block.getData().toolToMine)
					{
					case ToolToMine::None:
						break;
					case ToolToMine::Pickaxe:
						if (BlockDatabase::get().isPickaxe(m_player.getHeldItems().getBlockId())) {
							if (BlockDatabase::get().canPickaxeMine(heldItemId, block.getData().id))
								hardness /= heldItem.getData().effieciencyCoef;
						}
						break;
					case ToolToMine::Shovel:
						if (BlockDatabase::get().isShovel(m_player.getHeldItems().getBlockId()))
							hardness /= heldItem.getData().effieciencyCoef;
						break;
					case ToolToMine::Axe:
						if (BlockDatabase::get().isAxe(m_player.getHeldItems().getBlockId()))
							hardness /= heldItem.getData().effieciencyCoef;
						break;
					}
					// if block has been broken
					if (m_blockBreaker._break(m_player.getHeldItems().getBlockId(), ray.getEnd(), hardness)) {
						m_world.addEvent<PlayerDigEvent>(sf::Mouse::Left, ray.getEnd(), m_player, m_hand);
						m_player.loseDurability(1);
					}
				}
			}
			else { // right mouse clicked
				m_hand.leftMouseUnhold();
				m_blockBreaker.stopBreaking();
			
				if (m_placeBlockTimer.getElapsedTime().asSeconds() >= 0.21f) {
					auto & heldItemMaterial = Material::toMaterial(heldItemId);
					if (block.getData().id == BlockId::CraftingTable &&
						!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
						m_hand.swing();
						m_player.openCraftingTable();
					}
					else if (m_player.getHeldItems().getNumInStack() != 0 && heldItemMaterial.isBlock) {
						// Not placing block on player's position
						if (!(
							m_player.position.x < ceil(lastPosition.x) + m_player.box.dimensions.x	&&
							m_player.position.x > floor(lastPosition.x) - m_player.box.dimensions.x &&
							m_player.position.z < ceil(lastPosition.z) + m_player.box.dimensions.z &&
							m_player.position.z > floor(lastPosition.z) - m_player.box.dimensions.z &&
							m_player.position.y < ceil(lastPosition.y) + m_player.box.dimensions.y &&
							m_player.position.y > floor(lastPosition.y) - m_player.box.dimensions.y
							) ||
							!heldItem.getData().isCollidable
							) {
							m_world.addEvent<PlayerDigEvent>(sf::Mouse::Right, lastPosition, m_player, m_hand);
							m_placeBlockTimer.restart();
						}
					}
				}
			}
		}
		// if block in front of player wasn't found
		else {
			m_blockBreaker.stopBreaking();

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				// @TODO
				// Hit enteties and deal damage here
				m_hand.swing();
				m_hand.leftMouseHold();

				eatingTimer.restart();
				m_hand.stopEating();
			}
			else {
				m_hand.leftMouseUnhold();
			}
		}
	}
	else {
		m_hand.leftMouseUnhold();
		m_blockBreaker.stopBreaking();
	}

	static sf::Clock eatingPause;
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && !sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

		if (heldItem.getData().itemType == ItemType::Food &&
			m_player.canEat() && !m_player.isRunning() &&
			eatingPause.getElapsedTime().asSeconds() > 0.15f) {

			m_hand.rightMouseHold();
			m_hand.eat();

			// if player finished eating
			if (eatingTimer.getElapsedTime().asSeconds() >= 1.61f) {
				g_SoundMaster.play(SoundId::Burp);
				m_player.eatFood(heldItem.getData().hunger, heldItem.getData().saturation);
				m_player.removeHeldItem(1);

				eatingTimer.restart();
				m_hand.stopEating();

				eatingPause.restart();
			}
		}
		else {
			eatingTimer.restart();
			m_hand.stopEating();
		}
	}
	else {
		m_hand.rightMouseUnhold();
		eatingTimer.restart();
		m_hand.stopEating();
	}



	for (Ray ray({ m_player.position.x, m_player.position.y + g_PlayerInfo.cameraPosition, m_player.position.z }
	, m_player.rotation);
		ray.getLength() < 5;
		ray.step(0.05f)) {
		int x = (int)ray.getEnd().x;
		int y = (int)ray.getEnd().y;
		int z = (int)ray.getEnd().z;

		auto block = m_world.getBlock(x, y, z);

		if (block != 0 && block.getData().id != BlockId::Water) {
			g_PlayerInfo.delineatedBlock = { x, y, z };
			break;
		}
		else {
			g_PlayerInfo.delineatedBlock = { 0, -1, 0 };
		}

		lastPosition = ray.getEnd();
	}
}

void StatePlaying::update(float deltaTime)
{
	m_fpsCounter.update();
	m_player.update(deltaTime, m_world);
	m_world.update(m_player, deltaTime);
	m_makeHitSound = m_hand.update(m_player.getHeldItems().getBlockId());

	if (g_Info.weather) {
		static PrecipitationType precipitation;
	
		BiomeId biomeId = m_world.getBiomeId(m_player.position.x, m_player.position.z);
		switch (biomeId)
		{
		case BiomeId::ValleyBiome:
		case BiomeId::ForestBiome:
		case BiomeId::OceanBiome:
			precipitation = PrecipitationType::RAIN;
			break;
		case BiomeId::TundraBiome:
		case BiomeId::MountainBiome:
			precipitation = PrecipitationType::SNOW;
			break;
		case BiomeId::DesertBiome:
		default:
			precipitation = PrecipitationType::NONE;
			break;
		}
	
		m_sky->update(m_player.position, precipitation);
	}
	else {
		m_sky->update(m_player.position, PrecipitationType::NONE);
	}
}

void StatePlaying::render(RenderMaster &renderer)
{
	if (g_PlayerInfo.FPS_HUD) {
		m_fpsCounter.draw(renderer);
		m_player.drawGUI(renderer);
	}

	switch (g_PlayerInfo.gameState)
	{
	case GameState::NOT_STARTED:
		renderer.drawSFML(m_startText);
		break;
	case GameState::PAUSED:
		renderer.drawSFML(m_pauseText);
		break;
	case GameState::DIED:
		renderer.drawSFML(m_deathText);
		break;
	default:
		break;
	}

	renderer.drawSFML(m_crosshair);

	m_player.drawInventory(renderer);

	m_hand.drawHand(renderer);

	renderer.drawBreakingBlock();

	m_world.renderWorld(renderer, m_pApplication->getCamera());
}

void StatePlaying::onOpen()
{
	m_pApplication->turnOffMouse();
}
