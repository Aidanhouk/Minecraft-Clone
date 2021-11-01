#include "Snowfall.h"

#include <vector>
#include "SkyManager.h"
#include "../Application.h"

#include <iostream>

Snowfall::Snowfall()
	: m_texture("Environment/snow")
{
	const float bottom = -2.0f;
	const float top = 6.8f;

	const std::vector<GLfloat> frontCoords{
		-1.1f,	bottom,	-1.1f,
		 1.1f,	bottom,	-1.1f,
		 1.1f,	top,	-1.1f,
		-1.1f,	top,	-1.1f,
	};
	const std::vector<GLfloat> frontRightCoords{
		 0.0f,	bottom,	-1.5f,
		 1.5f,	bottom,	0.0f,
		 1.5f,	top,	0.0f,
		 0.0f,	top,	-1.5f,
	};
	const std::vector<GLfloat> rightCoords{
		 1.1f,	bottom,	 1.1f,
		 1.1f,	bottom,	-1.1f,
		 1.1f,	top,	-1.1f,
		 1.1f,	top,	 1.1f,
	};
	const std::vector<GLfloat> rightBackCoords{
		 0.0f,	bottom,	1.5f,
		 1.5,	bottom,	0.0f,
		 1.5f,	top,	0.0f,
		 0.0f,	top,	1.5f,
	};
	const std::vector<GLfloat> backCoords{
		-1.1f,	bottom,	1.1f,
		 1.1f,	bottom,	1.1f,
		 1.1f,	top,	1.1f,
		-1.1f,	top,	1.1f,
	};
	const std::vector<GLfloat> backLeftCoords{
		 0.0f,	bottom,	1.5f,
		 -1.5f,	bottom,	0.0f,
		 -1.5f,	top,	0.0f,
		 0.0f,	top,	1.5f,
	};
	const std::vector<GLfloat> leftCoords{
		-1.1f,	bottom,	 1.1f,
		-1.1f,	bottom,	-1.1f,
		-1.1f,	top,	-1.1f,
		-1.1f,	top,	 1.1f,
	};
	const std::vector<GLfloat> leftFrontCoords{
		 0.0f,	bottom,	-1.5f,
		 -1.5f,	bottom,	0.0f,
		 -1.5f,	top,	0.0f,
		 0.0f,	top,	-1.5f,
	};

	std::vector<GLfloat> vertexCoords;
	vertexCoords.reserve(12 * 8 * 25);
	int numberOfTextures = 0;
	int bias;
	// Distance between textures
	const float distBetwTex = 2.0f;

	for (int i = -7; i <= 7; ++i) {
		for (int j = -7; j <= 7; ++j) {
			if (glm::abs(i + j) % 2 == 1)
				continue;
			if (glm::abs(i) + glm::abs(j) > 10)
				continue;
			//if (glm::abs(i) < 5 && glm::abs(j) < 5)
			//	continue;
			++numberOfTextures;
			addVertexCoord(vertexCoords, frontCoords,
				j * distBetwTex,
				i * distBetwTex);

			++numberOfTextures;
			addVertexCoord(vertexCoords, frontRightCoords,
				j * distBetwTex,
				i * distBetwTex);

			++numberOfTextures;
			addVertexCoord(vertexCoords, rightCoords,
				j * distBetwTex,
				i * distBetwTex);

			++numberOfTextures;
			addVertexCoord(vertexCoords, rightBackCoords,
				j * distBetwTex,
				i * distBetwTex);

			++numberOfTextures;
			addVertexCoord(vertexCoords, backCoords,
				j * distBetwTex,
				i * distBetwTex);

			++numberOfTextures;
			addVertexCoord(vertexCoords, backLeftCoords,
				j * distBetwTex,
				i * distBetwTex);

			++numberOfTextures;
			addVertexCoord(vertexCoords, leftCoords,
				j * distBetwTex,
				i * distBetwTex);

			++numberOfTextures;
			addVertexCoord(vertexCoords, leftFrontCoords,
				j * distBetwTex,
				i * distBetwTex);
		}
	}

	const std::vector<GLfloat> texCoord{
		0, 1,
		1, 1,
		1, 0,
		0, 0,
	};

	std::vector<GLfloat> textureCoords;
	for (int i = 0; i < numberOfTextures; ++i) {
		textureCoords.insert(textureCoords.end(), texCoord.begin(), texCoord.end());
	}

	std::vector<GLuint> indexCoords;
	for (int i = 0; i < numberOfTextures; ++i) {
		unsigned int bias = i * 4;
		std::vector<GLuint> newIndCoords{
			0 + bias, 1 + bias, 2 + bias,
			2 + bias, 3 + bias, 0 + bias };
		indexCoords.insert(indexCoords.end(), newIndCoords.begin(), newIndCoords.end());
	}

	std::vector<GLfloat> directions;
	float moveX, moveY;
	for (int i = 0; i < numberOfTextures; ++i) {
		moveX = m_random.intInRange(-10, 10) / 20.0f;
		do
			moveY = m_random.intInRange(-5, 1) / 20.0f;
		while (moveY == 0.0f);
		// for each vertex the same bias
		std::vector<GLfloat> dirCoords{
			moveX, moveY,
			moveX, moveY,
			moveX, moveY,
			moveX, moveY,
		};
		directions.insert(directions.end(), dirCoords.begin(), dirCoords.end());
	}

	m_snowModel.addSnowfallData({ vertexCoords, textureCoords, indexCoords, directions });
}

void Snowfall::addVertexCoord(std::vector<GLfloat> &verteciesMesh,
	const std::vector<GLfloat> &verteciesTexture, float biasX, float biasZ)
{
	for (int j = 0; j < 12; ++j) {
		// x
		if (j % 3 == 0)
			verteciesMesh.emplace_back(verteciesTexture[j] + biasX);
		// z
		else if ((j + 1) % 3 == 0)
			verteciesMesh.emplace_back(verteciesTexture[j] + biasZ);
		// y
		else
			verteciesMesh.emplace_back(verteciesTexture[j]);
	}
}

void Snowfall::Render(const Camera& camera, glm::vec3 pos, float precipitationVisibility)
{
	m_shader.useProgram();
	m_snowModel.bindVAO();
	m_texture.bindTexture();

	m_movement += g_Info.deltaTime / 6.0f;

	if (m_oldPos == glm::vec3(0.0f) && pos != glm::vec3(0.0f)) {
		m_oldPos = pos;
	}

	if (abs(pos.x - m_oldPos.x) + abs(pos.z - m_oldPos.z) > 8.0f) {
		m_oldPos.x = pos.x;
		m_oldPos.z = pos.z;
	}
	if (abs(pos.y - m_oldPos.y) > 1.5f) {
		m_oldPos.y = pos.y;
	}

	m_shader.loadModelMatrix(glm::translate(glm::mat4(1.0f),
		glm::vec3(m_oldPos.x, m_oldPos.y, m_oldPos.z)));
	m_shader.loadProjectionViewMatrix(camera.getProjectionViewMatrix());

	m_shader.loadTime(m_movement);
	m_shader.loadLighting(g_Info.lighting);
	m_shader.loadVisibility(precipitationVisibility);

	GL::drawElements(m_snowModel.getIndicesCount());
}
