#pragma once

#include "GL/glew.h"

namespace GL {
void drawElements(GLuint indicesCount) noexcept;
void drawLines(GLuint indicesCount) noexcept;
void bindVAO(GLuint vao) noexcept;

namespace Enum {
enum Texture { Tex2D = GL_TEXTURE_2D, TexCubeMap = GL_TEXTURE_CUBE_MAP };
}
} // namespace GL