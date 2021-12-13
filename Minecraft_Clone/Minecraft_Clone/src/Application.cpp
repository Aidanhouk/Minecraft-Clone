#include "Application.h"

#include "States/PlayingState.h"
#include "World/Block/BlockDatabase.h"
#include "Audio/SoundMaster.h"
#include "Player/PlayerInfo.h"

#include <iostream>

Application::Application(const Config &config)
    : m_context(config)
    , m_camera(config)
{
    BlockDatabase::get();
    pushState<StatePlaying>(*this, config);

	g_Config = config;
	g_SoundMaster.setVolumeAll(MusicSet::DefaultInGameSet, config.musicVolume);
}

void Application::runLoop()
{
    sf::Clock dtTimer;
    sf::Clock dt;

    sf::Time m;

    while (m_context.window.isOpen() && !m_states.empty()) {

        auto deltaTime = dtTimer.restart();
		g_Info.deltaTime = deltaTime.asSeconds();
        auto &state = *m_states.back();

		if (g_PlayerInfo.gameState != GameState::PAUSED) {
			state.handleInput();
			state.update(deltaTime.asSeconds());
			m_camera.update();
		}

		state.render(m_masterRenderer);
        m_masterRenderer.finishRender(m_context.window, m_camera);

        handleEvents();
        if (m_isPopState) {
            m_isPopState = false;
            m_states.pop_back();
        }

        m = dt.restart();
		g_Info.elapsedTime += m.asSeconds();
    }

	g_SoundMaster.stopEverything();
}

void Application::handleEvents()
{
    sf::Event e;
    while (m_context.window.pollEvent(e)) {
        m_states.back()->handleEvent(e);
        switch (e.type) {
            case sf::Event::Closed:
                m_context.window.close();
                break;

            case sf::Event::KeyPressed:
                switch (e.key.code) {
                    case sf::Keyboard::Enter:
						if (g_PlayerInfo.gameState == GameState::PAUSED) {
							g_PlayerInfo.gameState = GameState::PLAYING;
							g_PlayerInfo.darkScreen = false;
						}
                        break;
					case sf::Keyboard::Escape:
						if (g_PlayerInfo.gameState == GameState::PAUSED ||
							g_PlayerInfo.gameState == GameState::DIED) {
							m_context.window.close();
						}
						else if (!g_PlayerInfo.inventoryCursor) {
							g_PlayerInfo.gameState = GameState::PAUSED;
							g_PlayerInfo.playerState = PlayerState::NOT_MOVING;
							g_PlayerInfo.darkScreen = true;
						}
						break;

                    default:
                        break;
                }
                break;

			case sf::Event::MouseWheelMoved:
				g_PlayerInfo.player->mouseScrollEvent(e.mouseWheel.delta);
				break;

            default:
                break;
        }
    }
}

template<typename T, typename ...Args>
inline void Application::pushState(Args && ...args)
{
	m_states.push_back(std::make_unique<T>(std::forward<Args>(args)...));
	auto &s = m_states.back();
	s->onOpen();
}


void Application::popState()
{
    m_isPopState = true;
}

void Application::turnOffMouse()
{
    m_context.window.setMouseCursorVisible(false);
}

void Application::turnOnMouse()
{
    m_context.window.setMouseCursorVisible(true);
}