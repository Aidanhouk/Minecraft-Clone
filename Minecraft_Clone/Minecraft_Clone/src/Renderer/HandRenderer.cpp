#include "HandRenderer.h"

#include "World/Block/BlockDatabase.h"
#include "GlobalInfo.h"

void HandRenderer::render()
{
	m_shader.useProgram();
	BlockDatabase::get().textureAtlas.bindTexture();

	m_shader.loadLighting(g_info.lighting);

	m_model->bindVAO();
	GL::drawElements(m_model->getIndicesCount());
}
