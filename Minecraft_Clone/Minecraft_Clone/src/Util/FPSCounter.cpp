#include "FPSCounter.h"

#include "../Renderer/RenderMaster.h"
#include "RenderSettings.h"

#include <iostream>

FPSCounter::FPSCounter()
{
    m_text.move(12 * g_RenderSettings.resolutionX / 2560,
		8 * g_RenderSettings.resolutionX / 2560);
    m_text.setOutlineThickness(2 * g_RenderSettings.resolutionX / 2560);

    m_font.loadFromFile("Res/Fonts/MinecraftRegular.otf");
    m_text.setFont(m_font);
    m_text.setCharacterSize(g_RenderSettings.resolutionX / 85.3f);
}

void FPSCounter::update()
{
	++m_frameCount;

	if (m_delayTimer.getElapsedTime().asSeconds() > 0.5) {
		m_fps = m_frameCount / m_fpsTimer.restart().asSeconds();
		m_frameCount = 0;
		m_delayTimer.restart();
	}
}

void FPSCounter::draw(RenderMaster &renderer)
{
    m_text.setString("FPS: " + std::to_string((int)m_fps));
    renderer.drawSFML(m_text);
}
