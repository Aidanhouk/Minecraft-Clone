#pragma once

#include "../Tick/TickObject.h"
#include "Maths/Vector2XZ.h"
#include "World/Chunk/Chunk.h"

class ChunkManager;

using ChunkMap = std::unordered_map<VectorXZ, Chunk>;

class LiquidManager : public TickObject
{

public:
    LiquidManager(ChunkManager* chunkManager);

private:
    void tickUpdate(unsigned int tickTime) override;

private:
    ChunkMap* m_chunks;

};