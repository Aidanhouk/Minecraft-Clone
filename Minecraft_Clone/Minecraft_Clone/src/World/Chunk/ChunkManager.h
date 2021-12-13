#pragma once

#include "SFML/System/Vector2.hpp"
#include "Maths/Vector2XZ.h"
#include "../Generation/Terrain/TerrainGenerator.h"
#include "Chunk.h"

#include <functional>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

class World;

using ChunkMap = std::unordered_map<VectorXZ, Chunk>;

class ChunkManager {
public:
	ChunkManager(World *world);
	
	Chunk &getChunk(int x, int z);
	ChunkMap &getChunks();
	
	void deleteMeshes();
	
	bool chunkLoadedAt(int x, int z) const;
	bool chunkExistsAt(int x, int z) const;
	
	void generateTerrain(int x, int z);
	void loadChunk(int x, int z);
	void unloadChunk(int x, int z);
	
	const TerrainGenerator &getTerrainGenerator() const noexcept;

	BiomeId getBiomeId(int x, int z);
private:
	ChunkMap m_chunks;
	std::unique_ptr<TerrainGenerator> m_terrainGenerator;

	mutable std::mutex m_genMutex;
	
	World *m_pWorld;
};