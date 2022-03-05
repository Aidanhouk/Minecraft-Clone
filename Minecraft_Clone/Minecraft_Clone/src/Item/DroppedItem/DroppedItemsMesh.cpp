#include "DroppedItemsMesh.h"

#include <iostream>

void DroppedItemsMesh::addFace(
	const std::array<GLfloat, 12> &verticesPosition,
	const std::array<GLfloat, 8> &textureCoords,
	const glm::vec3 &itemPosition,
	GLfloat torchLight,
	GLfloat sunlight)
{
	auto &verticies = m_mesh.vertexPositions;
	auto &texCoords = m_mesh.textureCoords;
	auto &indices = m_mesh.indices;

	texCoords.insert(texCoords.end(), textureCoords.begin(), textureCoords.end());

	for (int i = 0, index = 0; i < 4; ++i) {
		verticies.push_back(verticesPosition[index++] + itemPosition.x);
		verticies.push_back(verticesPosition[index++] + itemPosition.y);
		verticies.push_back(verticesPosition[index++] + itemPosition.z);
		m_torchLight.push_back(torchLight);
		m_sunLight.push_back(sunlight);
	}

	indices.insert(indices.end(),
		{ m_indexIndex, m_indexIndex + 1, m_indexIndex + 2,
		 m_indexIndex + 2, m_indexIndex + 3, m_indexIndex });
	m_indexIndex += 4;
}

void DroppedItemsMesh::bufferMesh()
{
	m_model.addData(m_mesh);
	m_model.addVBO(1, m_torchLight);
	m_model.addVBO(1, m_sunLight);

	m_mesh.vertexPositions.clear();
	m_mesh.textureCoords.clear();
	m_mesh.indices.clear();
	m_torchLight.clear();
	m_sunLight.clear();

	m_mesh.vertexPositions.shrink_to_fit();
	m_mesh.textureCoords.shrink_to_fit();
	m_mesh.indices.shrink_to_fit();
	m_torchLight.shrink_to_fit();
	m_sunLight.shrink_to_fit();

	m_indexIndex = 0;
}

void DroppedItemsMesh::deleteData()
{
	m_model.deleteData();
}

const Model &DroppedItemsMesh::getModel() const
{
	return m_model;
}