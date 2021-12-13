#pragma once

#include "../../../Util/Random.h"
#include "Util/Array2D.h"
#include "World/World/WorldConstants.h"
#include "World/Block/BlockId.h"

class Chunk;

void makeOakTree(Chunk &chunk, Random<std::minstd_rand> &rand, int x, int y, int z);
void makeBirchTree(Chunk &chunk, Random<std::minstd_rand> &rand, int x, int y, int z);
void makePalmTree(Chunk &chunk, Random<std::minstd_rand> &rand, int x, int y, int z);
void makeSpruceTree(Chunk &chunk, Random<std::minstd_rand> &rand, int x, int y, int z);
void makeCactus(Chunk &chunk, Random<std::minstd_rand> &rand, int x, int y, int z);

void makeSugarCane(Chunk &chunk, Random<std::minstd_rand> &rand, int x, int z,
	Array2D<int, CHUNK_SIZE> &heightMap);
void makeDoublePlant(Chunk &chunk, int x, int y, int z, BlockId id);