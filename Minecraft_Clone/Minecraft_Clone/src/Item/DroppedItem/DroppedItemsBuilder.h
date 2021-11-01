#pragma once

#include "Maths/glm.h"

class DroppedItemsManager;
class DroppedItemsMesh;
class DroppedItem;
class World;
struct ChunkBlock;

class DroppedItemsBuilder
{
public:
	DroppedItemsBuilder(DroppedItemsManager &droppedItemsManager, DroppedItemsMesh &droppedItemsMesh);

	void buildMesh(World *world);
private:
	void buildCubeMesh(ChunkBlock& block, glm::vec3& position);
	void buildCactusMesh(ChunkBlock& block, glm::vec3& position);
	void buildDefaultItemMesh(ChunkBlock& block, glm::vec3& position);

	DroppedItemsManager *m_pDroppedItemsManager = nullptr;
	DroppedItemsMesh *m_pDroppedItemsMesh = nullptr;
};