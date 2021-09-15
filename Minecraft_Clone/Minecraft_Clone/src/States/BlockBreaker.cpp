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
			 if (m_startBreakingTimer.getElapsedTime().asSeconds() < 0.1 * hardness)
			p_info.breakingStage = 1;
		else if (m_startBreakingTimer.getElapsedTime().asSeconds() < 0.2 * hardness)
			p_info.breakingStage = 2;
		else if (m_startBreakingTimer.getElapsedTime().asSeconds() < 0.3 * hardness)
			p_info.breakingStage = 3;
		else if (m_startBreakingTimer.getElapsedTime().asSeconds() < 0.4 * hardness)
			p_info.breakingStage = 4;
		else if (m_startBreakingTimer.getElapsedTime().asSeconds() < 0.5 * hardness)
			p_info.breakingStage = 5;
		else if (m_startBreakingTimer.getElapsedTime().asSeconds() < 0.6 * hardness)
			p_info.breakingStage = 6;
		else if (m_startBreakingTimer.getElapsedTime().asSeconds() < 0.7 * hardness)
			p_info.breakingStage = 7;
		else if (m_startBreakingTimer.getElapsedTime().asSeconds() < 0.8 * hardness)
			p_info.breakingStage = 8;
		else if (m_startBreakingTimer.getElapsedTime().asSeconds() < 0.9 * hardness)
			p_info.breakingStage = 9;
		else if (m_startBreakingTimer.getElapsedTime().asSeconds() < 1.0 * hardness)
			p_info.breakingStage = 10;
		else {
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
