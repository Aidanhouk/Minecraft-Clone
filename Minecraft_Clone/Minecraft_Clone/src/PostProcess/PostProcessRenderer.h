#pragma once

#include <vector>

#include "../Model.h"
#include "../Maths/glm.h"
#include "../Shaders/PostProcessShader.h"
#include "../Texture/BasicTexture.h"
#include "../Config.h"
#include "../ShaderData.h"
#include "Framebuffer.h"
#include "Bloom.h"
#include "Antialiasing.h"   
#include "MBlur.h"

class Camera;

class PostProcessRender
{
public:
	PostProcessRender();
		
	void add(const glm::vec3& position);
	void render(const Camera& camera, FrameBufferObject& fbo);
private:
	void begin();
	void finalize();
	std::vector<glm::vec3> m_quads;

	Model m_quadModel;
	PostProcessShader m_shader;
	Bloom bloom;
	Antialiasing antialias;
	MBlur mblur;
};