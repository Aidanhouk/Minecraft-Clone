#include "IconsMesh.h"

#include <iostream>

void IconsMesh::addIcon(const std::array<GLfloat, 12> &iconPosition,
	const std::array<GLfloat, 8> &textureCoords)
{
	auto &verticies = m_mesh.vertexPositions;
	auto &texCoords = m_mesh.textureCoords;
	auto &indices = m_mesh.indices;

	verticies.insert(verticies.end(), iconPosition.begin(), iconPosition.end());

	texCoords.insert(texCoords.end(), textureCoords.begin(), textureCoords.end());

	indices.insert(indices.end(),
		{ m_indexIndex, m_indexIndex + 1, m_indexIndex + 2,
		 m_indexIndex + 2, m_indexIndex + 3, m_indexIndex });
	m_indexIndex += 4;
}

void IconsMesh::bufferMesh()
{
	m_model.addData(m_mesh);

	m_indexIndex = 0;
}

void IconsMesh::deleteData()
{
	m_mesh.vertexPositions.clear();
	m_mesh.textureCoords.clear();
	m_mesh.indices.clear();
	
	m_mesh.vertexPositions.shrink_to_fit();
	m_mesh.textureCoords.shrink_to_fit();
	m_mesh.indices.shrink_to_fit();

	//m_model.deleteData();
}

const Model &IconsMesh::getModel() const
{
	return m_model;
}