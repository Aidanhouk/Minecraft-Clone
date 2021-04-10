#include "PlayerDigEvent.h"

#include "../../Item/Material.h"
#include "../../Player/Player.h"
#include "../World.h"

PlayerDigEvent::PlayerDigEvent(sf::Mouse::Button button, const glm::vec3 &location, Player &player)
    : m_buttonPress(button)
    , m_digSpot(location)
    , m_pPlayer(&player)
{
}

void PlayerDigEvent::handle(World &world)
{
    auto chunkLocation = World::getChunkXZ(static_cast<int>(m_digSpot.x), static_cast<int>(m_digSpot.z));

    if (world.getChunkManager().chunkLoadedAt(chunkLocation.x, chunkLocation.z)) {
        dig(world);
    }
}

void PlayerDigEvent::dig(World &world)
{
    auto x = m_digSpot.x;
    auto y = m_digSpot.y;
    auto z = m_digSpot.z;
    switch (m_buttonPress) {
        case sf::Mouse::Button::Left: {
            auto block = world.getBlock(x, y, z);
            const auto &material = Material::toMaterial((BlockId)block.id);
			world.addDroppedItem(
				ItemStack(material, 1),
				glm::vec3(floor(x) + 0.5f, floor(y) + 0.5f, floor(z) + 0.5f)
			);
			world.blockBroken(glm::vec3(floor(x), floor(y), floor(z)));
			breakBlocksAbove(world, glm::vec3(x, y + 1, z));
            world.updateChunk(x, y, z);
			if (material.id == Material::Ice && y <= WATER_LEVEL + 1)
				world.setBlock(x, y, z, BlockId::Water);
			else
				world.setBlock(x, y, z, 0);
            break;
        }

        case sf::Mouse::Button::Right: {
            auto &stack = m_pPlayer->getHeldItems();
            auto &material = stack.getMaterial();

            if (material.id == Material::ID::Nothing) {
                return;
            }
            else {
				m_pPlayer->removeHeldItem(1);
                world.updateChunk(x, y, z);
                world.setBlock(x, y, z, material.toBlockID());
                break;
            }
        }
        default:
            break;
    }
}

void PlayerDigEvent::breakBlocksAbove(World & world, const glm::vec3 &pos)
{
	float y = pos.y;
	auto block = world.getBlock(pos.x, y, pos.z);
	/// Probably add a feature for those blocks
	while (block.getData().id == BlockId::Cactus
		|| block.getData().id == BlockId::SugarCane
		|| block.getData().id == BlockId::Rose
		|| block.getData().id == BlockId::DeadShrub
		|| block.getData().id == BlockId::TallGrass)
	{
		world.addDroppedItem(
			ItemStack(Material::toMaterial((BlockId)block.id), 1),
			glm::vec3(floor(pos.x) + 0.5f, floor(y) + 0.5f, floor(pos.z) + 0.5f)
		);
		world.blockBroken(glm::vec3(floor(pos.x), floor(y), floor(pos.z)));

		world.setBlock(pos.x, y, pos.z, 0);
		/// Should update chunk each time beacause of the multithreading
		world.updateChunk(pos.x, y, pos.z);

		++y;
		block = world.getBlock(pos.x, y, pos.z);
	}
}
