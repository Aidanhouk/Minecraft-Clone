#include "FPSCounter.h"

#include "../Renderer/RenderMaster.h"

#include <iostream>

FPSCounter::FPSCounter()
{
    m_text.move(10, 10);
    m_text.setOutlineThickness(2);

    m_font.loadFromFile("Res/Fonts/MinecraftRegular.otf");
    m_text.setFont(m_font);
    m_text.setCharacterSize(30);
}

void FPSCounter::update()
{
	++m_frameCount;

	if (m_delayTimer.getElapsedTime().asSeconds() > 0.5) {
		m_fps = m_frameCount / m_fpsTimer.restart().asSeconds();
		m_frameCount = 0;
		m_delayTimer.restart();
		//std::cout << m_fps << "fps\n";
	}
}

void FPSCounter::draw(RenderMaster &renderer)
{
    m_text.setString("FPS: " + std::to_string((int)m_fps));
    renderer.drawSFML(m_text);
}
