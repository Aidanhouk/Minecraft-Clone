#include "FloraShader.h"

FloraShader::FloraShader()
    : BasicShader("Flora", "Flora")
{
    getUniforms();
}

void FloraShader::loadTime(float time)
{
    loadFloat(m_time, time);
}

void FloraShader::loadLight(float lighting)
{
	loadFloat(m_light, lighting);
}

void FloraShader::loadDTime(float time)
{
	loadFloat(m_dtime, time);
}

void FloraShader::loadFog(bool fog)
{
	loadInt(m_fog, fog);
}

void FloraShader::loadFogDensity(float density)
{
	loadFloat(m_fogDensity, density);
}

void FloraShader::getUniforms()
{
    BasicShader::getUniforms();
    m_time = glGetUniformLocation(m_id, "globalTime");
	m_light = glGetUniformLocation(m_id, "lighting");
	m_dtime = glGetUniformLocation(m_id, "dayTime");
	m_fog = glGetUniformLocation(m_id, "fog");
	m_fogDensity = glGetUniformLocation(m_id, "fogDensity");
}
