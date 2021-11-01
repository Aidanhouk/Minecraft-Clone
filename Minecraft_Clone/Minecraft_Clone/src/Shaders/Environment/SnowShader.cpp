#include "SnowShader.h"

SnowShader::SnowShader()
	: BasicShader("Environment/Snow", "Environment/Snow")
{
	getUniforms();
}

void SnowShader::loadTime(float time)
{
	loadFloat(moveTime, time);
}

void SnowShader::loadLighting(float light)
{
	loadFloat(lighting, light);
}

void SnowShader::loadVisibility(float visibility)
{
	loadFloat(m_visibility, visibility);
}

void SnowShader::getUniforms()
{
	BasicShader::getUniforms();
	moveTime = glGetUniformLocation(m_id, "time");
	lighting = glGetUniformLocation(m_id, "lighting");
	m_visibility = glGetUniformLocation(m_id, "visibility");
}