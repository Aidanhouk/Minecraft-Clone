#include "BreakBlockRenderer.h"

#include "../Camera.h"
#include "Player/PlayerInfo.h"
#include "GlobalInfo.h"
#include "World/Block/BlockDatabase.h"

#include <iostream>

const float bias = 0.005f;

const std::vector<GLfloat> vertexCoords{
	// frontFace
	0 - bias, 0 - bias, 1 + bias, // 0 left-bottom
	1 + bias, 0 - bias, 1 + bias, // 1 rigth-bottom
	1 + bias, 1 + bias, 1 + bias, // 2 right-top
	0 - bias, 1 + bias, 1 + bias, // 3 left-top
	// backFace
	1 + bias, 0 - bias, 0 - bias, // 4 right-bottom
	0 - bias, 0 - bias, 0 - bias, // 5 left-bottom
	0 - bias, 1 + bias, 0 - bias, // 6 left-top
	1 + bias, 1 + bias, 0 - bias, // 7 right-top
	// leftFace
	0 - bias, 0 - bias, 0 - bias, // 5 left-bottom
	0 - bias, 0 - bias, 1 + bias, // 0 left-bottom
	0 - bias, 1 + bias, 1 + bias, // 3 left-top
	0 - bias, 1 + bias, 0 - bias, // 6 left-top
	// rightFace
	1 + bias, 0 - bias, 1 + bias, // 1 rigth-bottom
	1 + bias, 0 - bias, 0 - bias, // 4 right-bottom
	1 + bias, 1 + bias, 0 - bias, // 7 right-top
	1 + bias, 1 + bias, 1 + bias, // 2 right-top
	// bottomFace
	0 - bias, 0 - bias, 0 - bias, // 5 left-bottom
	1 + bias, 0 - bias, 0 - bias, // 4 right-bottom
	1 + bias, 0 - bias, 1 + bias, // 1 rigth-bottom
	0 - bias, 0 - bias, 1 + bias, // 0 left-bottom
	// topFace
	0 - bias, 1 + bias, 1 + bias, // 3 left-top
	1 + bias, 1 + bias, 1 + bias, // 2 right-top
	1 + bias, 1 + bias, 0 - bias, // 7 right-top
	0 - bias, 1 + bias, 0 - bias, // 6 left-top
};
const std::vector<GLuint> indices{
	0, 1, 2,
	2, 3, 0,

	4, 5, 6,
	6, 7, 4,

	8, 9, 10,
	10, 11, 8,

	12, 13, 14,
	14, 15, 12,

	16, 17, 18,
	18, 19, 16,

	20, 21, 22,
	22, 23, 20,
};

void BreakBlockRenderer::render(const Camera &camera)
{
	if (g_PlayerInfo.breakingStage == 0) {
		return;
	}

	glEnable(GL_BLEND);

	m_breakBlockShader.useProgram();
	BlockDatabase::get().textureAtlas.bindTexture();
	m_breakBlockShader.loadProjectionViewMatrix(camera.getProjectionViewMatrix());
	m_breakBlockShader.loadModelMatrix(glm::translate(glm::mat4(1.0f),
		glm::vec3(g_PlayerInfo.delineatedBlock.x, g_PlayerInfo.delineatedBlock.y, g_PlayerInfo.delineatedBlock.z)));

	m_breakBlockShader.loadLighting(g_Info.lighting);

	m_model.bindVAO();
	GL::drawElements(m_model.getIndicesCount());

	m_model.deleteData();
}

void BreakBlockRenderer::addMeshToDraw()
{
	if (g_PlayerInfo.breakingStage == 0) {
		return;
	}
	
	std::array<GLfloat, 8> texCoords;
	BlockDatabase::get().textureAtlas.getTextureCoords(texCoords, { g_PlayerInfo.breakingStage - 1, 14 });

	std::vector<GLfloat> texCoordsAll6Faces;
	for (int i = 0; i < 6; ++i) {
		texCoordsAll6Faces.insert(texCoordsAll6Faces.end(), texCoords.begin(), texCoords.end());
	}

	m_model.addData({ vertexCoords, texCoordsAll6Faces, indices });
}