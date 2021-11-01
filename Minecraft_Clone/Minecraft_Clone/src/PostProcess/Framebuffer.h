#pragma once

#include <GL/glew.h>

class FrameBufferObject{
public:
    FrameBufferObject();
    FrameBufferObject(int resolutionX = 0, int resolutionY = 0);

    void bind();
    void clear();
    GLuint getColorTex();
    GLuint getDepthTex();

    GLuint m_fbo;
    int width, height;
private:
    void init(int resolutionX, int resolutionY);

    GLuint m_colTex;
    GLuint m_depTex;
};