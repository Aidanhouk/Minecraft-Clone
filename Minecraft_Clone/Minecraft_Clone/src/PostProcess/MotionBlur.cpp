#include "MotionBlur.h"

MotionBlur::MotionBlur()
    :   BasicShader("PostProcess/MBlur", "PostProcess/MBlur")
{
    getUniforms();
}

void MotionBlur::getUniforms()
{
    BasicShader::getUniforms();
    useProgram();
    t1 = glGetUniformLocation(m_id, "texSampler");
    t2 = glGetUniformLocation(m_id, "depth");
    m_prev = glGetUniformLocation(m_id, "prevProjViewMatrix");
    m_curr = glGetUniformLocation(m_id, "currProjViewMatrix");
    m_res = glGetUniformLocation(m_id, "resolution");
}

void MotionBlur::loadT1(GLuint tt1)
{
	//glUniform1i(t1, 0);
	glUniform1i(t1, tt1);
}

void MotionBlur::loadT2(GLuint tt2)
{
	//glUniform1i(t2, 2);
	glUniform1i(t2, tt2);
}

void MotionBlur::loadPrevProj(glm::mat4 prev)
{
    loadMatrix4(m_prev, prev);
}
void MotionBlur::loadCurrProj(glm::mat4 curr)
{
    loadMatrix4(m_curr, curr);
}

void MotionBlur::loadRes(glm::vec2 v)
{
    loadVector2(m_res, v);
}
