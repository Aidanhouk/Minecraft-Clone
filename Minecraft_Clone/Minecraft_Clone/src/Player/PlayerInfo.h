#pragma once

#include "Maths/glm.h"

class Player;

enum class GameState
{
	NOT_STARTED = 0,
	PLAYING = 1,
	DIED = 2,
	PAUSED = 3,
};

enum class PlayerState
{
	NOT_MOVING = 0,
	WALKING = 1,
	SPRINTING = 2,
	SNEAKING = 3,
};

struct PlayerInfo {
	Player *player;
	bool underwater;
	bool darkScreen;
	bool canMove;
	bool inventoryCursor;
	bool FPS_HUD;
	glm::vec3 delineatedBlock;
	int breakingStage;
	float cameraPosition;
	float cameraRotation;
	GameState gameState;
	PlayerState playerState;
	float playerLighting;
};

extern PlayerInfo g_PlayerInfo;