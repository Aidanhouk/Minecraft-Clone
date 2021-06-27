#include "TextureAtlas.h"
#include <array>

TextureAtlas::TextureAtlas(const std::string &textureFileName, unsigned int imageSize, unsigned int individualTextureSize)
{
    if (!m_textureAtlasImage.loadFromFile("Res/Textures/" + textureFileName + ".png")) {
        throw std::runtime_error("Unable to open image: " + textureFileName);
    }
    loadFromImage(m_textureAtlasImage);

    m_imageSize = imageSize;
    m_individualTextureSize = individualTextureSize;
}

std::array<GLfloat, 8> TextureAtlas::getTextureCoords(const sf::Vector2i &coords)
{
    static const GLfloat TEX_PER_ROW = (GLfloat)m_imageSize / (GLfloat)m_individualTextureSize;
    static const GLfloat INDV_TEX_SIZE = 1.0f / TEX_PER_ROW;
    static const GLfloat PIXEL_SIZE = 1.0f / (float)m_imageSize;

	GLfloat xMin = coords.x * INDV_TEX_SIZE;
	GLfloat yMin = coords.y * INDV_TEX_SIZE;

    GLfloat xMax = xMin + INDV_TEX_SIZE;
    GLfloat yMax = yMin + INDV_TEX_SIZE;

    return {xMax, yMax, xMin, yMax, xMin, yMin, xMax, yMin};
}
