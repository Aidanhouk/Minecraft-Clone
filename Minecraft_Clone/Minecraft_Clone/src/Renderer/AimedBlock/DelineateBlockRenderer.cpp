#include "DelineateBlockRenderer.h"

#include "../Camera.h"
#include "Player/PlayerInfo.h"

#include <iostream>

DelineateBlockRenderer::DelineateBlockRenderer()
{
	std::vector<GLfloat> vertexCoords{
		-0.005f,  1.005f, -0.005f,
		 1.005f,  1.005f, -0.005f,
		 1.005f,  1.005f,  1.005f,
		-0.005f,  1.005f,  1.005f,
			 
		-0.005f, -0.005f, -0.005f,
		 1.005f, -0.005f, -0.005f,
		 1.005f, -0.005f,  1.005f,
		-0.005f, -0.005f,  1.005f,
	};
	std::vector<GLuint> indexCoords{
		0, 1,
		1, 2,
		2, 3,
		3, 0,

		4, 5,
		5, 6,
		6, 7,
		7, 4,

		0, 4,
		1, 5,
		2, 6,
		3, 7,
	};

	m_model.addDeliatedBlockData({ vertexCoords, std::vector<GLfloat>(), indexCoords });
}

void DelineateBlockRenderer::render(const Camera &camera)
{
	if (p_info.delineatedBlock.x == 0
		&& p_info.delineatedBlock.y == -1
		&& p_info.delineatedBlock.z == 0)
	{
		return;
	}

	m_delineateShader.useProgram();
	m_model.bindVAO();

	m_delineateShader.loadProjectionViewMatrix(camera.getProjectionViewMatrix());
	m_delineateShader.loadModelMatrix(glm::translate(glm::mat4(1.0f),
		glm::vec3(p_info.delineatedBlock.x, p_info.delineatedBlock.y, p_info.delineatedBlock.z)));
	GL::drawLines(m_model.getIndicesCount());
}