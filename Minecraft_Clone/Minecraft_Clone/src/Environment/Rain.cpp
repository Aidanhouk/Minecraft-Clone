#include "Rain.h"

#include <vector>
#include "SkyManager.h"
#include "../Application.h"

#include <iostream>

Rain::Rain()
	: m_texture("Environment/rain")
{
	const float bottom = -2.0f;
	const float top = 6.8f;

	const std::vector<GLfloat> frontCoords{
		-1.1f,	0.0f + bottom,	-1.1f,
		 1.1f,	0.0f + bottom,	-1.1f,
		 1.1f,	0.0f + top,	-1.1f,
		-1.1f,	0.0f + top,	-1.1f,
	};
	const std::vector<GLfloat> frontRightCoords{
		 0.0f,	0.1f + bottom,	-1.5f,
		 1.5f,	0.1f + bottom,	0.0f,
		 1.5f,	0.1f + top,	0.0f,
		 0.0f,	0.1f + top,	-1.5f,
	};
	const std::vector<GLfloat> rightCoords{
		 1.1f,	0.2f + bottom,	 1.1f,
		 1.1f,	0.2f + bottom,	-1.1f,
		 1.1f,	0.2f + top,	-1.1f,
		 1.1f,	0.2f + top,	 1.1f,
	};
	const std::vector<GLfloat> rightBackCoords{
		 0.0f,	0.3f + bottom,	1.5f,
		 1.5,	0.3f + bottom,	0.0f,
		 1.5f,	0.3f + top,	0.0f,
		 0.0f,	0.3f + top,	1.5f,
	};
	const std::vector<GLfloat> backCoords{
		-1.1f,	0.4f + bottom,	1.1f,
		 1.1f,	0.4f + bottom,	1.1f,
		 1.1f,	0.4f + top,	1.1f,
		-1.1f,	0.4f + top,	1.1f,
	};
	const std::vector<GLfloat> backLeftCoords{
		 0.0f,	0.5f + bottom,	1.5f,
		 -1.5f,	0.5f + bottom,	0.0f,
		 -1.5f,	0.5f + top,	0.0f,
		 0.0f,	0.5f + top,	1.5f,
	};
	const std::vector<GLfloat> leftCoords{
		-1.1f,	0.6f + bottom,	 1.1f,
		-1.1f,	0.6f + bottom,	-1.1f,
		-1.1f,	0.6f + top,	-1.1f,
		-1.1f,	0.6f + top,	 1.1f,
	};
	const std::vector<GLfloat> leftFrontCoords{
		 0.0f,	0.7f + bottom,	-1.5f,
		 -1.5f,	0.7f + bottom,	0.0f,
		 -1.5f,	0.7f + top,	0.0f,
		 0.0f,	0.7f + top,	-1.5f,
	};

	std::vector<GLfloat> vertexCoords;
	vertexCoords.reserve(12 * 8 * 30);
	int numberOfTextures = 0;
	int bias;
	// Distance between textures
	const float distBetwTex = 0.5f;
	// Distance between diagonal textures
	const float diagTexDist = distBetwTex * 0.707f;
	//const int biasCoeffMin = 0, biasCoeffMax = 0;
	const int biasCoeffMin = -10, biasCoeffMax = 10;
	
	for (int i = 0; i < 30; ++i) {
		++numberOfTextures;
		bias = m_random.intInRange(biasCoeffMin, biasCoeffMax);
		addVertexCoord(vertexCoords, frontCoords,
			+bias * distBetwTex,
			-i * distBetwTex);

		++numberOfTextures;
		bias = m_random.intInRange(biasCoeffMin, biasCoeffMax);
		addVertexCoord(vertexCoords, frontRightCoords,
			+i * diagTexDist + bias * diagTexDist,
			-i * diagTexDist + bias * diagTexDist);

		++numberOfTextures;
		bias = m_random.intInRange(biasCoeffMin, biasCoeffMax);
		addVertexCoord(vertexCoords, rightCoords,
			+i * distBetwTex,
			+bias * distBetwTex);

		++numberOfTextures;
		bias = m_random.intInRange(biasCoeffMin, biasCoeffMax);
		addVertexCoord(vertexCoords, rightBackCoords,
			+i * diagTexDist + bias * diagTexDist,
			+i * diagTexDist - bias * diagTexDist);

		++numberOfTextures;
		bias = m_random.intInRange(biasCoeffMin, biasCoeffMax);
		addVertexCoord(vertexCoords, backCoords,
			+bias * distBetwTex,
			+i * distBetwTex);

		++numberOfTextures;
		bias = m_random.intInRange(biasCoeffMin, biasCoeffMax);
		addVertexCoord(vertexCoords, backLeftCoords,
			-i * diagTexDist + bias * diagTexDist,
			+i * diagTexDist + bias * diagTexDist);

		++numberOfTextures;
		bias = m_random.intInRange(biasCoeffMin, biasCoeffMax);
		addVertexCoord(vertexCoords, leftCoords,
			-i * distBetwTex,
			+bias * distBetwTex);

		++numberOfTextures;
		bias = m_random.intInRange(biasCoeffMin, biasCoeffMax);
		addVertexCoord(vertexCoords, leftFrontCoords,
			-i * diagTexDist + bias * diagTexDist,
			-i * diagTexDist - bias * diagTexDist);
	}

	/*
		++numberOfTextures;
		bias = m_random.intInRange(biasCoeffMin, biasCoeffMax);
		// front
		for (int j = 0; j < 12; ++j) {
			// x
			if (j % 3 == 0)
				vertexCoords.emplace_back(frontCoords[j] + bias * distBetwTex);
			// z
			else if ((j + 1) % 3 == 0)
				vertexCoords.emplace_back(frontCoords[j] - i * distBetwTex);
			else
				vertexCoords.emplace_back(frontCoords[j]);
		}

		++numberOfTextures;
		bias = m_random.intInRange(biasCoeffMin, biasCoeffMax);
		// front-right
		for (int j = 0; j < 12; ++j) {
			// x
			if (j % 3 == 0)
				vertexCoords.emplace_back(frontRightCoords[j] + i * diagTexDist + bias * diagTexDist);
			// z
			else if ((j + 1) % 3 == 0)
				vertexCoords.emplace_back(frontRightCoords[j] - i * diagTexDist + bias * diagTexDist);
			else
				vertexCoords.emplace_back(frontRightCoords[j]);
		}

		++numberOfTextures;
		bias = m_random.intInRange(biasCoeffMin, biasCoeffMax);
		// right
		for (int j = 0; j < 12; ++j) {
			// x
			if (j % 3 == 0)
				vertexCoords.emplace_back(rightCoords[j] + i * distBetwTex);
			// z
			else if ((j + 1) % 3 == 0)
				vertexCoords.emplace_back(rightCoords[j] + bias * distBetwTex);
			else
				vertexCoords.emplace_back(rightCoords[j]);
		}

		++numberOfTextures;
		bias = m_random.intInRange(biasCoeffMin, biasCoeffMax);
		// right-back
		for (int j = 0; j < 12; ++j) {
			// x
			if (j % 3 == 0)
				vertexCoords.emplace_back(rightBackCoords[j] + i * diagTexDist + bias * diagTexDist);
			// z
			else if ((j + 1) % 3 == 0)
				vertexCoords.emplace_back(rightBackCoords[j] + i * diagTexDist - bias * diagTexDist);
			else
				vertexCoords.emplace_back(rightBackCoords[j]);
		}

		++numberOfTextures;
		bias = m_random.intInRange(biasCoeffMin, biasCoeffMax);
		// back
		for (int j = 0; j < 12; ++j) {
			// x
			if (j % 3 == 0)
				vertexCoords.emplace_back(backCoords[j] + bias * distBetwTex);
			// z
			else if ((j + 1) % 3 == 0)
				vertexCoords.emplace_back(backCoords[j] + i * distBetwTex);
			else
				vertexCoords.emplace_back(backCoords[j]);
		}

		++numberOfTextures;
		bias = m_random.intInRange(biasCoeffMin, biasCoeffMax);
		// back-left
		for (int j = 0; j < 12; ++j) {
			// x
			if (j % 3 == 0)
				vertexCoords.emplace_back(backLeftCoords[j] - i * diagTexDist + bias * diagTexDist);
			// z
			else if ((j + 1) % 3 == 0)
				vertexCoords.emplace_back(backLeftCoords[j] + i * diagTexDist + bias * diagTexDist);
			else
				vertexCoords.emplace_back(backLeftCoords[j]);
		}

		++numberOfTextures;
		bias = m_random.intInRange(biasCoeffMin, biasCoeffMax);
		// left
		for (int j = 0; j < 12; ++j) {
			// x
			if (j % 3 == 0)
				vertexCoords.emplace_back(leftCoords[j] - i * distBetwTex);
			// z
			else if ((j + 1) % 3 == 0)
				vertexCoords.emplace_back(leftCoords[j] + bias * distBetwTex);
			else
				vertexCoords.emplace_back(leftCoords[j]);
		}

		++numberOfTextures;
		bias = m_random.intInRange(biasCoeffMin, biasCoeffMax);
		// left-front
		for (int j = 0; j < 12; ++j) {
			// x
			if (j % 3 == 0)
				vertexCoords.emplace_back(leftFrontCoords[j] - i * diagTexDist + bias * diagTexDist);
			// z
			else if ((j + 1) % 3 == 0)
				vertexCoords.emplace_back(leftFrontCoords[j] - i * diagTexDist - bias * diagTexDist);
			else
				vertexCoords.emplace_back(leftFrontCoords[j]);
		}
	}
	*/

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

	m_snowModel.addData({ vertexCoords, textureCoords, indexCoords });
}

