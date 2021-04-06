#include "ToggleKey.h"

ToggleKey::ToggleKey(sf::Keyboard::Key key)
    : m_key(key)
{
}

bool ToggleKey::isKeyPressed()
{
    if (m_delayTimer.getElapsedTime().asSeconds() > 0.2f) {
        if (sf::Keyboard::isKeyPressed(m_key)) {
            m_delayTimer.restart();
            return true;
        }
    }
    return false;
}