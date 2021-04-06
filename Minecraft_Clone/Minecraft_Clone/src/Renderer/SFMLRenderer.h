#ifndef SFMLRENDERER_H_INCLUDED
#define SFMLRENDERER_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <vector>

class SFMLRenderer {
public:
    void add(const sf::Drawable &drawable);
	void addOverInterface(const sf::Drawable &drawable);

	void render(sf::RenderWindow &window);
	void renderOverInterface(sf::RenderWindow &window);

private:
	std::vector<const sf::Drawable *> m_draws;
	std::vector<const sf::Drawable *> m_drawsOverInterface;
};

#endif // SFMLRENDERER_H_INCLUDED
