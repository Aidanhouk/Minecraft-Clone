#pragma once

#include <SFML/System.hpp>

class DamageMaster
{
public:
	DamageMaster();

	int getFallDamage(float acceleration);
	int getCactusDamage();
	int breath();
	int getChokeDamage();
private:
	sf::Clock m_CactusDamageClock;
	sf::Clock m_BreathClock;
	sf::Clock m_UnderwaterClock;
};