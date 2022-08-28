#include "World.h"

#include <iostream>

const int SUN_LIGHT_VALUE = 15;

void World::setTorchLight(int x, int y, int z, int lightValue)
{
	auto chunkPosition = getChunkXZ(x, z);
	auto &chunk = m_chunkManager.getChunk(chunkPosition.x, chunkPosition.z);
	auto blockPositionInChunk = getBlockXZ(x, z);

	chunk.setTorchLight(blockPositionInChunk.x, y, blockPositionInChunk.z, lightValue);

	m_lightQueue.emplace(blockPositionInChunk.x, y, blockPositionInChunk.z, &chunk);

	propagateLight();
}

void World::removeTorchLight(int x, int y, int z)
{
	auto chunkPosition = getChunkXZ(x, z);
	auto &chunk = m_chunkManager.getChunk(chunkPosition.x, chunkPosition.z);
	auto blockPositionInChunk = getBlockXZ(x, z);

	auto light = (short)chunk.getTorchLight(blockPositionInChunk.x, y, blockPositionInChunk.z);

	m_lightRemovalQueue.emplace(blockPositionInChunk.x, y, blockPositionInChunk.z, light, &chunk);

	chunk.setTorchLight(blockPositionInChunk.x, y, blockPositionInChunk.z, 0);

	unPropagateLight();

	propagateLight();
}

int World::getTorchLight(int x, int y, int z)
{
	auto chunkPosition = getChunkXZ(x, z);
	auto &ch = m_chunkManager.getChunk(chunkPosition.x, chunkPosition.z);
	auto blockPositionInChunk = getBlockXZ(x, z);

	return ch.getTorchLight(blockPositionInChunk.x, y, blockPositionInChunk.z);
}

void World::propagateLight()
{
	while (!m_lightQueue.empty()) {
		LightNode &node = m_lightQueue.front();
		short x = node.x;
		short y = node.y;
		short z = node.z;
		Chunk* chunk = node.chunk;

		m_lightQueue.pop();

		if (chunk->getBlock(x, y, z).isOpaque())
			continue;

		int lightLevel = chunk->getTorchLight(x, y, z);

		if (x == 0) {
			auto &chunkLocation = chunk->getLocation();
			Chunk* adjChunk = &m_chunkManager.getChunk(chunkLocation.x - 1, chunkLocation.y);
			tryAddLightNode(CHUNK_SIZE - 1, y, z, lightLevel, adjChunk);
		}
		else
			tryAddLightNode(x - 1, y, z, lightLevel, chunk);

		if (x == CHUNK_SIZE - 1) {
			auto& chunkLocation = chunk->getLocation();
			Chunk* adjChunk = &m_chunkManager.getChunk(chunkLocation.x + 1, chunkLocation.y);
			tryAddLightNode(0, y, z, lightLevel, adjChunk);
		}
		else
			tryAddLightNode(x + 1, y, z, lightLevel, chunk);

		if (z == 0) {
			auto& chunkLocation = chunk->getLocation();
			Chunk* adjChunk = &m_chunkManager.getChunk(chunkLocation.x, chunkLocation.y - 1);
			tryAddLightNode(x, y, CHUNK_SIZE - 1, lightLevel, adjChunk);
		}
		else
			tryAddLightNode(x, y, z - 1, lightLevel, chunk);

		if (z == CHUNK_SIZE - 1) {
			auto& chunkLocation = chunk->getLocation();
			Chunk* adjChunk = &m_chunkManager.getChunk(chunkLocation.x, chunkLocation.y + 1);
			tryAddLightNode(x, y, 0, lightLevel, adjChunk);
		}
		else
			tryAddLightNode(x, y, z + 1, lightLevel, chunk);

		tryAddLightNode(x, y - 1, z, lightLevel, chunk);
		tryAddLightNode(x, y + 1, z, lightLevel, chunk);
	}
}

void World::tryAddLightNode(int x, int y, int z, int lightLevel, Chunk * chunk)
{
	if (chunk->getTorchLight(x, y, z) + 2 <= lightLevel) {

		chunk->setTorchLight(x, y, z, lightLevel - 1);
		m_lightQueue.emplace(x, y, z, chunk);
	}
}

