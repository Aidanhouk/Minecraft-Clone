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

void DroppedItemShader::getUniforms()
{
	BasicShader::getUniforms();
	m_time = glGetUniformLocation(m_id, "globalTime");
	m_lighting = glGetUniformLocation(m_id, "lighting");
}
