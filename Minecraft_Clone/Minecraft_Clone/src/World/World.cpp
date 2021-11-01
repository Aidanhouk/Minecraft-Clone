#include "World.h"

#include <future>
#include <iostream>
#include <vector>

#include "../Camera.h"
#include "../Maths/Vector2XZ.h"
#include "../Player/Player.h"
#include "../Renderer/RenderMaster.h"
#include "../Util/Random.h"

bool operator== (const UnloadedBlock &block1, const UnloadedBlock &block2)
{
	return (block1.pos.x == block2.pos.x &&
		block1.pos.y == block2.pos.y &&
		block1.pos.z == block2.pos.z);
}

World::World(const Camera &camera, const Config &config, Player &player)
	: m_chunkManager(this)
	, m_renderDistance(config.renderDistance)
{
	setSpawnPoint();
	player.position = m_playerSpawnPoint;

	//std::this_thread::sleep_for(std::chrono::milliseconds(50));
	m_unloadedBlocks.reserve(30000);
	m_chunkLoadThreads.emplace_back([&]() { loadChunks(camera); });
}

World::~World()
{
	m_isRunning = false;
	for (auto &thread : m_chunkLoadThreads) {
		thread.join();
	}
}

// world coords into chunk column coords
ChunkBlock World::getBlock(int x, int y, int z)
{
	auto bp = getBlockXZ(x, z);
	auto chunkPosition = getChunkXZ(x, z);

	Chunk &chunk = m_chunkManager.getChunk(chunkPosition.x, chunkPosition.z);
	if (chunk.hasLoaded())
		return chunk.getBlock(bp.x, y, bp.z);
	else
		return BlockId::Air;
}

void World::setBlock(int x, int y, int z, ChunkBlock block)
{
	if (y < 0)
		return;

	updateChunk(x, y, z);

	auto bp = getBlockXZ(x, z);
	auto chunkPosition = getChunkXZ(x, z);

	m_chunkManager.getChunk(chunkPosition.x, chunkPosition.z)
		.setBlock(bp.x, y, bp.z, block);

	if (block == 0)
		m_droppedItemManager.blockBrokenUpdate({ x, y, z }, *this);
}

void World::addUnloadedBlock(int x, int y, int z, ChunkBlock block)
{
	UnloadedBlock unloadedBlock{ { x,y,z }, block };
	if (std::find(m_unloadedBlocks.begin(), m_unloadedBlocks.end(), unloadedBlock) == m_unloadedBlocks.end())
		m_unloadedBlocks.emplace_back(unloadedBlock);
}

void World::setAllUnloadedBlocks()
{
	for (auto iter = m_unloadedBlocks.begin(); iter != m_unloadedBlocks.end(); ++iter) {
		auto bp = getBlockXZ(iter->pos.x, iter->pos.z);
		auto chunkPosition = getChunkXZ(iter->pos.x, iter->pos.z);

		std::unique_lock<std::mutex> lock(m_mainMutex); // Change locking place
		m_chunkManager.getChunk(chunkPosition.x, chunkPosition.z)
			.setBlock(bp.x, iter->pos.y, bp.z, iter->block);
	}

	m_unloadedBlocks.clear();
}

// loads chunks and make chunk meshes
void World::update(Player &player, float dt)
{
	m_droppedItemManager.update(player, *this, dt);

	if (m_reloadChunks) {
		m_reloadChunks = false;
		m_renderDistance = g_Config.renderDistance;
		// formula is not that good, would be nice to hardcore every render distance option
		g_Config.fogDensity = 0.014f * 8.0f / g_Config.renderDistance;
		m_loadDistance = 2;
		std::unique_lock<std::mutex> lock(m_mainMutex);
		m_chunkManager.deleteMeshes();
	}

	for (auto &event : m_events) {
		event->handle(*this);
	}
	m_events.clear();

	updateChunks();
}

