#include "Application.h"

#include <iostream>

#include "States/PlayingState.h"
#include "World/Block/BlockDatabase.h"

Application::Application(const Config &config)
    : m_context(config)
    , m_camera(config)
{
    BlockDatabase::get();
    pushState<StatePlaying>(*this, config);

	g_Config = config;
}

void Application::runLoop()
{
    sf::Clock dtTimer;
    sf::Clock dt;

    sf::Time m;

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while (m_context.window.isOpen() && !m_states.empty()) {
        auto deltaTime = dtTimer.restart();
		g_info.deltaTime = deltaTime.asSeconds();
        auto &state = *m_states.back();

        state.handleInput();
        state.update(deltaTime.asSeconds());
        m_camera.update();

        state.render(m_masterRenderer);
        m_masterRenderer.finishRender(m_context.window, m_camera);

        handleEvents();
        if (m_isPopState) {
            m_isPopState = false;
            m_states.pop_back();
        }

        m = dt.restart();
		g_info.elapsedTime += m.asSeconds();
    }
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
                    case sf::Keyboard::Escape:
                        m_context.window.close();
                        break;

                    default:
                        break;
                }
                break;

			case sf::Event::MouseWheelMoved:
				m_pPlayer->mouseScrollEvent(e.mouseWheel.delta);
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