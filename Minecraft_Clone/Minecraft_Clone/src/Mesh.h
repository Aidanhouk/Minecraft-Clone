#pragma once

#include "GL/glew.h"
#include <vector>

struct Mesh {
    std::vector<GLfloat> vertexPositions;
    std::vector<GLfloat> textureCoords;
    std::vector<GLuint> indices;
};