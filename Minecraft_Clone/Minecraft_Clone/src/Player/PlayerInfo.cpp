#include "PlayerInfo.h"

PlayerInfo g_PlayerInfo{
nullptr,					 //Player *player;
false,						 //bool underwater;
false,						 //bool darkScreen;
true,						 //bool canMove;
false,						 //bool inventoryCursor;
true,						 //bool FPS_HUD;
{ 0, -1, 0 },				 //glm::vec3 delineatedBlock;
0,							 //int breakingStage;
0.6f,						 //float cameraPosition;
0.0f,						 //float cameraRotation;
1.0f,						 //float caveLighting;
GameState::NOT_STARTED,		 //GameState gameState;
PlayerState::NOT_MOVING,	 //PlayerState playerState;
};