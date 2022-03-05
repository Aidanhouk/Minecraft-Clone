#pragma once

#include "../Shaders/BasicShader.h"

class MaskShader : public BasicShader{
public:
    MaskShader();
    //void loadLuma(float luma);
private:
    void getUniforms() override;
    //GLuint m_luma;
};