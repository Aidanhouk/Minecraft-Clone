#pragma once

#include <atomic>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>
#include <list>
#include <queue>

#include "Util/NonCopyable.h"
#include "Item/DroppedItem/DroppedItemsManager.h"

#include "../Chunk/Chunk.h"
#include "../Chunk/ChunkManager.h"
#include "../Chunk/UnloadedBlock.h"
#include "../Event/IWorldEvent.h"

#include "../Config.h"

class RenderMaster;
class Camera;
class Player;
class World;

struct Entity;

class World : public NonCopyable {
public:
    World(const Camera &camera, const Config &config, Player &player);
    ~World();

	World(const World& x) = delete;
	World& operator=(const World& x) = delete;

	ChunkBlock getBlock(int x, int y, int z);
	// Do not use if not sure chunk is loaded
	ChunkBlock& getBlockRef(int x, int y, int z);
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

    template <typename T, typename... Args>
	void addEvent(Args &&... args)
    {
        m_events.push_back(std::make_unique<T>(std::forward<Args>(args)...));
    }

	void addDroppedItem(const ItemStack& itemStack, const glm::vec3& pos);
	void blockBroken(const glm::vec3& pos, World &world);
	void checkForDroppedItems(const glm::vec3& pos);

	BiomeId getBiomeId(int x, int z);

	void setTorchLight(int x, int y, int z, int lightValue);
	void removeTorchLight(int x, int y, int z);
	int getTorchLight(int x, int y, int z);
	void updateLitChunks(int x, int y, int z);

	void calculateSunLight(Chunk * chunk);
	void updateSunLight(int x, int y, int z);
	void removeSunLight(int x, int y, int z);

	void updateAmbientOcclusion(int x, int y, int z, Chunk * chunk);
private:
	struct LightNode {
		LightNode(short x, short y, short z, Chunk* ch)
			: x(x), y(y), z(z), chunk(ch) {}
		short x, y, z;
		Chunk* chunk;
	};
	void tryAddLightNode(int x, int y, int z, int lightLevel, Chunk * chunk);
	void propagateLight();
	struct LightRemovalNode {
		LightRemovalNode(short x, short y, short z, short value, Chunk* ch)
			: x(x), y(y), z(z), value(value), chunk(ch) {}
		short x, y, z;
		short value;
		Chunk* chunk;
	};
	void tryAddLightRemovalNode(int x, int y, int z, int lightLevel, Chunk * chunk);
	void unPropagateLight();

	void propagateSunLight();
	// this function is like the one above but it also updates all chunk meshes affected by sunlight
	void propagateSunLightUpdate();
	void tryAddSunLightNode(int x, int y, int z, int lightLevel, Chunk * chunk);
	void unPropagateSunLight();
	void tryAddSunLightRemovalNode(int x, int y, int z, int lightLevel, Chunk * chunk);

	void calculateAmbientOcclusion(Chunk * chunk);
	void calculateAO_Top(int x, int y, int z, Chunk * chunk, ChunkBlock &block);
	void calculateAO_Bottom(int x, int y, int z, Chunk * chunk, ChunkBlock &block);
	void calculateAO_Left(int x, int y, int z, Chunk * chunk, ChunkBlock &block);
	void calculateAO_Right(int x, int y, int z, Chunk * chunk, ChunkBlock &block);
	void calculateAO_Back(int x, int y, int z, Chunk * chunk, ChunkBlock &block);
	void calculateAO_Front(int x, int y, int z, Chunk * chunk, ChunkBlock &block);
	int vertexAO(bool side1, bool side2, bool corner);



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

	std::queue<LightNode> m_lightQueue;
	std::queue<LightRemovalNode> m_lightRemovalQueue;

	std::queue<LightNode> m_sunLightQueue;
	std::queue<LightRemovalNode> m_sunLightRemovalQueue;
};