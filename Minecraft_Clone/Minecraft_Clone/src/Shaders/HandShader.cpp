#include "HandShader.h"

#include "Player/PlayerInfo.h"

HandShader::HandShader()
	: BasicShader("Hand", "Hand")
{
	getUniforms();
}

void HandShader::loadLighting(float light)
{
	loadFloat(m_lighting, light);
}

void HandShader::loadTime(float time)
{
	switch (g_PlayerInfo.playerState)
	{
	case PlayerState::NOT_MOVING:
	case PlayerState::SNEAKING:
		loadFloat(m_time, 3.14f / 4);
		break;
	case PlayerState::SPRINTING:
		loadFloat(m_time, time * 10);
		break;
	case PlayerState::WALKING:
		loadFloat(m_time, time * 7);
		break;
	default:
		break;
	}
}

void HandShader::getUniforms()
{
	BasicShader::getUniforms();
	m_lighting = glGetUniformLocation(m_id, "lighting");
	m_time = glGetUniformLocation(m_id, "globalTime");
}
