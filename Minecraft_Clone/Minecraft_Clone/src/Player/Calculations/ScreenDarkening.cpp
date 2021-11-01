#include "ScreenDarkening.h"

#include "../PlayerInfo.h"

void ScreenDarkening::makeScreenLighter()
{
	if (m_Clock.getElapsedTime().asSeconds() > 0.01f) {
		g_PlayerInfo.caveLighting += 0.01f;
		if (g_PlayerInfo.caveLighting > 1.0f)
			g_PlayerInfo.caveLighting = 1.0f;

		m_Clock.restart();
	}
}

void ScreenDarkening::makeScreenDarker()
{
	if (m_Clock.getElapsedTime().asSeconds() > 0.01f) {
		g_PlayerInfo.caveLighting -= 0.01f;
		if (g_PlayerInfo.caveLighting < 0.4f)
			g_PlayerInfo.caveLighting = 0.4f;

		m_Clock.restart();
	}
}
