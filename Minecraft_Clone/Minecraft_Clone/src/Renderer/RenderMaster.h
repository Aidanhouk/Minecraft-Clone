#ifndef RENDERMASTER_H_INCLUDED
#define RENDERMASTER_H_INCLUDED

#include <SFML/Graphics.hpp>

#include "../Config.h"
#include "ChunkRenderer.h"
#include "FloraRenderer.h"
#include "SFMLRenderer.h"
#include "WaterRenderer.h"
#include "Renderer_2D.h"

#include "../States/PlayingState.h"
#include "../PostProcess/Framebuffer.h"
#include "../PostProcess/PostProcessRenderer.h"

class Camera;
class ChunkSection;
class ItemIconsMesh;

class RenderMaster {
public:
	RenderMaster();

	void drawSFML(const sf::Drawable &drawable);
	void drawSFMLOverInterface(const sf::Drawable &drawable);
    void addChunkToRender(const ChunkSection &chunk);
	void drawIcons(const IconsMesh &iconsMesh);

    void finishRender(sf::RenderWindow &window, const Camera &camera);

private:
	//Primitives
	PostProcessRender m_postRenderer;

    // Chunks
    ChunkRenderer m_chunkRenderer;
    WaterRenderer m_waterRenderer;
    FloraRenderer m_floraRenderer;

    // Detail
    SFMLRenderer m_sfmlRenderer;

	Renderer_2D m_2D_Renderer;

	FrameBufferObject m_fbo;
};

#endif // RENDERMASTER_H_INCLUDED
