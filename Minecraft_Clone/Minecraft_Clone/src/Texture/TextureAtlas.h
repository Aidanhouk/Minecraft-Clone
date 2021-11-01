#pragma once

#include "BasicTexture.h"

#include <vector>

struct Pixel
{
	float r;
	float g;
	float b;
	float a;
};

class TextureAtlas : public BasicTexture {
public:
	TextureAtlas(const std::string &textureFileName, unsigned int imageSize, unsigned int individualTextureSize);

	void createAtlasPixelsVector();

	void getTextureCoords(std::array<GLfloat, 8>& texCoords, const sf::Vector2i &coords);
	std::vector<Pixel*>& getIndivTexturePixels(const sf::Vector2i &coords);

	const sf::Image & getAtlasImage() const { return m_textureAtlasImage; }
	int getAtlasSize() const { return m_imageSize; }
	int getIndivTextureSize() const { return m_individualTextureSize; }
private:
	sf::Image m_textureAtlasImage;
	int m_imageSize;
	int m_individualTextureSize;
	std::vector<Pixel> m_atlasPixels;
	std::vector<Pixel*> m_indivTexturePixels;
	sf::Vector2i m_lastTexCoords;
};