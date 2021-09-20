#include "RenderMaster.h"

#include <SFML/Graphics.hpp>
#include <iostream>

#include "../Application.h"
#include "../Context.h"
#include "../World/Chunk/ChunkMesh.h"
#include "../World/Chunk/ChunkSection.h"

#include "../Config.h"
#include "../RenderSettings.h"
#include "../ShaderData.h"
#include "Inventory/IconsMesh.h"
#include "Player/PlayerInfo.h"

RenderMaster::RenderMaster()
	: m_fbo(0, 0)
{}

void RenderMaster::drawSFML(const sf::Drawable &drawable)
{
    m_sfmlRenderer.add(drawable);
}

void RenderMaster::drawSFMLOverInterface(const sf::Drawable & drawable)
{
	m_sfmlRenderer.addOverInterface(drawable);
}

void RenderMaster::addChunkToRender(const ChunkSection &chunk)
{
    const auto &solidMesh = chunk.getMeshes().solidMesh;
    const auto &waterMesh = chunk.getMeshes().waterMesh;
    const auto &floraMesh = chunk.getMeshes().floraMesh;

    if (solidMesh.faces > 0)
        m_chunkRenderer.add(solidMesh);

    if (waterMesh.faces > 0)
        m_waterRenderer.add(waterMesh);

    if (floraMesh.faces > 0)
        m_floraRenderer.add(floraMesh);
}

void RenderMaster::drawIcons(const IconsMesh & iconsMesh)
{
	m_2D_Renderer.setMeshToDraw(iconsMesh);
}

void RenderMaster::drawDroppedItems(const DroppedItemsMesh & droppedItemsMesh)
{
	m_droppedItemsRenderer.setMeshToDraw(droppedItemsMesh);
}

void RenderMaster::drawBreakingBlock()
{
	m_breakBlockRenderer.setMeshToDraw();
}

void RenderMaster::setHandModel(Model & model)
{
	m_handRenderer.setHandModel(model);
}

void RenderMaster::finishRender(sf::RenderWindow &window, const Camera &camera)
{
	bool postProc = g_Config.postProcess;
	if (postProc) {
		m_fbo.bind();
		m_fbo.clear();
	}
	else {
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	
	m_chunkRenderer.render(camera);
	m_delineateBlockRenderer.render(camera);
	m_floraRenderer.render(camera);
	m_droppedItemsRenderer.render(camera);
	m_waterRenderer.render(camera);
	m_sky->render(camera);
	m_breakBlockRenderer.render(camera);
	
	if (postProc)
		m_postRenderer.render(camera, m_fbo);
	m_handRenderer.render();
	
	m_sfmlRenderer.render(window);
	m_2D_Renderer.render();
	m_sfmlRenderer.renderOverInterface(window);
	
	window.display();
}