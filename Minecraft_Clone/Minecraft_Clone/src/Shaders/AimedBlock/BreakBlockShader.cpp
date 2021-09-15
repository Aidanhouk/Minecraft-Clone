#include "BreakBlockShader.h"

BreakBlockShader::BreakBlockShader()
	: BasicShader("BreakBlock", "BreakBlock")
{
	getUniforms();
}

void BreakBlockShader::loadLighting(float light)
{
	loadFloat(lighting, light);
}

void BreakBlockShader::getUniforms()
{
	BasicShader::getUniforms();
	lighting = glGetUniformLocation(m_id, "lighting");
}