void World::unPropagateLight()
{
	while (!m_lightRemovalQueue.empty()) {
		LightRemovalNode &node = m_lightRemovalQueue.front();
		short x = node.x;
		short y = node.y;
		short z = node.z;
		short lightLevel = node.value;
		Chunk* chunk = node.chunk;

		m_lightRemovalQueue.pop();

		if (chunk->getBlock(x, y, z).isOpaque()
			&& chunk->getBlock(x, y, z).getData().id != BlockId::Glowstone) {
			continue;
		}

		if (x == 0) {
			auto &chunkLocation = chunk->getLocation();
			Chunk* adjChunk = &m_chunkManager.getChunk(chunkLocation.x - 1, chunkLocation.y);
			tryAddLightRemovalNode(CHUNK_SIZE - 1, y, z, lightLevel, adjChunk);
		}
		else
			tryAddLightRemovalNode(x - 1, y, z, lightLevel, chunk);

		if (x == CHUNK_SIZE - 1) {
			auto& chunkLocation = chunk->getLocation();
			Chunk* adjChunk = &m_chunkManager.getChunk(chunkLocation.x + 1, chunkLocation.y);
			tryAddLightRemovalNode(0, y, z, lightLevel, adjChunk);
		}
		else
			tryAddLightRemovalNode(x + 1, y, z, lightLevel, chunk);

		if (z == 0) {
			auto& chunkLocation = chunk->getLocation();
			Chunk* adjChunk = &m_chunkManager.getChunk(chunkLocation.x, chunkLocation.y - 1);
			tryAddLightRemovalNode(x, y, CHUNK_SIZE - 1, lightLevel, adjChunk);
		}
		else
			tryAddLightRemovalNode(x, y, z - 1, lightLevel, chunk);

		if (z == CHUNK_SIZE - 1) {
			auto& chunkLocation = chunk->getLocation();
			Chunk* adjChunk = &m_chunkManager.getChunk(chunkLocation.x, chunkLocation.y + 1);
			tryAddLightRemovalNode(x, y, 0, lightLevel, adjChunk);
		}
		else
			tryAddLightRemovalNode(x, y, z + 1, lightLevel, chunk);

		tryAddLightRemovalNode(x, y - 1, z, lightLevel, chunk);
		tryAddLightRemovalNode(x, y + 1, z, lightLevel, chunk);
	}
}

void World::tryAddLightRemovalNode(int x, int y, int z, int lightLevel, Chunk * chunk)
{
	int neighborLightLevel = chunk->getTorchLight(x, y, z);
	if (neighborLightLevel != 0 && neighborLightLevel < lightLevel) {
		chunk->setTorchLight(x, y, z, 0);
		m_lightRemovalQueue.emplace(x, y, z, neighborLightLevel, chunk);
	}
	else if (neighborLightLevel >= lightLevel) {
		m_lightQueue.emplace(x, y, z, chunk);
	}
}



void World::calculateSunLight(Chunk * chunk)
{
	if (!chunk->hasLoaded())
		return;
	std::unique_lock<std::mutex> lock(m_genMutex);

	for (int x = 0; x < CHUNK_SIZE; ++x) {
		for (int z = 0; z < CHUNK_SIZE; ++z) {
			int y = chunk->getHeightAt(x, z) + 1;

			while (!chunk->getBlock(x, y, z).isOpaque() && y > 0) {
				chunk->setSunLight(x, y, z, SUN_LIGHT_VALUE);
				m_sunLightQueue.emplace(x, y, z, chunk);
				--y;
			}
			chunk->setSunLight(x, y, z, SUN_LIGHT_VALUE);
		}
	}

	propagateSunLight();

	chunk->setSunLightLoaded(true);
}

void World::updateSunLight(int x, int y, int z)
{
	auto chunkPosition = getChunkXZ(x, z);
	std::unique_lock<std::mutex> lock(m_genMutex);

	auto &chunk = m_chunkManager.getChunk(chunkPosition.x, chunkPosition.z);
	chunk.addBusyLevel();
	if (!chunk.hasLoaded()) {
		chunk.subtractBusyLevel();
		return;
	}
	auto blockPositionInChunk = getBlockXZ(x, z);
	x = blockPositionInChunk.x, z = blockPositionInChunk.z;

	auto lightValue = chunk.getSunLight(x, y, z);
	
	if (lightValue == SUN_LIGHT_VALUE) {
		while (!chunk.getBlock(x, y, z).isOpaque() && y > 0) {
			chunk.setSunLight(x, y, z, SUN_LIGHT_VALUE);
			m_sunLightQueue.emplace(x, y, z, &chunk);
			--y;
		}
		chunk.setSunLight(x, y, z, SUN_LIGHT_VALUE);
	}
	else {
		chunk.setSunLight(x, y, z, lightValue);
		m_sunLightQueue.emplace(x, y, z, &chunk);
	}

	propagateSunLight(true);

	chunk.setSunLightLoaded(true);
	chunk.subtractBusyLevel();
}

