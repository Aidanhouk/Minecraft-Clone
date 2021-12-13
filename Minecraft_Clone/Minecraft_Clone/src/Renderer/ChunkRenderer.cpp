#include "ChunkRenderer.h"

#include "../World/Block/BlockDatabase.h"
#include "../World/Chunk/ChunkMesh/ChunkMesh.h"

#include "../Camera.h"
#include "GlobalInfo.h"

#include <iostream>

void ChunkRenderer::add(const ChunkMesh &mesh)
{
    m_chunks.push_back(&mesh.getModel().getRenderInfo());
}

void ChunkRenderer::render(const Camera &camera)
{
    if (m_chunks.empty()) {
        return;
    }

	if (g_Info.fog) {
		glEnable(GL_BLEND);
	}
	else {
		glDisable(GL_BLEND);
	}
    glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

    m_shader.useProgram();
    BlockDatabase::get().textureAtlas.bindTexture();

	m_shader.loadProjectionMatrix(camera.getProjMatrix());
	m_shader.loadViewMatrix(camera.getViewMatrix());
	m_shader.loadDTime(g_Info.dayTime);
	m_shader.loadLighting(g_Info.lighting);
	m_shader.loadFog(g_Info.fog);
	m_shader.loadFogDensity(g_Config.fogDensity);

    for (auto mesh : m_chunks) {
        GL::bindVAO(mesh->vao);
        GL::drawElements(mesh->indicesCount);
    }

    m_chunks.clear();
}
