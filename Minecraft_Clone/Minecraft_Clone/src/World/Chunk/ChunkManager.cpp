#include "ChunkManager.h"

#include "../Generation/Terrain/ClassicOverWorldGenerator.h"
#include "../Generation/Terrain/SuperFlatGenerator.h"
#include "Item/DroppedItem/DroppedItemsManager.h"

#include <iostream>

ChunkManager::ChunkManager(World *world)
    : m_pWorld(world)
{
	m_terrainGenerator = std::make_unique<ClassicOverWorldGenerator>();
	//m_terrainGenerator = std::make_unique<SuperFlatGenerator>();
}

Chunk &ChunkManager::getChunk(int x, int z)
{
    VectorXZ key{x, z};
    if (!chunkExistsAt(x, z)) {
		// sometimes debugger stops here if transfer { x,z } as an r-value... Probably not now
		sf::Vector2i vector{ x,z };
		Chunk chunk{ std::move(vector), m_pWorld };
        m_chunks.emplace(key, std::move(chunk));
    }

    return m_chunks[key];
}

ChunkMap &ChunkManager::getChunks()
{
    return m_chunks;
}

void ChunkManager::deleteMeshes()
{
	for (auto &chunk : m_chunks) {
		chunk.second.deleteMeshes();
	}
}

bool ChunkManager::chunkLoadedAt(int x, int z) const
{
    if (!chunkExistsAt(x, z))
        return false;

    return m_chunks.at({x, z}).hasLoaded();
}

bool ChunkManager::chunkExistsAt(int x, int z) const
{
    return m_chunks.find({x, z}) != m_chunks.end();
}

//bool ChunkManager::generateTerrain(int x, int z, const Camera &camera)
void ChunkManager::generateTerrain(int x, int z)
{
	// @TODO fix this
	// skip chunks close to zero coords
	// there also chunks with big coords... x and z = 1600+
	static bool skipChunks = true;

	for (int nx = -1; nx <= 1; ++nx)
		for (int nz = -1; nz <= 1; ++nz) {
			if (skipChunks) {
				if (x > 5 && z > 5)
					skipChunks = false;
			}
			else
				loadChunk(x + nx, z + nz);
		}
}

void ChunkManager::loadChunk(int x, int z)
{
	//getChunk(x, z).load(*m_terrainGenerator);
	Chunk &ch = getChunk(x, z);
	if (ch.hasLoaded())
		return;

	Chunk newChunk({ x,z }, m_pWorld);
	newChunk.load(*m_terrainGenerator);

	std::unique_lock<std::mutex> lock(m_mainMutex);
	ch = std::move(newChunk);
}

void ChunkManager::unloadChunk(int x, int z)
{
    //@TODO Save chunk to file
	if (chunkExistsAt(x, z)) {
		m_chunks.erase({ x, z });
	}
}

const TerrainGenerator &ChunkManager::getTerrainGenerator() const noexcept
{
	return *m_terrainGenerator;
}

BiomeId ChunkManager::getBiomeId(int x, int z)
{
	return m_terrainGenerator->getBiomeId(getChunk(x / CHUNK_SIZE, z / CHUNK_SIZE), x, z);
}
