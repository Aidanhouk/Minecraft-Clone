#include "PlayerDigEvent.h"

#include "../../Item/Material.h"
#include "../../Player/Player.h"
#include "../World/World.h"
#include "World/Block/BlockDatabase.h"
#include "Player/PlayerHand/Hand.h"
#include "Audio/SoundMaster.h"
#include "Audio/SoundFunctions.h"

#include <iostream>

PlayerDigEvent::PlayerDigEvent(sf::Mouse::Button button, const glm::vec3 &location, Player &player, Hand &hand)
    : m_buttonPress(button)
    , m_digSpot(location)
    , m_pPlayer(&player)
	, m_pHand(&hand)
{
}

void PlayerDigEvent::handle(World &world)
{
    auto chunkLocation = World::getChunkXZ(static_cast<int>(m_digSpot.x), static_cast<int>(m_digSpot.z));

    if (world.getChunkManager().chunkLoadedAt(chunkLocation.x, chunkLocation.z)) {
		_handle(world);
    }
}

void PlayerDigEvent::_handle(World &world)
{
	auto x = m_digSpot.x;
	auto y = m_digSpot.y;
	auto z = m_digSpot.z;

	BlockId heldItemId = m_pPlayer->getHeldItems().getBlockId();

	switch (m_buttonPress) {

	case sf::Mouse::Button::Left: {
		if ((int)y == 0)
			return;

		ChunkBlock block = world.getBlock(x, y, z);
		BlockId blockId = block.getData().id;

		if (BlockDatabase::get().isDoublePlant(blockId)) {
			breakDoublePlant(world, glm::vec3(x, y, z), blockId);
		}
		else {
			bool blockDrops = false;
			bool newBlockPlaced = false;

			switch (block.getData().toolToMine)
			{
			case ToolToMine::None:
			case ToolToMine::Shovel:
			case ToolToMine::Axe:
				blockDrops = true;
				break;
			case ToolToMine::Pickaxe:
				if (BlockDatabase::get().canPickaxeMine(heldItemId, blockId))
					blockDrops = true;
				break;
			}

			auto lightChar = world.getBlock(x, y, z).light;

			if (blockDrops)
				dropItems(world, blockId, x, y, z, newBlockPlaced);

			if (!newBlockPlaced)
				world.setBlock(x, y, z, 0);

			if (block.getData().isOpaque &&
				block.getData().id != BlockId::Glowstone) { // @TODO refactor to get rid of glowstone while calculating sun light

				if ((int)(lightChar & 0xF) > 0) {
					world.setTorchLight(x, y, z, (int)lightChar);
					world.updateLitChunks(x, y, z);
				}
				if ((int)((lightChar >> 4) & 0xF) > 0) {
					world.updateSunLight(x, y, z);
				}
			}

			breakBlocksAbove(world, glm::vec3(x, y + 1, z));
		}

		makeBreakSound(blockId);
		break;
	}

	case sf::Mouse::Button::Right: {

		if (BlockDatabase::canPlaceOnBlock(heldItemId, world.getBlock(x, y - 1, z).getData().id)) {

			if (world.getBlock(x + 1, y, z).getData().id == BlockId::Cactus ||
				world.getBlock(x - 1, y, z).getData().id == BlockId::Cactus ||
				world.getBlock(x, y, z + 1).getData().id == BlockId::Cactus ||
				world.getBlock(x, y, z - 1).getData().id == BlockId::Cactus)
				return;

			bool blockPlaced = placeBlock(world, heldItemId, x, y, z);

			if (blockPlaced) {
				world.checkForDroppedItems(glm::vec3(floor(x), floor(y), floor(z)));
				m_pPlayer->removeHeldItem(1);
				m_pHand->swing();
				makePlaceSound(heldItemId);
			}
		}
		break;
	}
	default:
		break;
	}
}

void PlayerDigEvent::breakBlocksAbove(World & world, const glm::vec3 &pos)
{
	float y = pos.y;
	auto block = world.getBlock(pos.x, y, pos.z);
	BlockId blockId = block.getData().id;

	while (blockId == BlockId::Cactus ||
		blockId == BlockId::SugarCane ||
		(block.getData().shaderType == BlockShaderType::Flora && !block.getData().isCollidable))
	{
		switch (blockId)
		{
		case BlockId::LargeFern1:
		case BlockId::Lilac1:
		case BlockId::Peony1:
		case BlockId::RoseBush1:
			world.addDroppedItem(
				{ BlockDatabase::getDoublePlantMain(blockId), 1 },
				glm::vec3(floor(pos.x) + 0.5f, floor(y) + 0.5f, floor(pos.z) + 0.5f)
			);
			if (world.getBlock(pos.x, y + 1, pos.z).getData().id == BlockDatabase::getDoublePlantSecondPart(blockId))
				world.setBlock(pos.x, y + 1, pos.z, 0);
			break;

		default:
			world.addDroppedItem(
				{ blockId, 1 },
				glm::vec3(floor(pos.x) + 0.5f, floor(y) + 0.5f, floor(pos.z) + 0.5f)
			);
			break;
		}
		world.setBlock(pos.x, y, pos.z, 0);

		++y;
		block = world.getBlock(pos.x, y, pos.z);
		blockId = block.getData().id;
	}
}

