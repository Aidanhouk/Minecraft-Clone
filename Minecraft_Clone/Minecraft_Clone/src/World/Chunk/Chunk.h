#pragma once

#include "Util/Array2D.h"
#include "Util/NonCopyable.h"
#include "ChunkSection.h"
#include "UnloadedBlock.h"
#include "../Block/LiquidBlock.h"

#include <vector>
#include <unordered_map>

class RenderMaster;
class Camera;
class TerrainGenerator;

class Chunk
{

public:
	Chunk() = default;
	Chunk(const sf::Vector2i &location, World* world);

	Chunk(const Chunk& x) = delete;
	Chunk(Chunk&& x);

	Chunk& operator=(const Chunk& x) = delete;
	Chunk& operator=(Chunk&& x);

public:
	void makeMesh(const Camera &camera);

	void setBlock(int x, int y, int z, ChunkBlock block, uint8_t waterLevel = 0);
	// use this function if you 100% know the block is in this chunk
	void setBlockInChunk(int x, int y, int z, ChunkBlock block, uint8_t waterLevel = 0);
    inline void setLiquidBlock(ChunkBlock* block, uint8_t waterLevel);
	ChunkBlock getBlock(int x, int y, int z) const noexcept;
	ChunkBlock getBlockInChunk(int x, int y, int z) const;
	ChunkBlock& getBlockInChunkRef(int x, int y, int z);
	int getHeightAt(int x, int z);
	World *getWorldPtr() { return m_pWorld; }
	int getChunkSectionsSize() { return m_chunkSections.size(); }
    std::unordered_map<ChunkBlock*, LiquidBlock>& getLiqiudBlocks() { return m_liquidBlocks; }

	void addUnloadedBlock(UnloadedBlock&& block);
	void setUnloadedBlocks(Chunk &chunk);

	void drawChunks(RenderMaster &renderer, const Camera &camera);

	bool hasLoaded() const noexcept;
	void load(TerrainGenerator &generator);

	ChunkSection &getSection(int index);

	const sf::Vector2i &getLocation() const { return m_location; }

	void deleteMeshes();

	void shouldUpdateMesh() { m_updateMesh = true; }

public:
	void setTorchLight(int x, int y, int z, int value);
	int getTorchLight(int x, int y, int z);
	void setSunLight(int x, int y, int z, int value);
	int getSunLight(int x, int y, int z);

	void setSunLightLoaded(bool f) { m_sunLightLoaded = f; }
	bool isSunLightLoaded() { return m_sunLightLoaded; }

	void setAmbientOcclusionLoaded(bool f) { m_ambientOcclusionLoaded = f; }
	bool isAmbientOcclusionLoaded() { return m_ambientOcclusionLoaded; }

public:
	void addBusyLevel() { ++m_busyLevel; }
	void subtractBusyLevel() { --m_busyLevel; }
	int getBusyLevel() { return m_busyLevel; }

private:
	void addSectionsBlockTarget(int blockY);
	void addSectionsIndexTarget(int index);
	void addSection();

	bool outOfBounds(int x, int y, int z) const noexcept;
	bool outOfBounds(int x, int z) const noexcept;
	bool outOfHeightBounds(int y) const noexcept;
	sf::Vector2i toWorldPosition(int x, int z) const;
	
private:
	std::vector<ChunkSection> m_chunkSections;
	Array2D<int, CHUNK_SIZE> m_highestBlocks;
	sf::Vector2i m_location;
	std::vector<UnloadedBlock> m_unloadedBlocks;
    std::unordered_map<ChunkBlock*, LiquidBlock> m_liquidBlocks;
	
	// better get rid of it later
	World *m_pWorld;
	
	int m_busyLevel = 0;

	bool m_isLoaded = false;
	bool m_updateMesh = false;
	bool m_sunLightLoaded = false;
	bool m_ambientOcclusionLoaded = false;
};