#include "TextureAtlas.h"

#include <array>
#include <iostream>

TextureAtlas::TextureAtlas(const std::string &textureFileName, unsigned int imageSize, unsigned int individualTextureSize)
	:m_lastTexCoords{ -1, -1 }
{
    if (!m_textureAtlasImage.loadFromFile("Res/Textures/" + textureFileName + ".png")) {
        throw std::runtime_error("Unable to open image: " + textureFileName);
    }
    loadFromImage(m_textureAtlasImage);

    m_imageSize = imageSize;
    m_individualTextureSize = individualTextureSize;
	m_indivTexturePixels.reserve(m_individualTextureSize * m_individualTextureSize);
}

// can't do this in constructor
void TextureAtlas::createAtlasPixelsVector()
{
	static bool once = 1;
	if (once == 0)
		return;
	once = 0;

	m_atlasPixels.reserve(
		m_individualTextureSize *
		m_individualTextureSize *
		m_imageSize
	);
	this->bindTexture();
	// this function doesn't add elements using std::vector functions
	// that means vector size is 0 and we better use pointer arithmetic
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, m_atlasPixels.data());
}

void TextureAtlas::getTextureCoords(std::array<GLfloat, 8>& texCoords, const sf::Vector2i &coords)
{
    static const GLfloat TEX_PER_ROW = (GLfloat)m_imageSize / (GLfloat)m_individualTextureSize;
    static const GLfloat INDV_TEX_SIZE = 1.0f / TEX_PER_ROW;
    static const GLfloat PIXEL_SIZE = 1.0f / (float)m_imageSize;

	GLfloat xMin = coords.x * INDV_TEX_SIZE;
	GLfloat yMin = coords.y * INDV_TEX_SIZE;

    GLfloat xMax = xMin + INDV_TEX_SIZE;
    GLfloat yMax = yMin + INDV_TEX_SIZE;

	texCoords[0] = xMin;
	texCoords[1] = yMax;
	texCoords[2] = xMax;
	texCoords[3] = yMax;
	texCoords[4] = xMax;
	texCoords[5] = yMin;
	texCoords[6] = xMin;
	texCoords[7] = yMin;
	//{ xMin, yMax, xMax, yMax, xMax, yMin, xMin, yMin };
}

std::vector<Pixel*>& TextureAtlas::getIndivTexturePixels (const sf::Vector2i & coords)
{
	if (coords == m_lastTexCoords)
		return m_indivTexturePixels;

	m_lastTexCoords = coords;
	m_indivTexturePixels.clear();

	int curPos = 0;
	int indivTexturesInCol = sqrt(m_imageSize);
	int numberOfPixelsInRow = m_individualTextureSize * m_individualTextureSize * indivTexturesInCol;
	curPos += coords.y * numberOfPixelsInRow;
	curPos += coords.x * m_individualTextureSize;

	for (int i = 0; i < m_individualTextureSize; ++i) {
		for (int j = 0; j < m_individualTextureSize; ++j) {
			m_indivTexturePixels.emplace_back(m_atlasPixels.data() + (curPos + j));
		}
		curPos += m_individualTextureSize * indivTexturesInCol;
	}

	return m_indivTexturePixels;
}
