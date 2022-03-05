#include "Bloom.h"

#include "../RenderSettings.h"

Bloom::Bloom()
	: renderer(g_RenderSettings.resolutionX, g_RenderSettings.resolutionY)
{}

void Bloom::render(GLuint texture)
{
    m_shader.useProgram();
    //m_shader.loadLuma(0.99f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    renderer.render();

    gblur.render(renderer.getOutputTexture());

    c_shader.useProgram();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    c_shader.loadTex(texture);

    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_2D, gblur.renderer.getOutputTexture());
    c_shader.loadTex2(gblur.renderer.getOutputTexture());

    renderer.render();

    glActiveTexture(GL_TEXTURE0);
}
