#include "Renderer_2D.h"

#include "../Application.h"
#include "../Camera.h"
#include "Inventory/IconsMesh.h"
#include "Inventory/IconDatabase.h"

void Renderer_2D::setMeshToDraw(const IconsMesh & iconsMesh)
{
	m_mesh = &iconsMesh.getModel().getRenderInfo();
}

void Renderer_2D::render()
{
	glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	m_shader.useProgram();
	IconDatabase::get().textureAtlas.bindTexture();

	GL::bindVAO(m_mesh->vao);
	GL::drawElements(m_mesh->indicesCount);

	m_mesh = nullptr;
}