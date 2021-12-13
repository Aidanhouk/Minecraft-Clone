#pragma once

#include "../../../Util/Random.h"
#include "Util/Array2D.h"
#include "World/World/WorldConstants.h"

class Chunk;

namespace Caves {
	void createCaves(Chunk &chunk, Random<std::minstd_rand> &rand, Array2D<int, CHUNK_SIZE> &heightMap);
}