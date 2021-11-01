#pragma once

#include <SFML/Graphics.hpp>

#include "Config.h"

extern sf::RenderWindow* g_Window;

struct Context
{
	Context(const Config& config);

	sf::RenderWindow window;
};
