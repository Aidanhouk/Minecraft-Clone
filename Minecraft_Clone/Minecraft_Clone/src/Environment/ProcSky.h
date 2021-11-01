#pragma once

#include "../Shaders/Environment/ProcSkyShader.h"
#include "../Model.h"
#include "../Camera.h"

class Camera;

class ProcSky
{
public:
    ProcSky();

    void render(const Camera& camera, float precipitationLighting);
private:
    ProcSkyShader m_shader;
    Model m_skybox;
};