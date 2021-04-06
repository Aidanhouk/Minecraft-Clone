#include "Chunk.h"

#include "../../Camera.h"
#include "../../Maths/NoiseGenerator.h"
#include "../../Renderer/RenderMaster.h"
#include "../../Util/Random.h"
#include "../Generation/Terrain/TerrainGenerator.h"
#include "../World.h"

Chunk::Chunk(World &world, const sf::Vector2i &location)
    : m_location(location)
    , m_pWorld(&world)
{
    m_highestBlocks.setAll(0);
}

bool Chunk::makeMesh(const Camera &camera)
{
    for (auto &chunk : m_chunksSection) {
        if (!chunk.hasMesh() &&
            camera.getFrustum().isBoxInFrustum(chunk.m_aabb)) {
            chunk.makeMesh();
            return true;
        }
    }
    return false;
}

void Chunk::setBlock(int x, int y, int z, ChunkBlock block)
{
    addSectionsBlockTarget(y);
    if (outOfBound(x, y, z))
        return;

    int bY = y % CHUNK_SIZE;
	m_chunksSection[y / CHUNK_SIZE].setBlock(x, bY, z, block);

	// if the highest block was destroyed
    if (y == m_highestBlocks.get(x, z)) {
		// it does nothing
        auto highBlock = getBlock(x, y--, z);
        while (!highBlock.getData().isOpaque) {
            highBlock = getBlock(x, y--, z);
        }
    }
	// if placed block is the highest now
    else if (y > m_highestBlocks.get(x, z)) {
        m_highestBlocks.get(x, z) = y;
    }

    //if (m_isLoaded) {
    //    // m_pWorld->updateChunk(x, y, z);
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

void Chunk::drawChunks(RenderMaster &renderer, const Camera &camera)
{
    for (auto &chunk : m_chunksSection) {
        if (chunk.hasMesh()) {
            if (!chunk.hasBuffered()) {
                chunk.bufferMesh();
            }

            if (camera.getFrustum().isBoxInFrustum(chunk.m_aabb))
                renderer.addChunkToRender(chunk);
        }
    }
}

bool Chunk::hasLoaded() const noexcept
{
    return m_isLoaded;
}

void Chunk::load(TerrainGenerator &generator)
{
    if (hasLoaded())
        return;

    generator.generateTerrainFor(*this);
    m_isLoaded = true;
}

ChunkSection &Chunk::getSection(int index)
{
    static ChunkSection errorSection({444, 444, 444}, *m_pWorld);

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

void Chunk::addSection()
{
    int y = m_chunksSection.size();
	m_chunksSection.emplace_back(sf::Vector3i(m_location.x, y, m_location.y),
                          *m_pWorld);
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
