#include "VerticalGaussian.h"

GaussianV::GaussianV()
    :   BasicShader("PostProcess/GaussianV", "PostProcess/Gaussian")
{
    getUniforms();
}

void GaussianV::getUniforms()
{
    BasicShader::getUniforms();
    target = glGetUniformLocation(m_id, "target");
}

void GaussianV::loadTarget(float atarget)
{
    loadFloat(target, atarget);
}
