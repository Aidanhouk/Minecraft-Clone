#include "World.h"

#include "Camera.h"

void World::addUnloadedBlock(int x, int y, int z, ChunkBlock block)
{
	UnloadedBlock unloadedBlock{ { x,y,z }, block };
	m_unloadedBlocks.emplace_back(unloadedBlock);
}

void World::setAllUnloadedBlocks()
{
	for (auto iter = m_unloadedBlocks.begin(); iter != m_unloadedBlocks.end(); ++iter) {
#ifdef _DEBUG
		if (iter->block.getId() >= (Block_t)BlockId::NUM_TYPES) {
			continue;
		}
#endif // _DEBUG

		auto bp = getBlockXZ(iter->pos.x, iter->pos.z);
		auto chunkPosition = getChunkXZ(iter->pos.x, iter->pos.z);

		m_mainMutex.lock();
		auto &chunk = m_chunkManager.getChunk(chunkPosition.x, chunkPosition.z);
		chunk.addBusyLevel();
		if (chunk.hasLoaded()) {
			chunk.setBlockInChunk(bp.x, iter->pos.y, bp.z, iter->block);
			m_mainMutex.unlock();
			updateAmbientOcclusion(bp.x, iter->pos.y, bp.z, &chunk);

			// @TODO update lighting
			// doesn't work
			//updateSunLight(iter->pos.x, iter->pos.y, iter->pos.z);
			//chunk.shouldUpdateMesh();
			// @TODO update adjacent chunks
			// doesn't work
			//updateChunk(iter->pos.x, iter->pos.y, iter->pos.z);
		}
		else {
			m_mainMutex.unlock();
			iter->pos.x = bp.x;
			iter->pos.z = bp.z;
			chunk.addUnloadedBlock(std::move(*iter));
		}
		chunk.subtractBusyLevel();
	}

	m_unloadedBlocks.clear();
	m_unloadedBlocks.shrink_to_fit();
}

void World::loadChunks(const Camera &camera)
{
	std::vector<Chunk*> m_chunksToUpdate;

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
					if (x >= 0 && z >= 0) {
						auto &chunk = m_chunkManager.getChunk(x, z);
						chunk.addBusyLevel();
						m_chunksToUpdate.emplace_back(&chunk);
					}
				}
			}
		}

		setAllUnloadedBlocks();

		for (auto & chunk : m_chunksToUpdate) {
			if (!chunk->isSunLightLoaded())
				calculateSunLight(chunk);
			if (!chunk->isAmbientOcclusionLoaded())
				calculateAmbientOcclusion(chunk);
			std::unique_lock<std::mutex> lock(m_mainMutex);
			chunk->makeMesh(camera);
			chunk->subtractBusyLevel();
		}
		m_chunksToUpdate.clear();
		m_chunksToUpdate.shrink_to_fit();

		++m_loadDistance;
		if (m_loadDistance >= m_renderDistance) {
			m_loadDistance = 2;
		}
	}
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