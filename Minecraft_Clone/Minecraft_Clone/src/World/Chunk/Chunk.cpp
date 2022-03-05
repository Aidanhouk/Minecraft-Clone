#include "Chunk.h"

#include "Camera.h"
#include "Maths/NoiseGenerator.h"
#include "Renderer/RenderMaster.h"
#include "Util/Random.h"
#include "../Generation/Terrain/TerrainGenerator.h"
#include "../World/World.h"

#include <iostream>

Chunk::Chunk(const sf::Vector2i &location, World* world)
    : m_location(location)
    , m_pWorld(world)
{
    m_highestBlocks.setAll(0);
}

Chunk::Chunk(Chunk && x)
	:m_isLoaded{ x.m_isLoaded }
	, m_updateMesh{ x.m_updateMesh }
	, m_sunLightLoaded{ x.m_sunLightLoaded }
	, m_ambientOcclusionLoaded{ x.m_ambientOcclusionLoaded }
	, m_pWorld{ x.m_pWorld }
	, m_location{ x.m_location }
	, m_highestBlocks{ std::move(x.m_highestBlocks) }
	, m_chunkSections{ std::move(x.m_chunkSections) }
	, m_unloadedBlocks{ std::move(x.m_unloadedBlocks) }
{
}

Chunk & Chunk::operator=(Chunk && x)
{
	if (&x == this)
		return *this;

	m_isLoaded = x.m_isLoaded;
	m_updateMesh = x.m_updateMesh;
	m_sunLightLoaded = x.m_sunLightLoaded;
	m_ambientOcclusionLoaded = x.m_ambientOcclusionLoaded;
	m_pWorld = x.m_pWorld;
	m_location = x.m_location;
	m_highestBlocks = std::move(x.m_highestBlocks);
	m_chunkSections = std::move(x.m_chunkSections);
	m_unloadedBlocks = std::move(x.m_unloadedBlocks);

	return *this;
}

void Chunk::makeMesh(const Camera &camera)
{
    for (auto &chunkSection : m_chunkSections) {
		if ((!chunkSection.hasMesh() || m_updateMesh)
			)//&& camera.getFrustum().isBoxInFrustum(chunkSection.m_aabb))
		{
			chunkSection.makeMesh();
			m_updateMesh = false;
		}
    }
}

void Chunk::setBlock(int x, int y, int z, ChunkBlock block)
{
	addSectionsBlockTarget(y);

	if (outOfBounds(x, y, z)) {
		auto location = toWorldPosition(x, z);
		if (location.x < 0 || location.y < 0)
			return;

		m_pWorld->addUnloadedBlock(location.x, y, location.y, block);
		return;
	}
	
	m_chunkSections[y / CHUNK_SIZE].setBlock(x, y % CHUNK_SIZE, z, block);

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
}

void Chunk::setBlockInChunk(int x, int y, int z, ChunkBlock block)
{
	addSectionsBlockTarget(y);

	m_chunkSections[y / CHUNK_SIZE].setBlockInSection(x, y % CHUNK_SIZE, z, block);

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
}

ChunkBlock Chunk::getBlock(int x, int y, int z) const noexcept
{
	if (outOfHeightBounds(y))
		return BlockId::Air;
    else if (outOfBounds(x, z)) {
		auto location = toWorldPosition(x, z);
		if (location.x < 0 || location.y < 0)
			return BlockId::Air;
		else
			return m_pWorld->getBlock(location.x, y, location.y);
    }

    return m_chunkSections[y / CHUNK_SIZE].getBlock(x, y % CHUNK_SIZE, z);
}

ChunkBlock Chunk::getBlockInChunk(int x, int y, int z) const
{
	if (outOfHeightBounds(y))
		return BlockId::Air;

	return m_chunkSections[y / CHUNK_SIZE].getBlock(x, y % CHUNK_SIZE, z);
}

