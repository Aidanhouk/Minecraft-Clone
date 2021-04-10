#pragma once

#include <SFML/Graphics.hpp>

#include "../Shaders/DroppedItemShader.h"

struct RenderInfo;
class Camera;
class DroppedItemsMesh;

class DroppedItemsRenderer
{
public:
	void setMeshToDraw(const DroppedItemsMesh &droppedItemsMesh);
	void render(const Camera &camera);
private:
	const RenderInfo * m_mesh;

	DroppedItemShader m_shader;
};
