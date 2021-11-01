#pragma once

#include "Mesh.h"

struct SnowfallMesh : public Mesh
{
	SnowfallMesh(
		std::vector<GLfloat>& vertexPositions,
		std::vector<GLfloat>& textureCoords,
		std::vector<GLuint>& indices,
		std::vector<GLfloat>& directions);
	std::vector<GLfloat> directions;
};