#pragma once

class DroppedItemsManager;
class DroppedItemsMesh;
class DroppedItem;

class DroppedItemsBuilder
{
public:
	DroppedItemsBuilder(DroppedItemsManager &droppedItemsManager, DroppedItemsMesh &droppedItemsMesh);

	void buildMesh();
private:

	DroppedItemsManager *m_pDroppedItemsManager = nullptr;
	DroppedItemsMesh *m_pDroppedItemsMesh = nullptr;
};