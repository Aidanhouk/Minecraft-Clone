#include "Context.h"

#include "RenderSettings.h"
#include "ShaderData.h"
#include "GL/glew.h"

#include <iostream>

sf::RenderWindow* g_window;

Context::Context(const Config &config)
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 0;
	settings.majorVersion = 3;
	settings.minorVersion = 3;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	//settings.attributeFlags = sf::ContextSettings::Core;
	//This is no longer necessary due to the Mac Support update.

	if (config.isFullscreen) {
		window.create(sf::VideoMode::getDesktopMode(), "Minecraft-Clone", sf::Style::Fullscreen, settings);
		g_renderSettings.resolutionX = sf::VideoMode::getDesktopMode().width;
		g_renderSettings.resolutionY = sf::VideoMode::getDesktopMode().height;
	}
	else {
		sf::VideoMode winMode(config.windowX, config.windowY);
		window.create(winMode, "Minecraft-Clone", sf::Style::Close, settings);
		g_renderSettings.resolutionX = config.windowX;
		g_renderSettings.resolutionY = config.windowY;
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

	g_window = &window;

    glViewport(0, 0, window.getSize().x, window.getSize().y);

    glCullFace(GL_BACK);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth(4);
}