void PlayerDigEvent::breakDoublePlant(World & world, const glm::vec3 & pos, BlockId brokenPlant)
{
	int secondPartPosition;

	switch (brokenPlant)
	{
	case BlockId::LargeFern1:
	case BlockId::Lilac1:
	case BlockId::Peony1:
	case BlockId::RoseBush1:
		secondPartPosition = 1;
		break;
	case BlockId::LargeFern2:
	case BlockId::Lilac2:
	case BlockId::Peony2:
	case BlockId::RoseBush2:
		secondPartPosition = -1;
		break;
	default:
		return;
	}
	world.setBlock(pos.x, pos.y, pos.z, 0);
	world.addDroppedItem(
		{ BlockDatabase::getDoublePlantMain(brokenPlant), 1 },
		glm::vec3(floor(pos.x) + 0.5f, floor(pos.y) + 0.5f, floor(pos.z) + 0.5f));
	if (world.getBlock(pos.x, pos.y + secondPartPosition, pos.z).getData().id == BlockDatabase::getDoublePlantSecondPart(brokenPlant))
			world.setBlock(pos.x, pos.y + secondPartPosition, pos.z, 0);
}

void PlayerDigEvent::dropItems(World &world, BlockId blockId, float x, float y, float z, bool &newBlockPlaced)
{
	switch (blockId)
	{
	case BlockId::Stone:
		world.addDroppedItem({ BlockId::Cobblestone, 1 },
			glm::vec3(floor(x) + 0.5f, floor(y) + 0.5f, floor(z) + 0.5f));
		break;
	case BlockId::CoalOre:
		world.addDroppedItem({ BlockId::Coal, 1 },
			glm::vec3(floor(x) + 0.5f, floor(y) + 0.5f, floor(z) + 0.5f));
		break;
	case BlockId::DiamondOre:
		world.addDroppedItem({ BlockId::Diamond, 1 },
			glm::vec3(floor(x) + 0.5f, floor(y) + 0.5f, floor(z) + 0.5f));
		break;
	case BlockId::OakLeaf:
		world.addDroppedItem({ blockId, 1 },
			glm::vec3(floor(x) + 0.5f, floor(y) + 0.5f, floor(z) + 0.5f));

		if (rand() % 10 == 0) // % 200 in real Minecraft
			world.addDroppedItem({ BlockId::Apple, 1 },
				glm::vec3(floor(x) + 0.5f, floor(y) + 0.5f, floor(z) + 0.5f));
		break;
	case BlockId::Ice:
		if (y <= WATER_LEVEL + 1) {
			world.setBlock(x, y, z, BlockId::Water);
			newBlockPlaced = true;
		}
		break;
	case BlockId::Glowstone:
		world.addDroppedItem({ blockId, 1 },
			glm::vec3(floor(x) + 0.5f, floor(y) + 0.5f, floor(z) + 0.5f));
		world.removeTorchLight(x, y, z);
		world.updateLitChunks(x, y, z);
		break;
	default:
		world.addDroppedItem({ blockId, 1 },
			glm::vec3(floor(x) + 0.5f, floor(y) + 0.5f, floor(z) + 0.5f));
		break;
	}
}

bool PlayerDigEvent::placeBlock(World & world, BlockId heldItemId, float x, float y, float z)
{
	switch (heldItemId)
	{
	case BlockId::LargeFern:
		if (world.getBlock(x, y + 1, z) != 0)
			return false;
		world.setBlock(x, y, z, { BlockId::LargeFern1 });
		world.setBlock(x, y + 1, z, { BlockId::LargeFern2 });
		break;
	case BlockId::Lilac:
		if (world.getBlock(x, y + 1, z) != 0)
			return false;
		world.setBlock(x, y, z, { BlockId::Lilac1 });
		world.setBlock(x, y + 1, z, { BlockId::Lilac2 });
		break;
	case BlockId::Peony:
		if (world.getBlock(x, y + 1, z) != 0)
			return false;
		world.setBlock(x, y, z, { BlockId::Peony1 });
		world.setBlock(x, y + 1, z, { BlockId::Peony2 });
		break;
	case BlockId::RoseBush:
		if (world.getBlock(x, y + 1, z) != 0)
			return false;
		world.setBlock(x, y, z, { BlockId::RoseBush1 });
		world.setBlock(x, y + 1, z, { BlockId::RoseBush2 });
		break;
	case BlockId::Cactus:
		if (world.getBlock(x + 1, y, z) != 0 ||
			world.getBlock(x - 1, y, z) != 0 ||
			world.getBlock(x, y, z + 1) != 0 ||
			world.getBlock(x, y, z - 1) != 0)
		{
			return false;
		}
		world.setBlock(x, y, z, heldItemId);
		break;
	case BlockId::Glowstone:
		world.setTorchLight(x, y, z, 15);
		world.setBlock(x, y, z, heldItemId);
		world.updateLitChunks(x, y, z);
		break;
	default:
		world.setBlock(x, y, z, heldItemId);
		break;
	}

	return true;
}
