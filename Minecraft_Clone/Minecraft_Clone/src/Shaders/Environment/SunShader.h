#pragma once

#include "../BasicShader.h"

class SunShader : public BasicShader{
public:
    SunShader();
    void loadTime(float ticks);
private:
    void getUniforms() override;
    GLuint m_day;
};