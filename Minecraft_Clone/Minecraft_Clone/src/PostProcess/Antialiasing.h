#pragma once

#include "ImageRenderer.h"
#include "fxaa.h"

class Antialiasing{
public:
    Antialiasing();

    void render(GLuint texture);
    ImageRenderer renderer;
private:
    FXAAShader fxaa;
};