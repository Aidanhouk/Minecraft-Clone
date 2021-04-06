#pragma once

#include "Model.h"

#include <SFML/Graphics.hpp>
#include <array>

class IconsMesh {
public:
	IconsMesh() = default;

	void addIcon(const std::array<GLfloat, 12> &iconPosition,
		const std::array<GLfloat, 8> &textureCoords);

	void bufferMesh();

	const Model &getModel() const;

	void deleteData();
private:
	Mesh m_mesh;
	Model m_model;
	GLuint m_indexIndex = 0;
};