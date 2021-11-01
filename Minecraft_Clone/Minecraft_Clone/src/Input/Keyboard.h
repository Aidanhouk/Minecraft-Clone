#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <array>

enum class KeyState
{
	NOT_PRESSED = 0,
	PRESSED = 1,
	PRESSED_AND_USED = 2,
};

class Keyboard {
public:
	Keyboard();
	
	void update(sf::Event& e);
	
	bool isKeyDown(sf::Keyboard::Key key) const;
	bool toggle(sf::Keyboard::Key key);
	bool isKeyReleased(sf::Keyboard::Key key);
private:
	std::array<KeyState, sf::Keyboard::KeyCount> m_keys;
	sf::Keyboard::Key m_recentlyReleased;
};