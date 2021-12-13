#pragma once

#include "Model.h"

#include <SFML/Graphics.hpp>
#include <array>
#include <vector>

class ChunkMesh {
public:
	ChunkMesh() = default;

	void addFace(
		const std::array<GLfloat, 12> &blockFace,
		const std::array<GLfloat, 8> &textureCoords,
		const sf::Vector3i &chunkPosition,
		const sf::Vector3i &blockPosition,
		GLfloat torchLight,
		GLfloat sunLight,
		const std::array<GLfloat, 4> &ambientOcclusion = { 1.0f,1.0f,1.0f,1.0f });

	void bufferMesh();

	const Model &getModel() const;

	void deleteData();

	int faces = 0;

private:
	Mesh m_mesh;
	Model m_model;
	std::vector<GLfloat> m_torchLight;
	std::vector<GLfloat> m_sunLight;
	std::vector<GLfloat> m_ambientOcclusion;
	GLuint m_indexIndex = 0;
};

struct ChunkMeshCollection {
	ChunkMesh solidMesh;
	ChunkMesh waterMesh;
	ChunkMesh floraMesh;
};