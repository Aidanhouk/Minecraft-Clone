#pragma once

#include "../Shaders/AimedBlock/DelineateShader.h"
#include "Model.h"

struct RenderInfo;
class Camera;

class DelineateBlockRenderer {
public:
	DelineateBlockRenderer();

	void render(const Camera &camera);

private:
	DelineateShader m_delineateShader;
	Model m_model;
};