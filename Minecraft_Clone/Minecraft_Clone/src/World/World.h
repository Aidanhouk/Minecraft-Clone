#pragma once

#include <atomic>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>
#include <list>

#include "../Util/NonCopyable.h"
#include "Chunk/Chunk.h"
#include "Chunk/ChunkManager.h"

#include "Event/IWorldEvent.h"
#include "Item/DroppedItem/DroppedItemsManager.h"

#include "../Config.h"

class RenderMaster;
class Camera;
class Player;
class World;

struct Entity;

struct UnloadedBlock
{
	sf::Vector3i pos;
	ChunkBlock block;

	friend bool operator== (const UnloadedBlock &block1, const UnloadedBlock &block2);
};

class World : public NonCopyable {
public:
    World(const Camera &camera, const Config &config, Player &player);
    ~World();

    ChunkBlock getBlock(int x, int y, int z);
    void setBlock(int x, int y, int z, ChunkBlock block);
	void addUnloadedBlock(int x, int y, int z, ChunkBlock block);
	void setAllUnloadedBlocks();

	void reloadChunks() { m_reloadChunks = true; }
    void update(Player &player, float dt);
    void updateChunk(int blockX, int blockY, int blockZ);

    void renderWorld(RenderMaster &master, const Camera &camera);

    ChunkManager &getChunkManager();
	DroppedItemsManager &getDroppedItemsManager() { return m_droppedItemManager; }

    static VectorXZ getBlockXZ(int x, int z);
    static VectorXZ getChunkXZ(int x, int z);

    template <typename T, typename... Args> void addEvent(Args &&... args)
    {
        m_events.push_back(std::make_unique<T>(std::forward<Args>(args)...));
    }

	void addDroppedItem(const ItemStack& itemStack, const glm::vec3& pos);
	void blockBroken(const glm::vec3& pos, World &world);
	void checkForDroppedItems(const glm::vec3& pos);

	BiomeId getBiomeId(int x, int z);
private:
    void loadChunks(const Camera &camera);
    void updateChunks();
    void setSpawnPoint();

    ChunkManager m_chunkManager;

    std::vector<std::unique_ptr<IWorldEvent>> m_events;
    std::unordered_map<sf::Vector3i, ChunkSection *> m_chunkUpdates;

    std::atomic<bool> m_isRunning{true};
    std::vector<std::thread> m_chunkLoadThreads;
    std::mutex m_mainMutex;
    std::mutex m_genMutex;

    int m_loadDistance = 2;
    int m_renderDistance;
	bool m_reloadChunks = false;

    glm::vec3 m_playerSpawnPoint;

	DroppedItemsManager m_droppedItemManager;

	std::vector<UnloadedBlock> m_unloadedBlocks;
};