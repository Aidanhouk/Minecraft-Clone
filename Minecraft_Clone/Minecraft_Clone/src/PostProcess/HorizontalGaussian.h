#pragma once

#include "../Shaders/BasicShader.h"

class GaussianH : public BasicShader{
public:
    GaussianH();
    void loadTarget(float target);
private:
    void getUniforms() override;
    GLuint target;
};