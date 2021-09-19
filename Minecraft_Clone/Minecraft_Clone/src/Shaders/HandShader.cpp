#include "HandShader.h"

HandShader::HandShader()
	: BasicShader("Hand", "Hand")
{
	getUniforms();
}

void HandShader::loadLighting(float light)
{
	loadFloat(lighting, light);
}

void HandShader::getUniforms()
{
	BasicShader::getUniforms();
	lighting = glGetUniformLocation(m_id, "lighting");
}
