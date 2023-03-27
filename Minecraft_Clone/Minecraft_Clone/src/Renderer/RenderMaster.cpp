#include "RenderMaster.h"

#include <SFML/Graphics.hpp>
#include <iostream>

#include "../Application.h"
#include "../Context.h"
#include "../World/Chunk/ChunkMesh/ChunkMesh.h"
#include "../World/Chunk/ChunkSection.h"

#include "UserSettings/Config.h"
#include "../RenderSettings.h"
#include "../ShaderData.h"
#include "Inventory/IconsMesh.h"
#include "Player/PlayerInfo.h"

#include "World/Block/BlockDatabase.h"

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
	m_2D_Renderer.addMeshToDraw(iconsMesh);
}

void RenderMaster::drawDroppedItems(const DroppedItemsMesh & droppedItemsMesh)
{
	m_droppedItemsRenderer.addMeshToDraw(droppedItemsMesh);
}

void RenderMaster::drawBreakingBlock()
{
	m_breakBlockRenderer.addMeshToDraw();
}

void RenderMaster::drawHand(const HandMesh & handMesh)
{
	m_handRenderer.addMeshToDraw(handMesh);
}

void RenderMaster::finishRender(sf::RenderWindow &window, const Camera &camera)
{
	bool postProc = g_Config.postProcess;
	if (postProc) {
		m_fbo.bind();
		m_fbo.clear();
	}
	else {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	//if (g_Info.fog) {
	//	m_sky->renderSkyBox(camera);
	//	m_chunkRenderer.render(camera);
	//	m_delineateBlockRenderer.render(camera);
	//	m_floraRenderer.render(camera);
	//	m_droppedItemsRenderer.render(camera);
	//	m_breakBlockRenderer.render(camera);
	//	m_waterRenderer.render(camera);
	//	m_sky->render(camera);
	//}
	//else {
		m_chunkRenderer.render(camera);
		m_delineateBlockRenderer.render(camera);
		m_floraRenderer.render(camera);
		m_droppedItemsRenderer.render(camera);
		m_waterRenderer.render(camera);
		m_sky->render(camera);
		m_breakBlockRenderer.render(camera);
	//}
	
	m_handRenderer.render();
	if (postProc) {
		m_postRenderer.render(camera, m_fbo);
	}
	
	m_sfmlRenderer.render(window);
	m_2D_Renderer.render();
	m_sfmlRenderer.renderOverInterface(window);
	
	window.display();
}