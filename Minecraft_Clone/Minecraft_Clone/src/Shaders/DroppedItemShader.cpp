#include "DroppedItemShader.h"

#include "GlobalInfo.h"

DroppedItemShader::DroppedItemShader()
	: BasicShader("DroppedItem", "DroppedItem")
{
	getUniforms();
}

void DroppedItemShader::loadTime(const float &time)
{
	loadFloat(m_time, time);
}

void DroppedItemShader::loadLighting(float light)
{
	loadFloat(m_lighting, light);
}

void DroppedItemShader::loadFog(bool fog)
{
	loadInt(m_fog, fog);
}

void DroppedItemShader::loadFogDensity(float density)
{
	loadFloat(m_fogDensity, density);
}

void DroppedItemShader::getUniforms()
{
	BasicShader::getUniforms();
	m_time = glGetUniformLocation(m_id, "globalTime");
	m_lighting = glGetUniformLocation(m_id, "lighting");
	m_fog = glGetUniformLocation(m_id, "fog");
	m_fogDensity = glGetUniformLocation(m_id, "fogDensity");
}
