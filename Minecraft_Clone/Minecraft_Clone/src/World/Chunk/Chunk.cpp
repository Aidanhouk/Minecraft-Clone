#include "Chunk.h"

#include "../../Camera.h"
#include "../../Maths/NoiseGenerator.h"
#include "../../Renderer/RenderMaster.h"
#include "../../Util/Random.h"
#include "../Generation/Terrain/TerrainGenerator.h"
#include "../World.h"

#include <iostream>

Chunk::Chunk(const sf::Vector2i &location, World* world)
    : m_location(location)
    , m_pWorld(world)
{
    m_highestBlocks.setAll(0);
}

Chunk::Chunk(Chunk && x)
	:m_isLoaded{ x.m_isLoaded },
	m_pWorld{ x.m_pWorld },
	m_location{ x.m_location },
	m_highestBlocks{ std::move(x.m_highestBlocks) },
	m_chunksSection{ std::move(x.m_chunksSection) }
{
}

Chunk & Chunk::operator=(Chunk && x)
{
	if (&x == this)
		return *this;

	m_isLoaded = x.m_isLoaded;

	m_pWorld = x.m_pWorld;

	m_location = x.m_location;

	m_highestBlocks = std::move(x.m_highestBlocks);

	m_chunksSection = std::move(x.m_chunksSection);

	return *this;
}

bool Chunk::makeMesh(const Camera &camera)
{
    for (auto &chunkSection : m_chunksSection) {
        if (!chunkSection.hasMesh() &&
			camera.getFrustum().isBoxInFrustum(chunkSection.m_aabb)) {

			chunkSection.makeMesh();
            return true;
        }
    }
    return false;
}

void Chunk::setBlock(int x, int y, int z, ChunkBlock block)
{
	addSectionsBlockTarget(y);

	if (outOfBound(x, y, z)) {
		auto location = toWorldPosition(x, z);
		if (location.x < 0 || location.y < 0)
			return;

		m_pWorld->addUnloadedBlock(location.x, y, location.y, block);
		return;
	}
	
	m_chunksSection[y / CHUNK_SIZE].setBlock(x, y % CHUNK_SIZE, z, block);

	// if the highest block was destroyed
	if (y == m_highestBlocks.get(x, z)) {
	    auto highBlock = getBlock(x, y--, z);
	    while (!highBlock.getData().isOpaque && y >= 0) {
	        highBlock = getBlock(x, y--, z);
	    }
	}
	// if placed block is the highest now
	else if (y > m_highestBlocks.get(x, z)) {
	    m_highestBlocks.get(x, z) = y;
	}

	//if (hasLoaded()) {
	//	auto location = toWorldPosition(x, z);
	//	m_pWorld->updateChunk(location.x, y, location.y);
	//}
}

// Chunk block to SECTION BLOCK positions
ChunkBlock Chunk::getBlock(int x, int y, int z) const noexcept
{
    if (outOfBound(x, y, z)) {
        return BlockId::Air;
    }

    int bY = y % CHUNK_SIZE;

    return m_chunksSection[y / CHUNK_SIZE].getBlock(x, bY, z);
}

int Chunk::getHeightAt(int x, int z)
{
    return m_highestBlocks.get(x, z);
}

bool Chunk::outOfBound(int x, int y, int z) const noexcept
{
    if (x >= CHUNK_SIZE)
        return true;
    if (z >= CHUNK_SIZE)
        return true;

    if (x < 0)
        return true;
    if (y < 0)
        return true;
    if (z < 0)
        return true;

    if (y >= (int)m_chunksSection.size() * CHUNK_SIZE) {
        return true;
    }

    return false;
}

sf::Vector2i Chunk::toWorldPosition(int x, int z) const
{
	return {
		m_location.x * CHUNK_SIZE + x,
		m_location.y * CHUNK_SIZE + z };
}

void Chunk::drawChunks(RenderMaster &renderer, const Camera &camera)
{
	for (auto &chunkSection : m_chunksSection) {
		if (chunkSection.hasMesh()) {
			if (!chunkSection.hasBuffered()) {
				chunkSection.bufferMesh();
			}

			if (camera.getFrustum().isBoxInFrustum(chunkSection.m_aabb))
				renderer.addChunkToRender(chunkSection);
		}
	}
}

bool Chunk::hasLoaded() const noexcept
{
	return m_isLoaded;
}

void Chunk::load(TerrainGenerator &generator)
{
	generator.generateTerrainFor(*this);
	m_isLoaded = true;
}

ChunkSection &Chunk::getSection(int index)
{
    static ChunkSection errorSection({444, 444, 444}, m_pWorld);

    if (index >= (int)m_chunksSection.size() || index < 0)
        return errorSection;

    return m_chunksSection[index];
}

void Chunk::deleteMeshes()
{
    for (unsigned i = 0; i < m_chunksSection.size(); i++) {
		m_chunksSection[i].deleteMeshes();
    }
}

void Chunk::addSectionsBlockTarget(int blockY)
{
    int index = blockY / CHUNK_SIZE;
    addSectionsIndexTarget(index);
}

void Chunk::addSectionsIndexTarget(int index)
{
    while ((int)m_chunksSection.size() < index + 1) {
        addSection();
    }
}

void Chunk::addSection()
{
	int y = m_chunksSection.size();
	m_chunksSection.emplace_back(sf::Vector3i(m_location.x, y, m_location.y), m_pWorld);
}
