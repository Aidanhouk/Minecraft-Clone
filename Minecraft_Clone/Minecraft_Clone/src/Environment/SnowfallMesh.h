#pragma once

#include "Mesh.h"

struct SnowfallMesh : public Mesh
{
	std::vector<GLfloat> directions;
};