void World::removeSunLight(int x, int y, int z)
{
	auto chunkPosition = getChunkXZ(x, z);
	std::unique_lock<std::mutex> lock(m_genMutex);

	auto &chunk = m_chunkManager.getChunk(chunkPosition.x, chunkPosition.z);
	if (!chunk.hasLoaded())
		return;
	auto blockPositionInChunk = getBlockXZ(x, z);
	
	auto light = (short)chunk.getSunLight(blockPositionInChunk.x, y, blockPositionInChunk.z);
	
	m_sunLightRemovalQueue.emplace(blockPositionInChunk.x, y, blockPositionInChunk.z, light, &chunk);
	chunk.setSunLight(blockPositionInChunk.x, y, blockPositionInChunk.z, 0);
	--y;

	while (!chunk.getBlock(blockPositionInChunk.x, y, blockPositionInChunk.z).isOpaque() && y > 0) {
		chunk.setSunLight(blockPositionInChunk.x, y, blockPositionInChunk.z, 0);
		m_sunLightRemovalQueue.emplace(blockPositionInChunk.x, y, blockPositionInChunk.z, 0, &chunk);
		--y;
	}
	
	unPropagateSunLight();
	
	propagateSunLight();
}

void World::propagateSunLight(bool _updateChunk/* = false*/)
{
	while (!m_sunLightQueue.empty()) {
		LightNode &node = m_sunLightQueue.front();
		short x = node.x;
		short y = node.y;
		short z = node.z;
		Chunk* chunk = node.chunk;
        if (_updateChunk)
            updateChunk(chunk->getLocation().x * CHUNK_SIZE + x, y, chunk->getLocation().y * CHUNK_SIZE + z);

		m_sunLightQueue.pop();

		if (chunk->getBlock(x, y, z).isOpaque())
			continue;

		int lightLevel = chunk->getSunLight(x, y, z);

		if (x == 0) {
			auto &chunkLocation = chunk->getLocation();
			Chunk* adjChunk = &m_chunkManager.getChunk(chunkLocation.x - 1, chunkLocation.y);
			tryAddSunLightNode(CHUNK_SIZE - 1, y, z, lightLevel, adjChunk);
		}
		else
			tryAddSunLightNode(x - 1, y, z, lightLevel, chunk);

		if (x == CHUNK_SIZE - 1) {
			auto& chunkLocation = chunk->getLocation();
			Chunk* adjChunk = &m_chunkManager.getChunk(chunkLocation.x + 1, chunkLocation.y);
			tryAddSunLightNode(0, y, z, lightLevel, adjChunk);
		}
		else
			tryAddSunLightNode(x + 1, y, z, lightLevel, chunk);

		if (z == 0) {
			auto& chunkLocation = chunk->getLocation();
			Chunk* adjChunk = &m_chunkManager.getChunk(chunkLocation.x, chunkLocation.y - 1);
			tryAddSunLightNode(x, y, CHUNK_SIZE - 1, lightLevel, adjChunk);
		}
		else
			tryAddSunLightNode(x, y, z - 1, lightLevel, chunk);

		if (z == CHUNK_SIZE - 1) {
			auto& chunkLocation = chunk->getLocation();
			Chunk* adjChunk = &m_chunkManager.getChunk(chunkLocation.x, chunkLocation.y + 1);
			tryAddSunLightNode(x, y, 0, lightLevel, adjChunk);
		}
		else
			tryAddSunLightNode(x, y, z + 1, lightLevel, chunk);

		tryAddSunLightNode(x, y - 1, z, lightLevel, chunk);
		tryAddSunLightNode(x, y + 1, z, lightLevel, chunk);
	}
}

void World::tryAddSunLightNode(int x, int y, int z, int lightLevel, Chunk * chunk)
{
	if (chunk->getSunLight(x, y, z) + 2 <= lightLevel) {

		chunk->setSunLight(x, y, z, lightLevel - 1);
		m_sunLightQueue.emplace(x, y, z, chunk);
	}
}

