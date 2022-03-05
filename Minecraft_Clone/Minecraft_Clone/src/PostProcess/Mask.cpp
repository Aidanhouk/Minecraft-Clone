#include "Mask.h"

MaskShader::MaskShader()
    : BasicShader("PostProcess/Mask", "PostProcess/Mask")
{
    getUniforms();
}

void MaskShader::getUniforms()
{
    BasicShader::getUniforms();
    //m_luma = glGetUniformLocation(m_id, "requestedLuminance");
}

//void MaskShader::loadLuma(float luma)
//{
//    loadFloat(m_luma, luma);
//}
