#pragma once

#include "../../../Util/Random.h"
#include "World/Block/BlockId.h"
#include "Util/Array2D.h"
#include "World/World/WorldConstants.h"

class Chunk;

namespace ClusterGenerator {
	// flowerFreq is probability in percent of putting a flower in each block (0-100)
	void makeFlowerCluster(Chunk &chunk, Random<std::minstd_rand> &rand, int x, int z,
		Array2D<int, CHUNK_SIZE> &heightMap, BlockId blockId, int radius, int flowerFreq);

	void makeDoubleFlowerCluster(Chunk &chunk, Random<std::minstd_rand> &rand, int x, int z,
		Array2D<int, CHUNK_SIZE> &heightMap, BlockId blockId, int radius, int flowerFreq);

	void makeFlowerWitchCircle(Chunk &chunk, Random<std::minstd_rand> &rand, int x, int z,
		Array2D<int, CHUNK_SIZE> &heightMap, BlockId blockId);

	void makeUnderwaterCluster(Chunk &chunk, Random<std::minstd_rand> &rand, int x, int z,
		Array2D<int, CHUNK_SIZE> &heightMap, BlockId blockId);

	void makeOreClusters(Chunk &chunk, Random<std::minstd_rand> &rand, Array2D<int, CHUNK_SIZE> &heightMap);
}