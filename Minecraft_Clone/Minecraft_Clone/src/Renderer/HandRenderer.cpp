#include "HandRenderer.h"

#include "World/Block/BlockDatabase.h"
#include "GlobalInfo.h"
#include "Camera.h"
#include "Player/PlayerHand/HandMesh.h"
#include "RenderSettings.h"

#include <iostream>

const float PI = 3.14159265359f;

HandRenderer::HandRenderer()
{
	m_handProjMatrix = glm::perspective(glm::radians(70.0f),
		(float)g_RenderSettings.resolutionX / (float)g_RenderSettings.resolutionY, 0.1f, 2000.0f);
}

void HandRenderer::addMeshToDraw(const HandMesh & handMesh)
{
	static bool doItOnce = 1;
	if (doItOnce) {
		doItOnce = 0;

		m_mesh = &handMesh.getModel().getRenderInfo();
		m_handData = &handMesh.getHandData();
	}
}

void HandRenderer::render()
{
	glDisable(GL_DEPTH_TEST);
	m_shader.useProgram();
	BlockDatabase::get().textureAtlas.bindTexture();

	glm::mat4 modelMatrix(1.0f);

	switch (m_handData->handType)
	{
	case HandType::Empty:
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.6f, 0.0f, -0.3f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(-80.0f), glm::vec3(0, 1, 0));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(-30.0f), glm::vec3(1, 0, 0));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(30.0f), glm::vec3(0, 0, 1));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.1f, 0.5f, 0.0f));

		if (m_handData->animationType == AnimationType::Breaking) {
			modelMatrix = glm::rotate(modelMatrix, glm::radians(std::min(90.0f * m_handData->animationStage, 60.0f)), glm::vec3(0, 0, 1));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(
				cos(m_handData->animationStage * PI) * 0.2f - m_handData->animationStage * 0.5f,
				sin(m_handData->animationStage * PI) * 0.6f - 0.3f,
				sin(m_handData->animationStage * PI) * 0.3f
			));
		}

		break;
	case HandType::Default:

		if (m_handData->animationType == AnimationType::Idle) {
			modelMatrix = glm::rotate(modelMatrix, glm::radians(-70.0f), glm::vec3(0, 1, 0));
			modelMatrix = glm::rotate(modelMatrix, glm::radians(20.0f), glm::vec3(0, 0, 1));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.8f, 0.2f, 0.0f));

			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.6f, 0.6f, 0.6f));
		}
		else if (m_handData->animationType == AnimationType::Breaking) {
			modelMatrix = glm::rotate(modelMatrix, glm::radians(-70.0f), glm::vec3(0, 1, 0));
			modelMatrix = glm::rotate(modelMatrix, glm::radians(20.0f), glm::vec3(0, 0, 1));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(
				-1.4f - m_handData->animationStage * 0.7f,
				-0.4f + m_handData->animationStage * 0.2f,
				m_handData->animationStage * 0.3f
			));
			modelMatrix = glm::rotate(modelMatrix, glm::radians(90 * m_handData->animationStage), glm::vec3(0, 0, 1));
		}
		else if (m_handData->animationType == AnimationType::Eating) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.6f, -0.5f, 0.0f));
			modelMatrix = glm::rotate(modelMatrix, glm::radians(25.0f), glm::vec3(1, 0, 0));
			modelMatrix = glm::rotate(modelMatrix, glm::radians(10.0f), glm::vec3(0, 0, 1));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, sin(m_handData->animationStage * PI) * 0.15f, 0.0f));
		}
		break;
	case HandType::Cube:
	case HandType::Cactus:
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, -1.0f)); // necessary
	
		modelMatrix = glm::rotate(modelMatrix, glm::radians(-40.0f), glm::vec3(0, 1, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.3f, -0.3f, 0.0f));
	
		if (m_handData->animationType == AnimationType::Breaking) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(
				-m_handData->animationStage * 0.2f,
				m_handData->animationStage * 0.3f,
				m_handData->animationStage * 0.2f));
			modelMatrix = glm::rotate(modelMatrix, glm::radians(-90 * m_handData->animationStage), glm::vec3(1, 0, 0));
			modelMatrix = glm::rotate(modelMatrix, glm::radians( 60 * m_handData->animationStage), glm::vec3(0, 0, 1));
			float scale = 1 - m_handData->animationStage * 0.4f;
			modelMatrix = glm::scale(modelMatrix, glm::vec3(scale, scale, scale));
		}
		break;
	default:
		break;
	}

	m_shader.loadModelMatrix(modelMatrix);
	m_shader.loadProjectionMatrix(m_handProjMatrix);

	m_shader.loadLighting(g_Info.lighting);
	m_shader.loadTime(g_Info.elapsedTime);

	GL::bindVAO(m_mesh->vao);
	GL::drawElements(m_mesh->indicesCount);
}
