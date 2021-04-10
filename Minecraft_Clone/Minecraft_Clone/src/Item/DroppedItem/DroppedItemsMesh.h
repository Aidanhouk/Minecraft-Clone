#pragma once

#include "Model.h"

#include "Maths/glm.h"
#include <array>

class DroppedItemsMesh {
public:
	DroppedItemsMesh() = default;

	void addItem(const std::array<GLfloat, 12> &verticesPosition,
		const std::array<GLfloat, 8> &textureCoords,
		const glm::vec3 &itemPosition);

	void bufferMesh();

	const Model &getModel() const;

	void deleteData();
private:
	Mesh m_mesh;
	Model m_model;
	GLuint m_indexIndex = 0;
};