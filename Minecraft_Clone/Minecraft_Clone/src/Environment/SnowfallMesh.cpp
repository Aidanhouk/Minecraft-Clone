#include "SnowfallMesh.h"

SnowfallMesh::SnowfallMesh(
	std::vector<GLfloat>& vertexPositions,
	std::vector<GLfloat>& textureCoords,
	std::vector<GLuint>& indices,
	std::vector<GLfloat>& directions)
	: Mesh{ vertexPositions,textureCoords, indices }, directions{ directions }
{
}
