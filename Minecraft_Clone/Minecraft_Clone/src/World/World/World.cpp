#include "World.h"

#include "Maths/Vector2XZ.h"
#include "Player/Player.h"
#include "Renderer/RenderMaster.h"
#include "Util/Random.h"

#include "../Chunk/ChunkMesh/ChunkMeshBuilder.h"

#include <future>
#include <iostream>
#include <vector>

bool operator== (const UnloadedBlock &block1, const UnloadedBlock &block2)
{
	return (block1.pos.x == block2.pos.x &&
		block1.pos.y == block2.pos.y &&
		block1.pos.z == block2.pos.z);
}

World::World(const Camera &camera, const Config &config, Player &player)
	: m_chunkManager(this)
	, m_renderDistance(config.renderDistance)
	, m_droppedItemManager(this)
{
	setSpawnPoint();
	player.position = m_playerSpawnPoint;

	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	m_unloadedBlocks.reserve(100000);
	m_chunkLoadThreads.emplace_back([&]() { loadChunks(camera); });
}

World::~World()
{
	m_isRunning = false;
	for (auto &thread : m_chunkLoadThreads) {
		thread.join();
	}
}

ChunkBlock World::getBlock(int x, int y, int z)
{
	auto bp = getBlockXZ(x, z);
	auto chunkPosition = getChunkXZ(x, z);
	Chunk &chunk = m_chunkManager.getChunk(chunkPosition.x, chunkPosition.z);

	chunk.addBusyLevel();
	if (chunk.hasLoaded()) {
		auto block = chunk.getBlockInChunk(bp.x, y, bp.z);
		chunk.subtractBusyLevel();
		return block;
	}
	else {
		chunk.subtractBusyLevel();
		return BlockId::Air;
	}
}

std::pair<Chunk*, ChunkBlock*> World::getBlockRef(int x, int y, int z)
{
	//@TODO fix this one day
	static ChunkBlock dummy(0);

	if (x < 0 || z < 0) {
		return { nullptr, &dummy };
	}

	auto bp = getBlockXZ(x, z);
	auto chunkPosition = getChunkXZ(x, z);
	Chunk * chunk = &m_chunkManager.getChunk(chunkPosition.x, chunkPosition.z);
	chunk->addBusyLevel();
	if (chunk->hasLoaded()) {
		auto &bl = chunk->getBlockInChunkRef(bp.x, y, bp.z);
		chunk->subtractBusyLevel();
		return { chunk, &bl };
	}
	else {
		chunk->subtractBusyLevel();
		return { chunk, &dummy };
	}
}

void World::setBlock(int x, int y, int z, ChunkBlock block)
{
	if (y < 0)
		return;

	updateChunk(x, y, z);

	auto bp = getBlockXZ(x, z);
	auto chunkPosition = getChunkXZ(x, z);
	auto &chunk = m_chunkManager.getChunk(chunkPosition.x, chunkPosition.z);

	auto lightChar = chunk.getBlock(bp.x, y, bp.z).light;
	block.light = lightChar;

	if (block.getData().isOpaque &&
		block.getData().id != BlockId::Glowstone) {

		if ((int)(lightChar & 0xF) > 0) {
			removeTorchLight(x, y, z);
			updateLitChunks(x, y, z);
		}
		if ((int)((lightChar >> 4) & 0xF) > 0) {
			removeSunLight(x, y, z);
		}
	}

	chunk.setBlock(bp.x, y, bp.z, block);

	updateAmbientOcclusion(bp.x, y, bp.z, &chunk);

	if (block == 0)
		m_droppedItemManager.blockBrokenUpdate({ x, y, z }, *this);
}

void World::update(Player &player, float dt)
{
	m_droppedItemManager.update(player, dt);

	if (m_reloadChunks) {
		m_reloadChunks = false;
		m_renderDistance = g_Config.renderDistance;
		// formula is not that good, would be nice to hardcore every render distance option
		g_Config.fogDensity = 0.014f * 8.0f / g_Config.renderDistance;
		m_loadDistance = 2;
		std::unique_lock<std::mutex> lock(m_genMutex);
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
	m_droppedItemManager.checkForDroppedItems(pos);
}

BiomeId World::getBiomeId(int x, int z)
{
	return m_chunkManager.getBiomeId(x, z);
}

void World::updateChunk(int blockX, int blockY, int blockZ)
{
	std::unique_lock<std::mutex> lock(m_mainMutex);

	auto addChunkToUpdateBatch = [&](const sf::Vector3i &key, ChunkSection &section) {
		if (m_chunkUpdates.find(key) == m_chunkUpdates.end())
			m_chunkUpdates.emplace(key, &section);
	};

	auto chunkPosition = getChunkXZ(blockX, blockZ);
	auto chunkSectionY = blockY / CHUNK_SIZE;

	auto sectionBlockXZ = getBlockXZ(blockX, blockZ);
	auto sectionBlockY = blockY % CHUNK_SIZE;

	int xx = 0, yy = 0, zz = 0;
	int sectionsX = 1, sectionsY = 1, sectionsZ = 1;

	if (sectionBlockXZ.x == 0) {
		xx = -1;
		++sectionsX;
	}
	else if (sectionBlockXZ.x == CHUNK_SIZE - 1) {
		xx = 1;
		++sectionsX;
	}

	if (sectionBlockY == 0) {
		yy = -1;
		++sectionsY;
	}
	else if (sectionBlockY == CHUNK_SIZE - 1) {
		yy = 1;
		++sectionsY;
	}

	if (sectionBlockXZ.z == 0) {
		zz = -1;
		++sectionsZ;
	}
	else if (sectionBlockXZ.z == CHUNK_SIZE - 1) {
		zz = 1;
		++sectionsZ;
	}

	for (int x = chunkPosition.x, secX = 0; secX++ < sectionsX; x += xx)
		for (int y = chunkSectionY, secY = 0; secY++ < sectionsY; y += yy)
			for (int z = chunkPosition.z, secZ = 0; secZ++ < sectionsZ; z += zz) {
				if (x >= 0 && z >= 0) {
					sf::Vector3i newKey(x, y, z);
					//addChunkToUpdateBatch(newKey,
					//	m_chunkManager.getChunk(newKey.x, newKey.z).getSection(newKey.y));
					Chunk &chunk = m_chunkManager.getChunk(newKey.x, newKey.z);
					if (chunk.hasLoaded())
						addChunkToUpdateBatch(newKey, chunk.getSection(newKey.y));
				}
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
			if (itr->second.getBusyLevel() == 0)
				itr = chunkMap.erase(itr);
			else
				++itr;
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