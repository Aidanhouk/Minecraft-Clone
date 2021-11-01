#pragma once

#include "Util/Random.h"

#include "Shaders/Environment/RainShader.h"
#include "Model.h"
#include "Camera.h"
#include "Texture/BasicTexture.h"

class Camera;

class Rain {
public:
	Rain();

	void Render(const Camera& camera, glm::vec3 position, float precipitationVisibility);
private:
	void addVertexCoord(std::vector<GLfloat> &verteciesMesh,
		const std::vector<GLfloat> &verteciesTexture, float biasX, float biasZ);

	RainShader m_shader;
	Model m_snowModel;
	BasicTexture m_texture;

	Random<std::minstd_rand> m_random;

	glm::vec3 m_oldPos;
	float m_movement = 0.0f;
};