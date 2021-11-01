#pragma once

#include "BlockId.h"

struct BlockDataHolder;
class BlockType;

struct ChunkBlock {
	ChunkBlock() = default;

	ChunkBlock(Block_t id);
	ChunkBlock(BlockId id);

	const BlockDataHolder &getData() const;
	const BlockType &getType() const;

	const Block_t getId() const
	{
		return id;
	}

	bool operator==(ChunkBlock other) const;
	bool operator!=(ChunkBlock other) const;

	Block_t id = 0;
};