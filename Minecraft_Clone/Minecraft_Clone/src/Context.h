#pragma once

#include <SFML/Graphics.hpp>

#include "UserSettings/Config.h"

extern sf::RenderWindow* g_Window;

struct Context
{
	Context(const Config& config);

	sf::RenderWindow window;
};
