#pragma once

#include <SFML/System.hpp>

class ScreenDarkening
{
public:
	void makeScreenLighter();
	void makeScreenDarker();
private:
	sf::Clock m_Clock;
};