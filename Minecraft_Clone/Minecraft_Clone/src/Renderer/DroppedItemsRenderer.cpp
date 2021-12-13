#include "DroppedItemsRenderer.h"

#include "../Application.h"
#include "../Camera.h"
#include "Item/DroppedItem/DroppedItemsMesh.h"
#include "World/Block/BlockDatabase.h"

#include <iostream>

void DroppedItemsRenderer::addMeshToDraw(const DroppedItemsMesh & droppedItemsMesh)
{
	m_mesh = &droppedItemsMesh.getModel().getRenderInfo();
}

void DroppedItemsRenderer::render(const Camera & camera)
{
	glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);

	m_shader.useProgram();
	BlockDatabase::get().textureAtlas.bindTexture();

	m_shader.loadProjectionMatrix(camera.getProjMatrix());
	m_shader.loadViewMatrix(camera.getViewMatrix());

	m_shader.loadTime(g_Info.dayTime);
	// @TODO do smth here
	m_shader.loadLighting(g_Info.lighting);
	m_shader.loadFog(g_Info.fog);
	m_shader.loadFogDensity(g_Config.fogDensity);

	GL::bindVAO(m_mesh->vao);
	GL::drawElements(m_mesh->indicesCount);

	m_mesh = nullptr;
}
