#pragma once

#include "SFML/System.hpp"

#include "World/Block/BlockId.h"
#include "Sound/SoundId.h"
#include "Entity.h"

sf::Vector3f calculateSoundPosition(sf::Vector3f &diffSoundAndListenerPos, float listenerRotationY);

float distanceFromSound(sf::Vector3f &diffSoundAndListenerPos);

void makeBreakSound(BlockId id);
void makePlaceSound(BlockId id);
void makeStepSound(BlockId id);
void makeFallSound(BlockId id);
void makeHitSound(BlockId id);