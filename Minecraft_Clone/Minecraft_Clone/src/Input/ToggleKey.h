#pragma once

#include <SFML/Graphics.hpp>

class ToggleKey {
  public:
    ToggleKey(sf::Keyboard::Key);

	bool isKeyPressed();
	sf::Keyboard::Key &getKey() { return m_key; }

  private:
    sf::Keyboard::Key m_key;
    sf::Clock m_delayTimer;
};
