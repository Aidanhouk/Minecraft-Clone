#pragma once

#include <SFML/Graphics.hpp>

#include "../Shaders/Basic_2D_Shader.h"

struct RenderInfo;
class Camera;
class IconsMesh;

class Renderer_2D {
public:
	void setMeshToDraw(const IconsMesh &iconsMesh);
	void render();
private:
	const RenderInfo * m_mesh;

	Basic_2D_Shader m_shader;
};