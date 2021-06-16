#include "DamageMaster.h"

DamageMaster::DamageMaster()
{
	m_FallDamageClock.restart();
	m_CactusDamageClock.restart();
	m_BreathClock.restart();
	m_UnderwaterClock.restart();
}

int DamageMaster::getFallDamage(float acceleration)
{
	if (m_FallDamageClock.getElapsedTime().asSeconds() > 0.1f) {
		m_FallDamageClock.restart();
		int damage = -(int)acceleration - 15;
		damage *= damage * 0.02;
		return damage;
	}
	return 0;
}

int DamageMaster::getCactusDamage()
{
	if (m_CactusDamageClock.getElapsedTime().asSeconds() > 1.5f) {
		m_CactusDamageClock.restart();
		return 1;
	}
	return 0;
}

int DamageMaster::breath()
{
	if (m_BreathClock.getElapsedTime().asSeconds() > 0.333f) {
		m_BreathClock.restart();
		return 1;
	}
	else
		return 0;
}

int DamageMaster::getChokeDamage()
{
	if (m_UnderwaterClock.getElapsedTime().asSeconds() > 0.75f) {
		if (m_UnderwaterClock.getElapsedTime().asSeconds() > 0.85f) {
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