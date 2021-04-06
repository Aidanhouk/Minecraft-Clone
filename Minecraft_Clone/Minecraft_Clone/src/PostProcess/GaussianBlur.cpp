#include "GaussianBlur.h"
#include "../RenderSettings.h"

GaussianBlur::GaussianBlur()
	: renderer((int)g_renderSettings.resolutionX >> 1, (int)g_renderSettings.resolutionY >> 1)
{}

void GaussianBlur::render(GLuint texture)
{
    gh_shader.useProgram();
    gh_shader.loadTarget((float)(g_renderSettings.resolutionX >> 1));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    renderer.render();

    gv_shader.useProgram();
    gv_shader.loadTarget((float)(g_renderSettings.resolutionY >> 1));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderer.getOutputTexture());
    renderer.render();
}
