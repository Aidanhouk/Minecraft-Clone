#pragma once

#include "../../Block/ChunkBlock.h"

#include "GL/glew.h"
#include <SFML/Graphics.hpp>

#include <vector>

class ChunkSection;
class ChunkMesh;
class BlockData;

struct ChunkMeshCollection;
struct BlockDataHolder;

class ChunkMeshBuilder {
public:
    ChunkMeshBuilder(ChunkSection &chunk, ChunkMeshCollection &meshes);

    void buildMesh();

private:
    void setActiveMesh(ChunkBlock block);

	void calculateAO_Top(int x, int y, int z, std::array<GLfloat, 4> &verticesAO);
	void calculateAO_Bottom(int x, int y, int z, std::array<GLfloat, 4> &verticesAO);
	void calculateAO_Left(int x, int y, int z, std::array<GLfloat, 4> &verticesAO);
	void calculateAO_Right(int x, int y, int z, std::array<GLfloat, 4> &verticesAO);
	void calculateAO_Front(int x, int y, int z, std::array<GLfloat, 4> &verticesAO);
	void calculateAO_Back(int x, int y, int z, std::array<GLfloat, 4> &verticesAO);

	float vertexAO(bool side1, bool side2, bool corner);

	void tryAddFaceToMesh(const std::array<GLfloat, 12> &blockFace,
		const sf::Vector2i &textureCoords,
		const sf::Vector3i &blockPosition,
		const sf::Vector3i &blockFacing,
		const std::array<GLfloat, 4> &ambientOcclusion,
		GLfloat torchLight,
		GLfloat sunlight);

    bool shouldMakeFace(const sf::Vector3i &adjBlockPos,
                        const BlockDataHolder &blockData);

    bool shouldMakeLayer(int y);

    const ChunkBlock *m_pBlockPtr = nullptr;
    ChunkSection *m_pChunkSection = nullptr;
    ChunkMeshCollection *m_pMeshes = nullptr;
    ChunkMesh *m_pActiveMesh = nullptr;
    const BlockDataHolder *m_pBlockData = nullptr;
};