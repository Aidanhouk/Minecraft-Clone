#pragma once

#include "Maths/glm.h"

struct PlayerInfo {
	bool underwater;
	bool darkScreen;
	bool canMove;
	bool interfaceCursor;
	glm::vec3 delineatedBlock;
	int breakingStage;
	glm::vec3 position;
};

extern PlayerInfo p_info;