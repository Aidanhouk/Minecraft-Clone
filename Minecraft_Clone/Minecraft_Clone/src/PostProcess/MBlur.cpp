#include "MBlur.h"

#include "../RenderSettings.h"

MBlur::MBlur()
	: renderer(g_RenderSettings.resolutionX, g_RenderSettings.resolutionY)
{
    prev = glm::mat4(1.0f);
}

void MBlur::render(const Camera& cam, FrameBufferObject& fbo)
{
    m_shader.useProgram();
    m_shader.loadCurrProj(cam.getProjectionViewMatrix());
    m_shader.loadPrevProj(prev);
    m_shader.loadRes(glm::vec2(g_RenderSettings.resolutionX, g_RenderSettings.resolutionY));
    prev = cam.getProjectionViewMatrix();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fbo.getColorTex());

    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_2D, fbo.getDepthTex());

    renderer.render();

    glActiveTexture(GL_TEXTURE0);
}
