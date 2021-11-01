#pragma once

#include "Mask.h"
#include "Combine.h"
#include "ImageRenderer.h"
#include "GaussianBlur.h"

class Bloom{
public:
    Bloom();

    void render(GLuint texture);
    ImageRenderer renderer;

    GaussianBlur gblur;
private:
    MaskShader m_shader;
    CombineShader c_shader;
};