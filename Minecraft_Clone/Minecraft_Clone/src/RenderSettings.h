#ifndef RENDER_SETTINGS_H
#define RENDER_SETTINGS_H

#include <GL/glew.h>

struct RenderSettings{
    int resolutionX = 1920;
    int resolutionY = 1080;
};
extern RenderSettings g_renderSettings;

#endif
