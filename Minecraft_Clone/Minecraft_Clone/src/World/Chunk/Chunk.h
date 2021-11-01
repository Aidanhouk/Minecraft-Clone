#pragma once

#include "../../Util/Array2D.h"
#include "../../Util/NonCopyable.h"
#include "ChunkSection.h"

#include <vector>

class RenderMaster;
class Camera;
class TerrainGenerator;

class Chunk : public IChunk {
public:
	Chunk() = default;
	Chunk(const sf::Vector2i &location, World* world);

	Chunk(const Chunk& x) = delete;
	Chunk(Chunk&& x);

	Chunk& operator=(const Chunk& x) = delete;
	Chunk& operator=(Chunk&& x);

	bool makeMesh(const Camera &camera);

	void setBlock(int x, int y, int z, ChunkBlock block) override;
	ChunkBlock getBlock(int x, int y, int z) const noexcept override;
	int getHeightAt(int x, int z);

	void drawChunks(RenderMaster &renderer, const Camera &camera);

	bool hasLoaded() const noexcept;
	void load(TerrainGenerator &generator);

	ChunkSection &getSection(int index);

	const sf::Vector2i &getLocation() const
	{
	    return m_location;
	}

	void deleteMeshes();
private:
	void addSectionsBlockTarget(int blockY);
	void addSectionsIndexTarget(int index);
	void addSection();
	
	bool outOfBound(int x, int y, int z) const noexcept;
	sf::Vector2i toWorldPosition(int x, int z) const;
	
	std::vector<ChunkSection> m_chunksSection;
	Array2D<int, CHUNK_SIZE> m_highestBlocks;
	sf::Vector2i m_location;
	
	// better get rid of it later
	World *m_pWorld;
	
	bool m_isLoaded = false;
};