ChunkBlock & Chunk::getBlockInChunkRef(int x, int y, int z)
{
	//@TODO fix this one day
	static ChunkBlock dummy(0);
	if (outOfHeightBounds(y))
		return dummy;

	return m_chunkSections[y / CHUNK_SIZE].getBlockInSectionRef(x, y % CHUNK_SIZE, z);
}

int Chunk::getHeightAt(int x, int z)
{
    return m_highestBlocks.get(x, z);
}

void Chunk::addUnloadedBlock(UnloadedBlock && block)
{
	m_unloadedBlocks.emplace_back(block);
}

// Note: there's no mutex lock before this function is being called
void Chunk::setUnloadedBlocks(Chunk &chunk)
{
	if (chunk.m_unloadedBlocks.empty())
		return;

	for (auto iter = chunk.m_unloadedBlocks.begin(); iter != chunk.m_unloadedBlocks.end(); ++iter) {
		setBlockInChunk(iter->pos.x, iter->pos.y, iter->pos.z, iter->block);
		m_pWorld->updateAmbientOcclusion(iter->pos.x, iter->pos.y, iter->pos.z, this);

		m_pWorld->updateSunLight(iter->pos.x, iter->pos.y, iter->pos.z);
		m_pWorld->updateChunk(iter->pos.x, iter->pos.y, iter->pos.z);
	}
	//chunk.shouldUpdateMesh();

	chunk.m_unloadedBlocks.clear();
	chunk.m_unloadedBlocks.shrink_to_fit();
}

bool Chunk::outOfBounds(int x, int y, int z) const noexcept
{
	if (x >= CHUNK_SIZE)
		return true;
	if (z >= CHUNK_SIZE)
		return true;

	if (x < 0)
		return true;
	if (z < 0)
		return true;
	if (y < 0)
		return true;

	if (y >= (int)m_chunkSections.size() * CHUNK_SIZE)
		return true;

	return false;
}

bool Chunk::outOfBounds(int x, int z) const noexcept
{
    if (x >= CHUNK_SIZE)
        return true;
    if (z >= CHUNK_SIZE)
        return true;

    if (x < 0)
        return true;
    if (z < 0)
        return true;

    return false;
}

bool Chunk::outOfHeightBounds(int y) const noexcept
{
	if (y < 0 || y >= (int)m_chunkSections.size() * CHUNK_SIZE)
		return true;
	
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
	for (auto &chunkSection : m_chunkSections) {
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

    if (index >= (int)m_chunkSections.size() || index < 0)
        return errorSection;

    return m_chunkSections[index];
}

void Chunk::deleteMeshes()
{
    for (unsigned i = 0; i < m_chunkSections.size(); i++) {
		m_chunkSections[i].deleteMeshes();
    }
}

void Chunk::setTorchLight(int x, int y, int z, int value)
{
	m_chunkSections[y / CHUNK_SIZE].setTorchLight(x, y % CHUNK_SIZE, z, value);
}

int Chunk::getTorchLight(int x, int y, int z)
{
	addSectionsBlockTarget(y);
	return m_chunkSections[y / CHUNK_SIZE].getTorchLight(x, y % CHUNK_SIZE, z);
}

void Chunk::setSunLight(int x, int y, int z, int value)
{
	addSectionsBlockTarget(y);
	m_chunkSections[y / CHUNK_SIZE].setSunLight(x, y % CHUNK_SIZE, z, value);
}

int Chunk::getSunLight(int x, int y, int z)
{
	addSectionsBlockTarget(y);
	return m_chunkSections[y / CHUNK_SIZE].getSunLight(x, y % CHUNK_SIZE, z);
}

void Chunk::addSectionsBlockTarget(int blockY)
{
    int index = blockY / CHUNK_SIZE;
    addSectionsIndexTarget(index);
}

void Chunk::addSectionsIndexTarget(int index)
{
    while ((int)m_chunkSections.size() < index + 1) {
        addSection();
    }
}

void Chunk::addSection()
{
	int y = m_chunkSections.size();
	m_chunkSections.emplace_back(sf::Vector3i(m_location.x, y, m_location.y), m_pWorld);
}
