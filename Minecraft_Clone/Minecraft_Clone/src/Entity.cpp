#include "Entity.h"

#include "World/World/World.h"

#include <iostream>

BlockId Entity::getBlockEntityStandsOn(World & world)
{
	std::array<BlockId, 5> blocksIDs;
	getBlocksUnderEntity(world, blocksIDs);

	for (auto & blockId : blocksIDs) {
		if (ChunkBlock(blockId).getData().isCollidable)
			return ChunkBlock(blockId).getData().id;
	}

	return BlockId::NUM_TYPES;
}

void Entity::getBlocksUnderEntity(World & world, std::array<BlockId, 5>& blocksIDs, float specificHeight)
{
	blocksIDs[0] = world.getBlock(position.x, position.y - specificHeight, position.z).getData().id;
	blocksIDs[1] = world.getBlock(position.x - box.dimensions.x, position.y - specificHeight, position.z - box.dimensions.z).getData().id;
	blocksIDs[2] = world.getBlock(position.x - box.dimensions.x, position.y - specificHeight, position.z + box.dimensions.z).getData().id;
	blocksIDs[3] = world.getBlock(position.x + box.dimensions.x, position.y - specificHeight, position.z - box.dimensions.z).getData().id;
	blocksIDs[4] = world.getBlock(position.x + box.dimensions.x, position.y - specificHeight, position.z + box.dimensions.z).getData().id;
}

bool Entity::isEntityOnWaterSurface(World & world)
{
	ChunkBlock block;
	bool waterBeneath = false;

	block = world.getBlock(position.x, (std::ceil)(position.y - 2.2f), position.z).getData().id;
	if (block.getData().isCollidable)
		return false;
	else {
		if (block.getData().shaderType == BlockShaderType::Liquid)
			waterBeneath = true;

		block = world.getBlock(position.x - box.dimensions.x, (std::ceil)(position.y - 2.2f), position.z - box.dimensions.z).getData().id;
		if (block.getData().isCollidable)
			return false;
		else {
			if (block.getData().shaderType == BlockShaderType::Liquid)
				waterBeneath = true;

			block = world.getBlock(position.x - box.dimensions.x, (std::ceil)(position.y - 2.2f), position.z + box.dimensions.z).getData().id;
			if (block.getData().isCollidable)
				return false;
			else {
				if (block.getData().shaderType == BlockShaderType::Liquid)
					waterBeneath = true;

				block = world.getBlock(position.x + box.dimensions.x, (std::ceil)(position.y - 2.2f), position.z - box.dimensions.z).getData().id;
				if (block.getData().isCollidable) 
					return false;
				else {
					if (block.getData().shaderType == BlockShaderType::Liquid)
						waterBeneath = true;

					block = world.getBlock(position.x + box.dimensions.x, (std::ceil)(position.y - 2.2f), position.z + box.dimensions.z).getData().id;
					if (block.getData().isCollidable)
						return false;
					else if (block.getData().shaderType == BlockShaderType::Liquid)
						waterBeneath = true;
				}
			}
		}
	}

	if (waterBeneath)
		return true;
	else // e.g entity is flying
		return false;
}
