#include "MovementFOV.h"

#include "Config.h"

MovementFOV::MovementFOV(const Config &config)
	: m_MinFOV{ config.fov }
{
	m_MaxFOV = m_MinFOV + 10; // + 13
	if (m_MaxFOV > 115)
		m_MaxFOV = 115;

	m_Clock.restart();
}

void MovementFOV::sprint()
{
	if (g_Config.fov >= m_MaxFOV)
		return;
	if (m_Clock.getElapsedTime().asSeconds() > 0.01f) {
		++g_Config.fov;
		m_Clock.restart();
	}
}

void MovementFOV::walk()
{
	if (g_Config.fov <= m_MinFOV)
		return;
	if (m_Clock.getElapsedTime().asSeconds() > 0.01f) {
		--g_Config.fov;
		m_Clock.restart();
	}
}