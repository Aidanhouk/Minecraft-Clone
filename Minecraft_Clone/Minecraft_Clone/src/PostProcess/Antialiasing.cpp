#include "Antialiasing.h"

Antialiasing::Antialiasing()
	: renderer(g_renderSettings.resolutionX,g_renderSettings.resolutionY)
{

}

void Antialiasing::render(GLuint texture)
{
    fxaa.useProgram();
    fxaa.loadRes(glm::vec2(1.0f / g_renderSettings.resolutionX, 1.0f / g_renderSettings.resolutionY));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    renderer.render();
}
