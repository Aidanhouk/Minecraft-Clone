#include "LiquidManager.h"

#include "World/Chunk/ChunkManager.h"

LiquidManager::LiquidManager(ChunkManager* chunkManager)
    : TickObject(5u)
{
    m_chunks = &chunkManager->getChunks();
}

void LiquidManager::tickUpdate(unsigned int tickTime)
{
    if (++currentTick < tickRate)
        return;

    currentTick = 0;

    for (auto& chunkPair : *m_chunks) {
        auto& chunk = chunkPair.second;
        chunk.addBusyLevel();

        auto& liquidBlocks = chunk.getLiqiudBlocks();
        for (auto& block : liquidBlocks) {
            // queue add...
        }

        chunk.subtractBusyLevel();
    }
}
