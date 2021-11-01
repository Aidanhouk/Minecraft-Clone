#pragma once

#include "Shaders/HandShader.h"
#include "Player/PlayerHand/HandData.h"

#include "SFML/System.hpp"

class HandMesh;
struct RenderInfo;

class HandRenderer
{
public:
	HandRenderer();

	void addMeshToDraw(const HandMesh &handMesh);
	void render();
private:
	HandShader m_shader;
	const RenderInfo * m_mesh;

	const HandData* m_handData;
	glm::mat4 m_handProjMatrix;
};