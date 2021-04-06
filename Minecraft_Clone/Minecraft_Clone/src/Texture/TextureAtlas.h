#ifndef TEXTUREATLAS_H_INCLUDED
#define TEXTUREATLAS_H_INCLUDED

#include "BasicTexture.h"

class TextureAtlas : public BasicTexture {
public:
    TextureAtlas(const std::string &textureFileName, unsigned int imageSize, unsigned int individualTextureSize);

    std::array<GLfloat, 8> getTextureCoords(const sf::Vector2i &coords);

	sf::Image & getAtlasImage() { return m_textureAtlasImage; }
	int getAtlasSize() const { return m_imageSize; }
	int getIndivTextureSize() const { return m_individualTextureSize; }
private:
	sf::Image m_textureAtlasImage;
    int m_imageSize;
    int m_individualTextureSize;
};

#endif // TEXTUREATLAS_H_INCLUDED
