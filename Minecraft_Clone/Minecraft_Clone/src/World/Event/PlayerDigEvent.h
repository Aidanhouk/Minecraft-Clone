#pragma once

#include <SFML/Graphics.hpp>

#include "../../Maths/glm.h"
#include "IWorldEvent.h"
#include "World/Block/BlockId.h"
#include "Util/Random.h"

class Player;
class Hand;

class PlayerDigEvent : public IWorldEvent {
public:
    PlayerDigEvent(sf::Mouse::Button button, const glm::vec3 &location,
                   Player &player, Hand &hand);

    void handle(World &world) override;

private:
    void _handle(World &world);
	void dropItems(World &world, BlockId blockId, float x, float y, float z, bool &newBlockPlaced);
	bool placeBlock(World &world, BlockId heldItemId, float x, float y, float z);
	
	void breakBlocksAbove(World &world, const glm::vec3 &pos);
	void breakDoublePlant(World &world, const glm::vec3 &pos, BlockId brokenPlant);

    sf::Mouse::Button m_buttonPress;
    glm::vec3 m_digSpot;
    Player *m_pPlayer;
	Hand *m_pHand;
	Random<std::minstd_rand> m_random;
};