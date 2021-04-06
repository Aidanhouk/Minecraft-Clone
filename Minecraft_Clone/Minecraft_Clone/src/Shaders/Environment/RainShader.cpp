#include "RainShader.h"

RainShader::RainShader()
	: BasicShader("Environment/Rain", "Environment/Rain")
{
	getUniforms();
}

void RainShader::loadLighting(float light)
{
	loadFloat(lighting, light);
}

void RainShader::getUniforms()
{
	BasicShader::getUniforms();
	lighting = glGetUniformLocation(m_id, "lighting");
}