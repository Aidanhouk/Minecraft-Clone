#pragma once

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
	
	Camera &getCamera() { return m_camera; }
	
	const sf::RenderWindow &getWindow() const { return m_context.window; }
	
	void turnOffMouse();
	void turnOnMouse();
	
private:
	void handleEvents();
	
	std::vector<std::unique_ptr<StateBase>> m_states;
	
	Context m_context;
	RenderMaster m_masterRenderer;
	Camera m_camera;
	
	bool m_isPopState = false;
};