#pragma once

#include "../Shaders/BasicShader.h"

class MotionBlur : public BasicShader{
public:
    MotionBlur();

    void loadT1(GLuint tt1);
    void loadT2(GLuint tt2);
    void loadPrevProj(glm::mat4 prev);
    void loadCurrProj(glm::mat4 curr);
    void loadRes(glm::vec2 res);

private:
    void getUniforms();
    GLuint t1, t2, m_prev, m_curr, m_res;
};