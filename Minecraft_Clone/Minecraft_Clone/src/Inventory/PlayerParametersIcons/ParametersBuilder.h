#pragma once

#include "Texture/TextureAtlas.h"

enum ParameterType {
	FullHeart = 0,
	HalfHeart = 1,
	EmptyHeart = 2,
	FullHunger = 3,
	HalfHunger = 4,
	EmptyHunger = 5,
	FullBubble = 6,

	NUMBER_OF_TYPES,
};

class IconsMesh;
class Inventory;
class Player;

class ParametersBuilder
{
public:
	ParametersBuilder(Inventory &inventory, IconsMesh &itemIconsMesh, Player &player);

	void buildMesh();
private:
	void buildIcon(ParameterType parameterType, int number);

	Inventory *m_pInventory = nullptr;
	IconsMesh *m_pIconsMesh = nullptr;
	Player *m_pPlayer = nullptr;
};