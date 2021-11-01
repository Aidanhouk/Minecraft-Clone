#include "CameraShaking.h"

#include "../PlayerInfo.h"

#include <array>
#include <iostream>

CameraShaking::CameraShaking()
{
	m_Clock.restart();
}

const static std::array<int, 4> rotDirections{ -1, 1, 1, -1 };

void CameraShaking::defaultPosition()
{
	m_shift = 0.0f;

	m_rotationStage = 0;
	g_PlayerInfo.cameraRotation = 0.0f;
}

void CameraShaking::walk(bool &makeStep)
{
	if (m_shift > 1.0f) {
		m_coef *= -1;
		m_shift = 0.0f;

		switch (m_rotationStage)
		{
		case 0:
		case 2:
			makeStep = true;
		case 1:
			++m_rotationStage;
			break;
		case 3:
			m_rotationStage = 0;
		default:
			break;
		}

		//switch (m_rotationStage)
		//{
		//case 1:
		//	makeStep = true;
		//case 0:
		//case 2:
		//	++m_rotationStage;
		//	break;
		//case 3:
		//	makeStep = true;
		//	m_rotationStage = 0;
		//default:
		//	break;
		//}
	}

	if (m_Clock.getElapsedTime().asSeconds() > 0.020f * 1.2f) {
		m_Clock.restart();
		m_shift += 0.1f;
		g_PlayerInfo.cameraPosition += 0.01f * m_coef;
		g_PlayerInfo.cameraRotation += 0.04f * rotDirections[m_rotationStage];
		if (g_PlayerInfo.cameraPosition > 0.72f)
			g_PlayerInfo.cameraPosition = 0.72f;
	}
}

void CameraShaking::sprint(bool &makeStep)
{
	if (m_shift > 1.0f) {
		m_coef *= -1;
		m_shift = 0.0f;

		switch (m_rotationStage)
		{
		case 0:
		case 2:
			makeStep = true;
		case 1:
			++m_rotationStage;
			break;
		case 3:
			m_rotationStage = 0;
		default:
			break;
		}
	}

	if (m_Clock.getElapsedTime().asSeconds() > 0.016f * 1.2f) {
		m_Clock.restart();
		m_shift += 0.1f;
		g_PlayerInfo.cameraPosition += 0.015f * m_coef;
		g_PlayerInfo.cameraRotation += 0.05f * rotDirections[m_rotationStage];
		if (g_PlayerInfo.cameraPosition > 0.72f)
			g_PlayerInfo.cameraPosition = 0.72f;
	}
}
