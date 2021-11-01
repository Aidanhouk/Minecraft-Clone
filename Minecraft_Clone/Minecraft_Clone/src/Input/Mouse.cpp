#include "Mouse.h"

Mouse::Mouse()
	:m_recentlyReleased{ sf::Mouse::ButtonCount }
{
	std::fill(m_buttons.begin(), m_buttons.end(), ButtonState::NOT_PRESSED);
}

void Mouse::update(sf::Event& e)
{
	switch (e.type) {
	case sf::Event::MouseButtonPressed:
		if (m_buttons[e.mouseButton.button] != ButtonState::PRESSED_AND_USED)
			m_buttons[e.mouseButton.button] = ButtonState::PRESSED;
		break;
	case sf::Event::MouseButtonReleased:
		m_recentlyReleased = e.mouseButton.button;
		m_buttons[e.mouseButton.button] = ButtonState::NOT_PRESSED;
		break;
	default:
		break;
	}
}

bool Mouse::isButtonDown(sf::Mouse::Button button) const
{
	return m_buttons[button] == ButtonState::PRESSED;
}

bool Mouse::toggle(sf::Mouse::Button button)
{
	if (m_buttons[button] == ButtonState::PRESSED) {
		m_buttons[button] = ButtonState::PRESSED_AND_USED;
		return true;
	}
	else {
		return false;
	}
}

bool Mouse::isButtonReleased(sf::Mouse::Button button)
{
	if (m_recentlyReleased == button) {
		m_recentlyReleased = sf::Mouse::ButtonCount;
		return true;
	}
	else {
		return false;
	}
}
