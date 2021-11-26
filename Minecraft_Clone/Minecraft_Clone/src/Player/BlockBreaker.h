#pragma once

#include "SFML/System.hpp"
#include "Maths/glm.h"
#include "World/Block/BlockId.h"

class BlockBreaker
{
public:
	BlockBreaker();

	// returns if the block has been broken
	bool _break(BlockId heldItemId, glm::ivec3 blockPosition, float hardness);
	void stopBreaking();
private:

	bool m_playerIsBreaking;
	// position of the block that is being broken now
	glm::ivec3 m_blockPostion;
	BlockId m_heldItemId;
	sf::Clock m_startBreakingTimer;
};