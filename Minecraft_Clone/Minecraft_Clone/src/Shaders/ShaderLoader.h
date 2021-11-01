#pragma once

#include "GL/glew.h"
#include <string>

GLuint loadShaders(const std::string &vertexShader,
                   const std::string &fragmentShader);