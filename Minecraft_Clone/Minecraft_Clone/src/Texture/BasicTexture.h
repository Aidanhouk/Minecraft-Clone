#pragma once

#include "GL/glew.h"

#include <SFML/Graphics.hpp>
#include <string>

#include "../Util/NonCopyable.h"

class BasicTexture : public NonCopyable {
public:
	BasicTexture() = default;
	BasicTexture(const std::string &file);
	
	~BasicTexture();
	
	void loadFromImage(const sf::Image &image);
	void loadFromFile(const std::string &file);
	
	void bindTexture() const;
	
	GLuint getTextureId() const { return m_id; }
private:
	GLuint m_id;
};