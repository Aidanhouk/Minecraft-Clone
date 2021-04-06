#include "HorizontalGaussian.h"

GaussianH::GaussianH()
    : BasicShader("PostProcess/GaussianH", "PostProcess/Gaussian")
{
    getUniforms();
}

void GaussianH::getUniforms()
{
    BasicShader::getUniforms();
    target = glGetUniformLocation(m_id, "target");
}

void GaussianH::loadTarget(float atarget)
{
    loadFloat(target, atarget);
}
