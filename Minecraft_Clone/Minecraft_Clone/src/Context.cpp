#include "Context.h"

#include "RenderSettings.h"
#include "ShaderData.h"
#include "GL/glew.h"

#include <iostream>

sf::RenderWindow* g_Window;

Context::Context(const Config &config)
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 0;
	settings.majorVersion = 3;
	settings.minorVersion = 3;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	
	if (config.isFullscreen) {
		if (config.customResolution) {
			sf::VideoMode winMode(config.windowX, config.windowY);
			window.create(winMode, "Minecraft-Clone", sf::Style::Fullscreen, settings);
			g_RenderSettings.resolutionX = config.windowX;
			g_RenderSettings.resolutionY = config.windowY;
		}
		else {
			window.create(sf::VideoMode::getDesktopMode(), "Minecraft-Clone", sf::Style::Fullscreen, settings);
			g_RenderSettings.resolutionX = sf::VideoMode::getDesktopMode().width;
			g_RenderSettings.resolutionY = sf::VideoMode::getDesktopMode().height;
		}
	}
	else {
		if (config.customResolution) {
			sf::VideoMode winMode(config.windowX, config.windowY);
			window.create(winMode, "Minecraft-Clone", sf::Style::Close, settings);
			g_RenderSettings.resolutionX = config.windowX;
			g_RenderSettings.resolutionY = config.windowY;
		}
		else {
			window.create(sf::VideoMode::getDesktopMode(), "Minecraft-Clone", sf::Style::Close, settings);
			g_RenderSettings.resolutionX = sf::VideoMode::getDesktopMode().width;
			g_RenderSettings.resolutionY = sf::VideoMode::getDesktopMode().height;
		}
	}

	if (glewInit() != GLEW_OK) {
		std::cout << "Unable to load OpenGL libs.\n";
		exit(-1);
	}

	if (GL_VERSION < 3) {
		std::cout << "Your system does not support the correct OpenGL Version.\n"
			<< "Minimum version required: 3. Your version: " << GL_VERSION
			<< "\n";
		exit(-1);
	}

	g_Window = &window;

	glViewport(0, 0, window.getSize().x, window.getSize().y);

    glCullFace(GL_BACK);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth(4.0f * g_RenderSettings.resolutionX / 2560.0f);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}