void Rain::addVertexCoord(std::vector<GLfloat> &verteciesMesh,
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

void Rain::Render(const Camera& camera, glm::vec3 pos, float precipitationVisibility)
{
	m_shader.useProgram();
	m_snowModel.bindVAO();
	m_texture.bindTexture();
	m_movement += g_Info.deltaTime * 6.0f;

	if (m_movement > 2.0f) {
		m_movement = 0.0f;
	}
	if (m_oldPos == glm::vec3(0.0f) && pos != glm::vec3(0.0f)) {
		m_oldPos = pos;
	}

	if (abs(pos.x - m_oldPos.x) + abs(pos.z - m_oldPos.z) > 5.0f) {
		m_oldPos.x = pos.x;
		m_oldPos.z = pos.z;
	}
	if (abs(pos.y - m_oldPos.y) > 1.5f) {
		m_oldPos.y = pos.y;
	}

	m_shader.loadModelMatrix(glm::translate(glm::mat4(1.0f),
		glm::vec3(m_oldPos.x, m_oldPos.y - m_movement, m_oldPos.z)));
	m_shader.loadProjectionViewMatrix(camera.getProjectionViewMatrix());
	m_shader.loadLighting(g_Info.lighting);
	m_shader.loadVisibility(precipitationVisibility);

	GL::drawElements(m_snowModel.getIndicesCount());
}
