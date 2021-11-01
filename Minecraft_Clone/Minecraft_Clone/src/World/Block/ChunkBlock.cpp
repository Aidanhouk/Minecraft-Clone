#include "ChunkBlock.h"

#include "BlockDatabase.h"

ChunkBlock::ChunkBlock(Block_t id)
    : id(id)
{
}

ChunkBlock::ChunkBlock(BlockId id)
    : id(static_cast<Block_t>(id))
{
}

const BlockDataHolder &ChunkBlock::getData() const
{
    return BlockDatabase::get().getData((BlockId)id).getBlockData();
}

const BlockType &ChunkBlock::getType() const
{
    return BlockDatabase::get().getBlock((BlockId)id);
}

bool ChunkBlock::operator==(ChunkBlock other) const
{
	if (id == (Block_t)BlockId::CaveAir)
		return (other.id == id || other.id == 0);

	return id == other.id;
}

bool ChunkBlock::operator!=(ChunkBlock other) const
{
	return !(*this == other);
}
