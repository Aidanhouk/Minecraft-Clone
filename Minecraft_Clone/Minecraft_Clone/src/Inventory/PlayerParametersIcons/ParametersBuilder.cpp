#include "ParametersBuilder.h"

#include "../IconsMesh.h"
#include "../Inventory.h"
#include "../IconDatabase.h"

#include "World/Block/ChunkBlock.h"
#include "World/Block/BlockData.h"

#include "Player/Player.h"
#include "Player/PlayerInfo.h"
#include "RenderSettings.h"

#include <iostream>

const std::array<sf::Vector2i, 4> ParametersTexturePositions{
	sf::Vector2i{0, 18}, sf::Vector2i{9, 18}, sf::Vector2i{18, 18}, sf::Vector2i{27, 18},
};

ParametersBuilder::ParametersBuilder(Inventory &inventory, IconsMesh &itemIconsMesh, Player &player)
	: m_pInventory{ &inventory },
	m_pIconsMesh{ &itemIconsMesh },
	m_pPlayer{ &player }
{
}

void ParametersBuilder::buildMesh()
{
	int hp = m_pPlayer->getHP();
	if (hp < 0)
		hp = 0;

	int fullHearts = hp >> 1;
	bool halfHeart = hp % 2;
	int emptyHearts = (20 - hp) >> 1;

	int iconNumber = 0;

	for (int i = 0; i < fullHearts; ++i)
		buildIcon(FullHeart, iconNumber++);
	if (halfHeart)
		buildIcon(HalfHeart, iconNumber++);
	for (int i = 0; i < emptyHearts; ++i)
		buildIcon(EmptyHeart, iconNumber++);

	int oxygen = m_pPlayer->getOxygen();
	if (p_info.underwater || oxygen < 20) {
		int bubbles = oxygen / 2.0f + 0.5f;
		for (int i = 0; i < bubbles; ++i)
			buildIcon(FullBubble, iconNumber++);
	}
}

void ParametersBuilder::buildIcon(ParameterType parameterType, int number)
{
	static const float RESX = g_renderSettings.resolutionX;
	static const float RESY = g_renderSettings.resolutionY;

	static const float INVENTORY_START = RESX * 0.36f;
	static const float ICON_SIZE = (RESX - 2 * INVENTORY_START) / 20.0f;
	static const float BIAS = ICON_SIZE / 9.0f;
	float bias = BIAS * number;
	
	static const float BUBBLES_START = 993 * RESX / 2560.0f;

	int start = INVENTORY_START;
	if (number > 9)
		start = BUBBLES_START;

	std::array<GLfloat, 12> vertexPos{
		(start + ICON_SIZE * number - bias) / RESX,			(RESY * 0.062f) / RESY,				0.0f,
		(start + ICON_SIZE * (number + 1) - bias) / RESX,	(RESY * 0.062f) / RESY,				0.0f,
		(start + ICON_SIZE * (number + 1) - bias) / RESX,	(RESY * 0.062f + ICON_SIZE) / RESY,	0.0f,
		(start + ICON_SIZE * number - bias) / RESX,			(RESY * 0.062f + ICON_SIZE) / RESY,	0.0f,
	};

	for (auto & vertex : vertexPos) {
		vertex = vertex * 2 - 1.0f;
	}

	sf::Vector2i textureStarPos = ParametersTexturePositions[parameterType];
	std::array<GLfloat, 8U> texCoords{
		textureStarPos.x,		textureStarPos.y,
		textureStarPos.x + 9,	textureStarPos.y,
		textureStarPos.x + 9,	textureStarPos.y - 9,
		textureStarPos.x,		textureStarPos.y - 9,
	};

	for (auto & textureCoord : texCoords) {
		// might change to textCoord /= IconDatabase::get().textureAtlas.getAtlasSize()
		textureCoord /= 4096.0f;
	}

	m_pIconsMesh->addIcon(vertexPos, texCoords);
}