#include "PlayerDigEvent.h"

#include "../../Item/Material.h"
#include "../../Player/Player.h"
#include "../World.h"

PlayerDigEvent::PlayerDigEvent(sf::Mouse::Button button,
                               const glm::vec3 &location, Player &player)
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
			m_pPlayer->addItem(material, 1);
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
