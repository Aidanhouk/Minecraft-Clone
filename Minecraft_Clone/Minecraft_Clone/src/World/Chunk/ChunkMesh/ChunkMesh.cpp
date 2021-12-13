#include "ChunkMesh.h"

#include "../../World/WorldConstants.h"

void ChunkMesh::addFace(
	const std::array<GLfloat, 12> &blockFace,
	const std::array<GLfloat, 8> &textureCoords,
	const sf::Vector3i &chunkPosition,
	const sf::Vector3i &blockPosition,
	GLfloat torchLight,
	GLfloat sunLight,
	const std::array<GLfloat, 4> &ambientOcclusion)
{
	++faces;
	auto &verticies = m_mesh.vertexPositions;
	auto &texCoords = m_mesh.textureCoords;
	auto &indices = m_mesh.indices;

	texCoords.insert(texCoords.end(), textureCoords.begin(), textureCoords.end());

	// Vertex: The current vertex in the "blockFace" vector, 4 vertex in total
	// hence "< 4" Index: X, Y, Z
	for (int i = 0, index = 0; i < 4; ++i) {
		verticies.push_back(blockFace[index++] + chunkPosition.x * CHUNK_SIZE + blockPosition.x);
		verticies.push_back(blockFace[index++] + chunkPosition.y * CHUNK_SIZE + blockPosition.y);
		verticies.push_back(blockFace[index++] + chunkPosition.z * CHUNK_SIZE + blockPosition.z);
		m_torchLight.push_back(torchLight);
		m_sunLight.push_back(sunLight);
		m_ambientOcclusion.push_back(ambientOcclusion[i]);
	}

	if (ambientOcclusion[0] + ambientOcclusion[2] < ambientOcclusion[1] + ambientOcclusion[3]) {
		indices.insert(indices.end(),
			{ m_indexIndex + 3, m_indexIndex, m_indexIndex + 1,
			 m_indexIndex + 1, m_indexIndex + 2, m_indexIndex + 3});
	}
	else {
		indices.insert(indices.end(),
			{ m_indexIndex, m_indexIndex + 1, m_indexIndex + 2,
			 m_indexIndex + 2, m_indexIndex + 3, m_indexIndex });
	}

	m_indexIndex += 4;
}

void ChunkMesh::bufferMesh()
{
	m_model.addData(m_mesh);
	m_model.addVBO(1, m_ambientOcclusion);
	m_model.addVBO(1, m_torchLight);
	m_model.addVBO(1, m_sunLight);

	m_mesh.vertexPositions.clear();
	m_mesh.textureCoords.clear();
	m_mesh.indices.clear();
	m_ambientOcclusion.clear();
	m_torchLight.clear();
	m_sunLight.clear();

	m_mesh.vertexPositions.shrink_to_fit();
	m_mesh.textureCoords.shrink_to_fit();
	m_mesh.indices.shrink_to_fit();
	m_ambientOcclusion.shrink_to_fit();
	m_torchLight.shrink_to_fit();
	m_sunLight.shrink_to_fit();

	m_indexIndex = 0;
}

void ChunkMesh::deleteData()
{
	m_model.deleteData();
}

const Model &ChunkMesh::getModel() const
{
	return m_model;
}
