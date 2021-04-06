#include "CloudShader.h"

CloudShader::CloudShader()
    :   BasicShader("Environment/Cloud", "Environment/Cloud")
{
    getUniforms();
}

void CloudShader::loadLighting(float light)
{
    loadFloat(lighting, light);
}

void CloudShader::getUniforms()
{
    BasicShader::getUniforms();
    lighting = glGetUniformLocation(m_id, "lighting");
}
