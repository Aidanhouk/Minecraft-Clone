#pragma once

#include "Model.h"

#include "Maths/glm.h"
#include <array>

class DroppedItemsMesh {
public:

	DroppedItemsMesh() = default;

	void addFace(
		const std::array<GLfloat, 12> &verticesPosition,
		const std::array<GLfloat, 8> &textureCoords,
		const glm::vec3 &itemPosition,
		GLfloat torchLight,
		GLfloat sunlight);

	void bufferMesh();

	const Model &getModel() const;

	void deleteData();
private:
	Mesh m_mesh;
	Model m_model;
	GLuint m_indexIndex = 0;

	std::vector<GLfloat> m_torchLight;
	std::vector<GLfloat> m_sunLight;
};