void World::addDroppedItem(const ItemStack & itemStack, const glm::vec3 & pos)
{
	m_droppedItemManager.addItem(itemStack, pos);
}

void World::blockBroken(const glm::vec3 & pos, World &world)
{
	m_droppedItemManager.blockBrokenUpdate(pos, world);
}

void World::checkForDroppedItems(const glm::vec3 & pos)
{
	m_droppedItemManager.checkForDroppedItems(pos, *this);
}

BiomeId World::getBiomeId(int x, int z)
{
	return m_chunkManager.getBiomeId(x, z);
}

//@TODO
// Optimize for chunkPositionU usage :thinking:
void World::loadChunks(const Camera &camera)
{
	while (m_isRunning) {
		int cameraX = camera.position.x / CHUNK_SIZE;
		int cameraZ = camera.position.z / CHUNK_SIZE;

		for (int i = 0; i < m_loadDistance; i++) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			int minX = std::max(cameraX - i, 0);
			int minZ = std::max(cameraZ - i, 0);
			int maxX = cameraX + i;
			int maxZ = cameraZ + i;

			for (int x = minX; x < maxX; ++x) {
				for (int z = minZ; z < maxZ; ++z) {
					m_chunkManager.generateTerrain(x, z);
					m_chunksToUpdate.emplace_back(x, z);
				}
			}
		}

		setAllUnloadedBlocks();

		for (auto & var : m_chunksToUpdate) {
			std::unique_lock<std::mutex> lock(m_mainMutex);
			m_chunkManager.getChunk(var.x, var.y).makeMesh(camera);
		}
		m_chunksToUpdate.clear();
		m_chunksToUpdate.shrink_to_fit();

		++m_loadDistance;

		if (m_loadDistance >= m_renderDistance)
			m_loadDistance = 2;
	}
}

void World::updateChunk(int blockX, int blockY, int blockZ)
{
	std::unique_lock<std::mutex> lock(m_mainMutex);

	auto addChunkToUpdateBatch = [&](const sf::Vector3i &key,
		ChunkSection &section) {
		m_chunkUpdates.emplace(key, &section);
	};

	auto chunkPosition = getChunkXZ(blockX, blockZ);
	auto chunkSectionY = blockY / CHUNK_SIZE;

	sf::Vector3i key(chunkPosition.x, chunkSectionY, chunkPosition.z);

	addChunkToUpdateBatch(
		key, m_chunkManager.getChunk(chunkPosition.x, chunkPosition.z)
		.getSection(chunkSectionY));

	auto sectionBlockXZ = getBlockXZ(blockX, blockZ);
	auto sectionBlockY = blockY % CHUNK_SIZE;

	// for correct chunk render
	if (sectionBlockXZ.x == 0) {
		sf::Vector3i newKey(chunkPosition.x - 1, chunkSectionY, chunkPosition.z);
		addChunkToUpdateBatch(
			newKey,
			m_chunkManager.getChunk(newKey.x, newKey.z).getSection(newKey.y));
	}
	else if (sectionBlockXZ.x == CHUNK_SIZE - 1) {
		sf::Vector3i newKey(chunkPosition.x + 1, chunkSectionY, chunkPosition.z);
		addChunkToUpdateBatch(
			newKey,
			m_chunkManager.getChunk(newKey.x, newKey.z).getSection(newKey.y));
	}

	if (sectionBlockY == 0) {
		sf::Vector3i newKey(chunkPosition.x, chunkSectionY - 1, chunkPosition.z);
		addChunkToUpdateBatch(
			newKey,
			m_chunkManager.getChunk(newKey.x, newKey.z).getSection(newKey.y));
	}
	else if (sectionBlockY == CHUNK_SIZE - 1) {
		sf::Vector3i newKey(chunkPosition.x, chunkSectionY + 1, chunkPosition.z);
		addChunkToUpdateBatch(
			newKey,
			m_chunkManager.getChunk(newKey.x, newKey.z).getSection(newKey.y));
	}

	if (sectionBlockXZ.z == 0) {
		sf::Vector3i newKey(chunkPosition.x, chunkSectionY, chunkPosition.z - 1);
		addChunkToUpdateBatch(
			newKey,
			m_chunkManager.getChunk(newKey.x, newKey.z).getSection(newKey.y));
	}
	else if (sectionBlockXZ.z == CHUNK_SIZE - 1) {
		sf::Vector3i newKey(chunkPosition.x, chunkSectionY, chunkPosition.z + 1);
		addChunkToUpdateBatch(
			newKey,
			m_chunkManager.getChunk(newKey.x, newKey.z).getSection(newKey.y));
	}
}

