#pragma once

#include "../Shaders/AimedBlock/BreakBlockShader.h"
#include "Model.h"

struct RenderInfo;
class Camera;

class BreakBlockRenderer {
public:
	void render(const Camera &camera);

	void setMeshToDraw();
private:

	BreakBlockShader m_breakBlockShader;
	Model m_model;
};