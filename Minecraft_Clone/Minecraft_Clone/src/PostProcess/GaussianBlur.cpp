#include "GaussianBlur.h"
#include "../RenderSettings.h"

GaussianBlur::GaussianBlur()
	: renderer(g_RenderSettings.resolutionX / 2.0f, g_RenderSettings.resolutionY / 2.0f)
{}

void GaussianBlur::render(GLuint texture)
{
    gh_shader.useProgram();
    gh_shader.loadTarget(g_RenderSettings.resolutionX / 2.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    renderer.render();

    gv_shader.useProgram();
    gv_shader.loadTarget(g_RenderSettings.resolutionY / 2.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderer.getOutputTexture());
    renderer.render();
}
