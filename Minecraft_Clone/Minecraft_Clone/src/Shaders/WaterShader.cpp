#include "WaterShader.h"

#include "GlobalInfo.h"

WaterShader::WaterShader()
    : BasicShader("Water", "Water")
{
    getUniforms();
}

void WaterShader::loadTime(const float &time)
{
    loadFloat(m_time, time);
}

void WaterShader::loadLighting(float light)
{
	loadFloat(m_lighting, light);
}

void WaterShader::loadDTime(float time)
{
	loadFloat(m_dtime, g_Info.dayTime);
}

void WaterShader::loadFog(bool fog)
{
	loadInt(m_fog, fog);
}

void WaterShader::loadFogDensity(float density)
{
	loadFloat(m_fogDensity, density);
}

void WaterShader::getUniforms()
{
    BasicShader::getUniforms();
    m_time = glGetUniformLocation(m_id, "globalTime");
	m_lighting = glGetUniformLocation(m_id, "lighting");
	m_dtime = glGetUniformLocation(m_id, "dayTime");
	m_fog = glGetUniformLocation(m_id, "fog");
	m_fogDensity = glGetUniformLocation(m_id, "fogDensity");
}
