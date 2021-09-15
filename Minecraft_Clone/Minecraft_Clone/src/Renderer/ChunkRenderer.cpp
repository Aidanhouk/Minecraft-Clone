#include "ChunkRenderer.h"

#include "../World/Block/BlockDatabase.h"
#include "../World/Chunk/ChunkMesh.h"

#include "../Camera.h"
#include "GlobalInfo.h"

void ChunkRenderer::add(const ChunkMesh &mesh)
{
    m_chunks.push_back(&mesh.getModel().getRenderInfo());
}

void ChunkRenderer::render(const Camera &camera)
{
    if (m_chunks.empty()) {
        return;
    }

    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);

    m_shader.useProgram();
    BlockDatabase::get().textureAtlas.bindTexture();

	m_shader.loadProjectionMatrix(camera.getProjMatrix());
	m_shader.loadViewMatrix(camera.getViewMatrix());
	m_shader.loadDTime(g_info.dayTime);
	m_shader.loadLighting(g_info.lighting);

    for (auto mesh : m_chunks) {
        GL::bindVAO(mesh->vao);
        GL::drawElements(mesh->indicesCount);
    }

    m_chunks.clear();
}
