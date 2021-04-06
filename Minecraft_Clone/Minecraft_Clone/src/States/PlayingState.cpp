#include "PlayingState.h"

#include "../Application.h"
#include "../Maths/Ray.h"
#include "../Renderer/RenderMaster.h"
#include "../World/Event/PlayerDigEvent.h"
#include "Maths/glm.h"
#include "Player/PlayerInfo.h"

#include <iostream>

std::shared_ptr<SkyManager> m_sky;

StatePlaying::StatePlaying(Application &app, const Config &config)
    : StateBase(app)
	, m_player(config, app)
    , m_world(app.getCamera(), config, m_player)
{
    app.getCamera().hookEntity(m_player);

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
    m_player.handleInput(m_pApplication->getWindow(), m_keyboard);

	if (p_info.interfaceCursor)
		return;

    static sf::Clock timer;
    glm::vec3 lastPosition;

    for (Ray ray({ m_player.position.x, m_player.position.y + 0.57f, m_player.position.z }, m_player.rotation); // Corrected for camera offset
         ray.getLength() < 5; ray.step(0.05f)) {
        int x = (int)ray.getEnd().x;
		int y = (int)ray.getEnd().y;
		int z = (int)ray.getEnd().z;
		
        auto block = m_world.getBlock(x, y, z);
        auto id = (BlockId)block.id;

        if (id != BlockId::Air && id != BlockId::Water) {
            if (timer.getElapsedTime().asSeconds() > 0.2f) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    timer.restart();
                    m_world.addEvent<PlayerDigEvent>(sf::Mouse::Left, ray.getEnd(), m_player);
                    break;
                }
                else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
					/// There's a bug when you place a block it sometimes replaces another block.\
					This hack doesn't let it happen. Should fix it later
					{
						auto positionToPlaceBlock = m_world.getBlock(lastPosition.x, lastPosition.y, lastPosition.z);
						auto _id = (BlockId)positionToPlaceBlock.id;
						if (!(_id == BlockId::Air || _id == BlockId::Water))
							return;
					}

					/// Not placing block on player position thus no collision
					if (
						m_player.position.x < ceil(lastPosition.x) + 0.3f
						&& m_player.position.x > floor(lastPosition.x) - 0.3f
						&& m_player.position.z < ceil(lastPosition.z) + 0.3f
						&& m_player.position.z > floor(lastPosition.z) - 0.3f
						&& m_player.position.y < ceil(lastPosition.y) + 1
						&& m_player.position.y > floor(lastPosition.y) - 1
						) {
						return;
					}

					timer.restart();
					m_world.addEvent<PlayerDigEvent>(sf::Mouse::Right, lastPosition, m_player);
					return;
                }
            }
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
    m_world.update(m_pApplication->getCamera());

	m_sky->Update(m_player.position);
}

void StatePlaying::render(RenderMaster &renderer)
{
    static sf::Clock dt;

    static bool drawGUI = false;
    static ToggleKey drawKey(sf::Keyboard::F3);

	renderer.drawSFML(m_crosshair);

    if (drawKey.isKeyPressed()) {
        drawGUI = !drawGUI;
    }

    if (drawGUI) {
        m_fpsCounter.draw(renderer);
        m_player.drawGUI(renderer);
    }

	m_player.drawInventory(renderer);

    m_world.renderWorld(renderer, m_pApplication->getCamera());
}

void StatePlaying::onOpen()
{
    m_pApplication->turnOffMouse();
}