void World::renderWorld(RenderMaster &renderer, const Camera &camera)
{
	m_droppedItemManager.addToRender(renderer);

	std::unique_lock<std::mutex> lock(m_mainMutex);

	auto &chunkMap = m_chunkManager.getChunks();
	for (auto itr = chunkMap.begin(); itr != chunkMap.end();) {
		Chunk &chunk = itr->second;

		int cameraX = camera.position.x;
		int cameraZ = camera.position.z;

		int minX = (cameraX / CHUNK_SIZE) - m_renderDistance;
		int minZ = (cameraZ / CHUNK_SIZE) - m_renderDistance;
		int maxX = (cameraX / CHUNK_SIZE) + m_renderDistance;
		int maxZ = (cameraZ / CHUNK_SIZE) + m_renderDistance;

		auto location = chunk.getLocation();

		// delete chunk if it's far from player
		if (minX > location.x || maxX < location.x ||
			minZ > location.y || maxZ < location.y) {
			itr = chunkMap.erase(itr);
			continue;
		}
		else {
			chunk.drawChunks(renderer, camera);
			++itr;
		}
	}
}

ChunkManager &World::getChunkManager()
{
	return m_chunkManager;
}

VectorXZ World::getBlockXZ(int x, int z)
{
	return { x % CHUNK_SIZE, z % CHUNK_SIZE };
}

VectorXZ World::getChunkXZ(int x, int z)
{
	return { x / CHUNK_SIZE, z / CHUNK_SIZE };
}

void World::updateChunks()
{
	std::unique_lock<std::mutex> lock(m_mainMutex);
	for (auto &c : m_chunkUpdates) {
		ChunkSection &s = *c.second;
		s.makeMesh();
	}
	m_chunkUpdates.clear();
}

void World::setSpawnPoint()
{
	int chunkX = -1;
	int chunkZ = -1;
	int blockX = 0;
	int blockZ = 0;
	int blockY = 0;

	auto h = m_chunkManager.getTerrainGenerator().getMinimumSpawnHeight();

	while (blockY <= h) {
		{
			std::unique_lock<std::mutex> lock(m_mainMutex);
			m_chunkManager.unloadChunk(chunkX, chunkZ);
		}

		chunkX = RandomSingleton::get().intInRange(100, 200);
		chunkZ = RandomSingleton::get().intInRange(100, 200);
		blockX = RandomSingleton::get().intInRange(0, 15);
		blockZ = RandomSingleton::get().intInRange(0, 15);

		m_chunkManager.loadChunk(chunkX, chunkZ);

		blockY = m_chunkManager.getChunk(chunkX, chunkZ).getHeightAt(blockX, blockZ);
	}

	int worldX = chunkX * CHUNK_SIZE + blockX;
	int worldZ = chunkZ * CHUNK_SIZE + blockZ;
	m_playerSpawnPoint = { worldX + 0.5f, blockY + 2.0f, worldZ + 0.5f };

	for (int x = worldX - 1; x <= worldX + 1; ++x) {
		for (int z = worldZ - 1; z < worldZ + 1; ++z) {
			std::unique_lock<std::mutex> lock(m_mainMutex);
			m_chunkManager.loadChunk(x, z);
		}
	}
}
