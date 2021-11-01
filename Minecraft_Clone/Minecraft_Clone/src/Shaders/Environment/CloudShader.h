#pragma once

#include "../BasicShader.h"

class CloudShader : public BasicShader
{
public:
    CloudShader();

    void loadLighting(float light);
private:
    void getUniforms() override;
    GLuint lighting;
};