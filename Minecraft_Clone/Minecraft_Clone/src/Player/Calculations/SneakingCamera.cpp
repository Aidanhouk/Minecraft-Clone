#include "SneakingCamera.h"

#include "../PlayerInfo.h"

SneakingCamera::SneakingCamera()
	:m_maxCamPos{ 0.6f }, m_minCamPos{ 0.4f }
{
	m_Clock.restart();
}

void SneakingCamera::sneak()
{
	if (g_PlayerInfo.cameraPosition <= m_minCamPos)
		return;
	if (m_Clock.getElapsedTime().asSeconds() > 0.02f) {
		g_PlayerInfo.cameraPosition -= 0.04f;
		m_Clock.restart();
	}
}

void SneakingCamera::stand()
{
	if (g_PlayerInfo.cameraPosition >= m_maxCamPos)
		return;
	if (m_Clock.getElapsedTime().asSeconds() > 0.02f) {
		g_PlayerInfo.cameraPosition += 0.04f;
		m_Clock.restart();
	}
}