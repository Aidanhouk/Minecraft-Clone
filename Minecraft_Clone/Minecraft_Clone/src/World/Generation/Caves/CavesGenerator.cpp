#include "CavesGenerator.h"

#include "../../Chunk/Chunk.h"
#include "WormCave.h"
#include "World/Block/BlockDatabase.h"
#include "../Seed.h"

static WormCave wormCave;

void Caves::createCaves(Chunk &chunk, Random<std::minstd_rand> &rand, Array2D<int, CHUNK_SIZE> &heightMap)
{
	static int caveSeed = g_Seed;

	for (int i = 1; i <= 2; ++i) {
		if (rand.intInRange(1, 10) > i)
			return;

		int startChunkX = (CHUNK_SIZE - 1) >> 1;
		int startChunkZ = (CHUNK_SIZE - 1) >> 1;
		//int startY = rand.intInRange(10, heightMap.get(startX, startZ) - 5);
		int startY = rand.intInRange(10, heightMap.get(startChunkX, startChunkZ) + 5);
		int startX = chunk.getLocation().x * CHUNK_SIZE + startChunkX;
		int startZ = chunk.getLocation().y * CHUNK_SIZE + startChunkZ;

		wormCave.SetCaveStartPos(sf::Vector3f(startX, startY, startZ));
		wormCave.SetSeed(caveSeed++);
		wormCave.SetSegmentCount(500);
		wormCave.SetThickness(rand.intInRange(3, 4));
		wormCave.createCave(chunk);

		//chunk.setBlock(startChunkX, heightMap.get(startChunkX, startChunkZ) + 5 + i, startChunkZ, BlockId::Bedrock);
	}
}
