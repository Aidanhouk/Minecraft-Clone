#include "Keyboard.h"

Keyboard::Keyboard()
	:m_recentlyReleased{ sf::Keyboard::KeyCount }
{
    std::fill(m_keys.begin(), m_keys.end(), KeyState::NOT_PRESSED);
}

void Keyboard::update(sf::Event& e)
{
    switch (e.type) {
		case sf::Event::KeyPressed:
			if (m_keys[e.key.code] != KeyState::PRESSED_AND_USED)
				m_keys[e.key.code] = KeyState::PRESSED;
			break;
        case sf::Event::KeyReleased:
			m_recentlyReleased = e.key.code;
			m_keys[e.key.code] = KeyState::NOT_PRESSED;
            break;
        default:
            break;
    }
}

bool Keyboard::isKeyDown(sf::Keyboard::Key key) const
{
    return m_keys[key] == KeyState::PRESSED;
}

bool Keyboard::toggle(sf::Keyboard::Key key)
{
	if (m_keys[key] == KeyState::PRESSED) {
		m_keys[key] = KeyState::PRESSED_AND_USED;
		return true;
	}
	else {
		return false;
	}
}

bool Keyboard::isKeyReleased(sf::Keyboard::Key key)
{
	if (m_recentlyReleased == key) {
		m_recentlyReleased = sf::Keyboard::KeyCount;
		return true;
	}
	else {
		return false;
	}
}
