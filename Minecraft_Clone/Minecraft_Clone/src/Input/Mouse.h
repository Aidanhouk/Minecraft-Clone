#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>

#include <array>

enum class ButtonState
{
	NOT_PRESSED = 0,
	PRESSED = 1,
	PRESSED_AND_USED = 2,
};

class Mouse {
public:
	Mouse();

	void update(sf::Event& e);

	bool isButtonDown(sf::Mouse::Button button) const;
	bool toggle(sf::Mouse::Button button);
	bool isButtonReleased(sf::Mouse::Button button);
private:
	std::array<ButtonState, sf::Mouse::ButtonCount> m_buttons;
	sf::Mouse::Button m_recentlyReleased;
};