#include "DamageMaster.h"

#include <iostream>

DamageMaster::DamageMaster()
{
	m_CactusDamageClock.restart();
	m_BreathClock.restart();
	m_UnderwaterClock.restart();
}

int DamageMaster::getFallDamage(float acceleration)
{
	return std::max(0, (int)acceleration - 3);
}

int DamageMaster::getCactusDamage()
{
	if (m_CactusDamageClock.getElapsedTime().asSeconds() > 0.5f) {
		m_CactusDamageClock.restart();
		return 1;
	}
	return 0;
}

int DamageMaster::breath()
{
	if (m_BreathClock.getElapsedTime().asSeconds() > 0.2f) { // 0.2 in Minecraft
		m_BreathClock.restart();
		return 1;
	}
	else
		return 0;
}

int DamageMaster::getChokeDamage()
{
	if (m_UnderwaterClock.getElapsedTime().asSeconds() > 0.5f) { // 0.5 in Minecraft
		if (m_UnderwaterClock.getElapsedTime().asSeconds() > 0.6f) {
			m_UnderwaterClock.restart();
			return 0;
		}
		else {
			m_UnderwaterClock.restart();
			return 1;
		}
	}
	else
		return 0;
}