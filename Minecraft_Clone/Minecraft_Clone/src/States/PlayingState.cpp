#include "PlayingState.h"

#include "../Application.h"
#include "../Maths/Ray.h"
#include "../Renderer/RenderMaster.h"
#include "../World/Event/PlayerDigEvent.h"
#include "Maths/glm.h"
#include "Player/PlayerInfo.h"
#include "World/Generation/Biome/Biome.h"

#include <iostream>

std::shared_ptr<SkyManager> m_sky;

StatePlaying::StatePlaying(Application &app, const Config &config)
    : StateBase(app)
	, m_player(config, app)
    , m_world(app.getCamera(), config, m_player)
{
    app.getCamera().hookEntity(m_player);

	m_font.loadFromFile("Res/Fonts/MinecraftRegular.otf");

	m_startText.setFont(m_font);
	m_startText.setCharacterSize(app.getWindow().getSize().x / 25);
	m_startText.setString("Click mouse to start");
	m_startText.setOrigin(m_startText.getGlobalBounds().width / 2,
		m_startText.getGlobalBounds().height / 2);
	m_startText.setPosition(app.getWindow().getSize().x / 2,
		app.getWindow().getSize().y / 3);

	m_deathText.setFont(m_font);
	m_deathText.setFillColor(sf::Color::Red);
	m_deathText.setCharacterSize(app.getWindow().getSize().x / 25);
	m_deathText.setString("YOU DIED");
	m_deathText.setOrigin(m_deathText.getGlobalBounds().width / 2,
		m_deathText.getGlobalBounds().height / 2);
	m_deathText.setPosition(app.getWindow().getSize().x / 2,
		app.getWindow().getSize().y / 3);

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

	app.setPlayer(&m_player);
	m_player.setDroppedItemsManager(&m_world.getDroppedItemsManager());

	m_placeBlockTimer.restart();
}

void StatePlaying::handleEvent(sf::Event e)
{
    m_keyboard.update(e);
}

StatePlaying::~StatePlaying()
{
    m_tickThread->join();
}

void StatePlaying::handleInput()
{
	if (p_info.gameState == GameState::DIED)
		return;

	m_player.handleInput(m_pApplication->getWindow(), m_keyboard, m_hand, m_world);

	if (p_info.interfaceCursor)
		return;

	glm::vec3 lastPosition;

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Right)) {

		ChunkBlock block;
		BlockId blockId = BlockId::Air;
		Ray ray({ m_player.position.x, m_player.position.y + 0.57f, m_player.position.z }, m_player.rotation);

		for (;
			ray.getLength() < 5;
			ray.step(0.05f)) {

			int x = (int)ray.getEnd().x;
			int y = (int)ray.getEnd().y;
			int z = (int)ray.getEnd().z;

			block = m_world.getBlock(x, y, z);
			blockId = (BlockId)block.id;

			if (blockId != BlockId::Air && blockId != BlockId::Water) {
				p_info.delineatedBlock = { 0, -1, 0 };
				break;
			}
			lastPosition = ray.getEnd();
		}
		if (blockId != BlockId::Air && blockId != BlockId::Water) {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				m_hand._break();
				/// if the block has been broken
				if (m_blockBreaker._break(ray.getEnd(), block.getData().hardness)) {
					m_world.addEvent<PlayerDigEvent>(sf::Mouse::Left, ray.getEnd(), m_player);
				}
			}
			/// right mouse clicked
			else {
				m_blockBreaker.stopBreaking();
				if (m_placeBlockTimer.getElapsedTime().asSeconds() >= 0.2f) {
					/// There's a bug when you place a block it sometimes replaces another block.\
						This hack doesn't let it happen. Should fix it later
					auto positionToPlaceBlock = m_world.getBlock(lastPosition.x, lastPosition.y, lastPosition.z);
					auto _id = (BlockId)positionToPlaceBlock.id;
					if (_id == BlockId::Air || _id == BlockId::Water) {
						/// Not placing block on player position thus no collision
						if (!(
							m_player.position.x < ceil(lastPosition.x) + 0.3f
							&& m_player.position.x > floor(lastPosition.x) - 0.3f
							&& m_player.position.z < ceil(lastPosition.z) + 0.3f
							&& m_player.position.z > floor(lastPosition.z) - 0.3f
							&& m_player.position.y < ceil(lastPosition.y) + 1
							&& m_player.position.y > floor(lastPosition.y) - 1
							)
							&& m_player.getHeldItems().getNumInStack() != 0
							/// Refactor this later
							|| m_player.getHeldItems().getMaterial().id == Material::Dandelion
							|| m_player.getHeldItems().getMaterial().id == Material::DeadShrub
							|| m_player.getHeldItems().getMaterial().id == Material::Rose
							|| m_player.getHeldItems().getMaterial().id == Material::SugarCane
							|| m_player.getHeldItems().getMaterial().id == Material::TallGrass
							) {
							m_hand.swing();
							m_world.addEvent<PlayerDigEvent>(sf::Mouse::Right, lastPosition, m_player);
							m_placeBlockTimer.restart();
						}
					}
				}
			}
		}
		else {
			m_blockBreaker.stopBreaking();

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				m_hand.swing();
				m_hand.leftMouseHold();
			}
			else { // right mouse clicked
				m_hand.leftMouseUnhold();
			}
		}
	}
	else {
		m_hand.leftMouseUnhold();
		m_blockBreaker.stopBreaking();
	}
	for (Ray ray({ m_player.position.x, m_player.position.y + 0.57f, m_player.position.z }, m_player.rotation);
		ray.getLength() < 5;
		ray.step(0.05f)) {
		int x = (int)ray.getEnd().x;
		int y = (int)ray.getEnd().y;
		int z = (int)ray.getEnd().z;

		auto block = m_world.getBlock(x, y, z);
		auto id = (BlockId)block.id;

		if (id != BlockId::Air && id != BlockId::Water) {
			p_info.delineatedBlock = { x, y, z };
			break;
		}
		else {
			p_info.delineatedBlock = { 0, -1, 0 };
		}

		lastPosition = ray.getEnd();
	}
}

void StatePlaying::update(float deltaTime)
{
    if (m_player.position.x < 0)
        m_player.position.x = 0;
    if (m_player.position.z < 0)
        m_player.position.z = 0;

    m_fpsCounter.update();
    m_player.update(deltaTime, m_world);
    m_world.update(m_player, deltaTime);

	m_sky->update(m_player.position);
}

void StatePlaying::render(RenderMaster &renderer)
{
    static bool drawGUI = true;
    static ToggleKey drawKey(sf::Keyboard::F3);
    if (drawKey.isKeyPressed()) {
        drawGUI = !drawGUI;
    }
    if (drawGUI) {
        m_fpsCounter.draw(renderer);
        m_player.drawGUI(renderer);
    }

	switch (p_info.gameState)
	{
	case GameState::NOT_STARTED:
		renderer.drawSFML(m_startText);
		break;
	case GameState::DIED:
		renderer.drawSFML(m_deathText);
		break;
	default:
		break;
	}

	renderer.drawSFML(m_crosshair);

	m_player.drawInventory(renderer);

	m_hand.setMeshToDraw();
	renderer.setHandModel(m_hand.getModel());

	renderer.drawBreakingBlock();

    m_world.renderWorld(renderer, m_pApplication->getCamera());
}

void StatePlaying::onOpen()
{
    m_pApplication->turnOffMouse();
}