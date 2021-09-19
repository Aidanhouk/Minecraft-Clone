#include "BlockBreaker.h"

#include "Player/PlayerInfo.h"

#include <iostream>

BlockBreaker::BlockBreaker()
	: m_playerIsBreaking(false), m_blockPostion{-1, -1, -1}
{
}

bool BlockBreaker::_break(glm::ivec3 blockPosition, float hardness)
{
	if (blockPosition != m_blockPostion) {
		m_playerIsBreaking = true;
		m_blockPostion = blockPosition;
		m_startBreakingTimer.restart();
	}
	else {
		bool animationChanged = false;
		for (int i = 1; i < 11; ++i) {
			if (m_startBreakingTimer.getElapsedTime().asSeconds() < 0.1 * i * hardness) {
				p_info.breakingStage = i;
				animationChanged = true;
				break;
			}
		}
		if (!animationChanged) {
			stopBreaking();
			return true;
		}
	}

	return false;
}

void BlockBreaker::stopBreaking()
{
	m_playerIsBreaking = false;
	m_blockPostion = { -1, -1, -1 };
	p_info.breakingStage = 0;
}
