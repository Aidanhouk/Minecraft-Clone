#pragma once

#include <array>
#include <memory>

#include "../../Util/Singleton.h"
#include "../../Util/Random.h"

#include "BlockId.h"
#include "BlockTypes/BlockType.h"

#include "../../Texture/TextureAtlas.h"

using Rand = Random<std::minstd_rand>;

class BlockDatabase : public Singleton {
public:
	static BlockDatabase &get();
	static bool canPlaceOnBlock(BlockId blockId, BlockId placeOnThisBlockId);
	static BlockId getRandomFlower(Rand &rand);
	static BlockId getRandomDoubleFlower(Rand &rand);
	static BlockId getDoublePlantMain(BlockId blockId);
	static BlockId getDoublePlantSecondPart(BlockId blockId);

	bool isDoublePlant(BlockId blockId);

	bool isWoodPlanks(BlockId blockId);

	bool isSword(BlockId blockId);
	bool isPickaxe(BlockId blockId);
	bool isShovel(BlockId blockId);
	bool isAxe(BlockId blockId);

	bool canPickaxeMine(BlockId toolId, BlockId blockId);
	
	const BlockType &getBlock(BlockId id) const;
	const BlockData &getData(BlockId id) const;
	
	TextureAtlas textureAtlas;
	
private:
	BlockDatabase();
	
	std::array<std::unique_ptr<BlockType>, (unsigned)BlockId::NUM_TYPES>
	    m_blocks;
};