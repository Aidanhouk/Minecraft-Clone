#pragma once

#include "Maths/glm.h"

enum class GameState
{
	NOT_STARTED = 0,
	PLAYING = 1,
	DIED = 2,
};

struct PlayerInfo {
	bool underwater = false;
	bool darkScreen = false;
	bool canMove = true;
	bool interfaceCursor = false;
	glm::vec3 delineatedBlock{ 0, -1, 0 };
	int breakingStage = 0;
	glm::vec3 position{ 0, 0, 0 };
	GameState gameState = GameState::NOT_STARTED;
};

extern PlayerInfo p_info;