void World::unPropagateSunLight()
{
	while (!m_sunLightRemovalQueue.empty()) {
		LightRemovalNode &node = m_sunLightRemovalQueue.front();
		short x = node.x;
		short y = node.y;
		short z = node.z;
		short lightLevel = node.value;
		Chunk* chunk = node.chunk;

		m_sunLightRemovalQueue.pop();
	
		if (chunk->getBlock(x, y, z).isOpaque())
			continue;
	
		if (x == 0) {
			auto &chunkLocation = chunk->getLocation();
			Chunk* adjChunk = &m_chunkManager.getChunk(chunkLocation.x - 1, chunkLocation.y);
			tryAddSunLightRemovalNode(CHUNK_SIZE - 1, y, z, lightLevel, adjChunk);
		}
		else
			tryAddSunLightRemovalNode(x - 1, y, z, lightLevel, chunk);
	
		if (x == CHUNK_SIZE - 1) {
			auto& chunkLocation = chunk->getLocation();
			Chunk* adjChunk = &m_chunkManager.getChunk(chunkLocation.x + 1, chunkLocation.y);
			tryAddSunLightRemovalNode(0, y, z, lightLevel, adjChunk);
		}
		else
			tryAddSunLightRemovalNode(x + 1, y, z, lightLevel, chunk);
	
		if (z == 0) {
			auto& chunkLocation = chunk->getLocation();
			Chunk* adjChunk = &m_chunkManager.getChunk(chunkLocation.x, chunkLocation.y - 1);
			tryAddSunLightRemovalNode(x, y, CHUNK_SIZE - 1, lightLevel, adjChunk);
		}
		else
			tryAddSunLightRemovalNode(x, y, z - 1, lightLevel, chunk);
	
		if (z == CHUNK_SIZE - 1) {
			auto& chunkLocation = chunk->getLocation();
			Chunk* adjChunk = &m_chunkManager.getChunk(chunkLocation.x, chunkLocation.y + 1);
			tryAddSunLightRemovalNode(x, y, 0, lightLevel, adjChunk);
		}
		else
			tryAddSunLightRemovalNode(x, y, z + 1, lightLevel, chunk);
	
		tryAddSunLightRemovalNode(x, y - 1, z, lightLevel, chunk);
		tryAddSunLightRemovalNode(x, y + 1, z, lightLevel, chunk);
	}
}

void World::tryAddSunLightRemovalNode(int x, int y, int z, int lightLevel, Chunk * chunk)
{
	int neighborLightLevel = chunk->getSunLight(x, y, z);
	if (neighborLightLevel != 0 && neighborLightLevel < lightLevel) {
		chunk->setSunLight(x, y, z, 0);
		m_sunLightRemovalQueue.emplace(x, y, z, neighborLightLevel, chunk);
	}
	else if (neighborLightLevel >= lightLevel) {
		m_sunLightQueue.emplace(x, y, z, chunk);
	}
}

void World::updateLitChunks(int x, int y, int z)
{
	// @TODO optimize this
	// update all possible lit chunks
	// apparently not effective

	updateChunk(x - CHUNK_SIZE, y + CHUNK_SIZE, z - CHUNK_SIZE);
	updateChunk(x - CHUNK_SIZE, y + CHUNK_SIZE, z);
	updateChunk(x - CHUNK_SIZE, y + CHUNK_SIZE, z + CHUNK_SIZE);
	updateChunk(x - CHUNK_SIZE, y, z - CHUNK_SIZE);
	updateChunk(x - CHUNK_SIZE, y, z);
	updateChunk(x - CHUNK_SIZE, y, z + CHUNK_SIZE);
	updateChunk(x - CHUNK_SIZE, y - CHUNK_SIZE, z - CHUNK_SIZE);
	updateChunk(x - CHUNK_SIZE, y - CHUNK_SIZE, z);
	updateChunk(x - CHUNK_SIZE, y - CHUNK_SIZE, z + CHUNK_SIZE);

	updateChunk(x, y + CHUNK_SIZE, z - CHUNK_SIZE);
	updateChunk(x, y + CHUNK_SIZE, z);
	updateChunk(x, y + CHUNK_SIZE, z + CHUNK_SIZE);
	updateChunk(x, y, z - CHUNK_SIZE);
	updateChunk(x, y, z);
	updateChunk(x, y, z + CHUNK_SIZE);
	updateChunk(x, y - CHUNK_SIZE, z - CHUNK_SIZE);
	updateChunk(x, y - CHUNK_SIZE, z);
	updateChunk(x, y - CHUNK_SIZE, z + CHUNK_SIZE);

	updateChunk(x + CHUNK_SIZE, y + CHUNK_SIZE, z - CHUNK_SIZE);
	updateChunk(x + CHUNK_SIZE, y + CHUNK_SIZE, z);
	updateChunk(x + CHUNK_SIZE, y + CHUNK_SIZE, z + CHUNK_SIZE);
	updateChunk(x + CHUNK_SIZE, y, z - CHUNK_SIZE);
	updateChunk(x + CHUNK_SIZE, y, z);
	updateChunk(x + CHUNK_SIZE, y, z + CHUNK_SIZE);
	updateChunk(x + CHUNK_SIZE, y - CHUNK_SIZE, z - CHUNK_SIZE);
	updateChunk(x + CHUNK_SIZE, y - CHUNK_SIZE, z);
	updateChunk(x + CHUNK_SIZE, y - CHUNK_SIZE, z + CHUNK_SIZE);
}