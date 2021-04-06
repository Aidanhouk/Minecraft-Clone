#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

#include <memory>
#include <vector>

#include "Renderer/RenderMaster.h"
#include "States/StateBase.h"

#include "Camera.h"
#include "Context.h"
#include "GlobalInfo.h"

class Application {

  public:
    Application(const Config &config);

    void runLoop();

    template <typename T, typename... Args>
	void pushState(Args &&... args);

    void popState();

    Camera &getCamera()
    {
        return m_camera;
    }

    const sf::RenderWindow &getWindow() const
    {
        return m_context.window;
    }

    void turnOffMouse();
    void turnOnMouse();

	// fix this later
	void setPlayer(Player *player) { m_pPlayer = player; }
  private:
    void handleEvents();

    std::vector<std::unique_ptr<StateBase>> m_states;

    Context m_context;
    RenderMaster m_masterRenderer;
    Camera m_camera;

	Player *m_pPlayer;

    bool m_isPopState = false;
};

#endif // APPLICATION_H_INCLUDED