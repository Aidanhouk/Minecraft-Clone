#include "ChunkShader.h"

ChunkShader::ChunkShader()
    : BasicShader("Chunk", "Chunk")
{
    getUniforms();
}

void ChunkShader::loadLighting(float light)
{
	loadFloat(m_lighting, light);
}

void ChunkShader::loadDTime(float dt)
{
	loadFloat(m_dtime, dt);
}

void ChunkShader::loadFog(bool fog)
{
	loadInt(m_fog, fog);
}

void ChunkShader::loadFogDensity(float density)
{
	loadFloat(m_fogDensity, density);
}

void ChunkShader::getUniforms()
{
    BasicShader::getUniforms();
	m_lighting = glGetUniformLocation(m_id, "lighting");
	m_dtime = glGetUniformLocation(m_id, "dayTime");
	m_fog = glGetUniformLocation(m_id, "fog");
	m_fogDensity = glGetUniformLocation(m_id, "fogDensity");
}
