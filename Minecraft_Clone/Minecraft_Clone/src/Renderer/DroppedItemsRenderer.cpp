#include "DroppedItemsRenderer.h"

#include "../Application.h"
#include "../Camera.h"
#include "Item/DroppedItem/DroppedItemsMesh.h"
#include "World/Block/BlockDatabase.h"

#include <iostream>

void DroppedItemsRenderer::setMeshToDraw(const DroppedItemsMesh & droppedItemsMesh)
{
	m_mesh = &droppedItemsMesh.getModel().getRenderInfo();
}

void DroppedItemsRenderer::render(const Camera & camera)
{
	glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);

	m_shader.useProgram();
	BlockDatabase::get().textureAtlas.bindTexture();

	m_shader.loadProjectionViewMatrix(camera.getProjectionViewMatrix());

	//m_shader.loadModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	//m_shader.loadModelMatrix(glm::rotate(glm::mat4(1.0f), glm::radians(1.0f * g_info.dayTime), glm::vec3(0.0f, 1.0f, 0.0f)));
	//m_shader.loadModelMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f)));

	m_shader.loadTime(g_info.dayTime);
	m_shader.loadLighting(g_info.lighting);

	GL::bindVAO(m_mesh->vao);
	GL::drawElements(m_mesh->indicesCount);

	m_mesh = nullptr;
}
