#include "Entity.h"

#include "World/World/World.h"

#include <iostream>

BlockId Entity::getBlockEntityStandsOn(World & world)
{
	std::array<BlockId, 5> blocksIDs;
	getBlocksUnderEntity(world, blocksIDs);

	for (auto & blockId : blocksIDs) {
		if (ChunkBlock(blockId).isCollidable())
			return ChunkBlock(blockId).getData().id;
	}

	return BlockId::NUM_TYPES;
}

void Entity::getBlocksUnderEntity(World & world, std::array<BlockId, 5>& blocksIDs, float specificHeight)
{
	blocksIDs[0] = world.getBlockId(position.x, position.y - specificHeight, position.z);
	blocksIDs[1] = world.getBlockId(position.x - box.dimensions.x, position.y - specificHeight, position.z - box.dimensions.z);
	blocksIDs[2] = world.getBlockId(position.x - box.dimensions.x, position.y - specificHeight, position.z + box.dimensions.z);
	blocksIDs[3] = world.getBlockId(position.x + box.dimensions.x, position.y - specificHeight, position.z - box.dimensions.z);
	blocksIDs[4] = world.getBlockId(position.x + box.dimensions.x, position.y - specificHeight, position.z + box.dimensions.z);
}

bool Entity::isEntityOnWaterSurface(World & world)
{
	const int y = (std::ceil)(position.y - 2.2f);
	const float x = position.x, z = position.z;
	const float boxX = box.dimensions.x, boxZ = box.dimensions.z;

	bool waterBeneath = false;

	ChunkBlock block = world.getBlockId(x, y, z);
	if (block.isCollidable())
		return false;
	else if (block.isShaderLiquid())
		waterBeneath = true;

	block = world.getBlockId(x - boxX, y, z - boxZ);
	if (block.isCollidable())
		return false;
	else if (block.isShaderLiquid())
		waterBeneath = true;

	block = world.getBlockId(x - boxX, y, z + boxZ);
	if (block.isCollidable())
		return false;
	else if (block.isShaderLiquid())
		waterBeneath = true;

	block = world.getBlockId(x + boxX, y, z - boxZ);
	if (block.isCollidable())
		return false;
	else if (block.isShaderLiquid())
		waterBeneath = true;

	block = world.getBlockId(x + boxX, y, z + boxZ);
	if (block.isCollidable())
		return false;
	else if (block.isShaderLiquid())
		waterBeneath = true;

	return waterBeneath;
	// if false e.g entity is flying
}
