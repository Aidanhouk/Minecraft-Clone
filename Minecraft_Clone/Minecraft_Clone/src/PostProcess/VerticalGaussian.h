#pragma once

#include "../Shaders/BasicShader.h"

class GaussianV : public BasicShader{
public:
    GaussianV();
    void loadTarget(float target);
private:
    void getUniforms() override;
    GLuint target;
};