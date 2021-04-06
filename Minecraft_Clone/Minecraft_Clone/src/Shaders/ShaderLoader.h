#ifndef SHADERLOADER_H_INCLUDED
#define SHADERLOADER_H_INCLUDED

#include "GL/glew.h"
#include <string>

GLuint loadShaders(const std::string &vertexShader,
                   const std::string &fragmentShader);

#endif // SHADERLOADER_H_INCLUDED
