#pragma once

#include "glm.h"

class Camera;

struct Entity;
struct Config;

glm::mat4 makeModelMatrix(const Entity &entity);
glm::mat4 makeViewMatrix(const Camera &camera);
glm::mat4 makeProjectionMatrix(const Config &config);