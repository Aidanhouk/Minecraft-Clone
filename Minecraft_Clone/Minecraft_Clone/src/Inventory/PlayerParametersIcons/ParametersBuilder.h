#pragma once

#include "Texture/TextureAtlas.h"

enum ParameterType {
	FullHeart = 0,
	HalfHeart = 1,
	EmptyHeart = 2,
	FullBubble = 3,
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