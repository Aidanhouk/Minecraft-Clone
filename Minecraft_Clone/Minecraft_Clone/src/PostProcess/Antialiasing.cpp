#include "Antialiasing.h"

#include "../RenderSettings.h"

Antialiasing::Antialiasing()
	: renderer(g_RenderSettings.resolutionX,g_RenderSettings.resolutionY)
{

}

void Antialiasing::render(GLuint texture)
{
    fxaa.useProgram();
    fxaa.loadRes(glm::vec2(1.0f / g_RenderSettings.resolutionX, 1.0f / g_RenderSettings.resolutionY));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    renderer.render();
}
