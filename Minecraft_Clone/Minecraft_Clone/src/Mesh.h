#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED

#include "GL/glew.h"
#include <vector>

struct Mesh {
    std::vector<GLfloat> vertexPositions;
    std::vector<GLfloat> textureCoords;
    std::vector<GLuint> indices;
};

#endif // MESH_H_INCLUDED
