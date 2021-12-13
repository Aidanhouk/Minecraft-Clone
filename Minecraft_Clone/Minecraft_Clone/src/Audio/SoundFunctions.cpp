#include "SoundFunctions.h"

#include "Maths/glm.h"
#include "SoundMaster.h"

#include <iostream>

sf::Vector3f calculateSoundPosition(sf::Vector3f & diffSoundAndListenerPos, float listenerRotationY)
{
	float diffX = diffSoundAndListenerPos.x;
	float diffY = diffSoundAndListenerPos.y;
	float diffZ = diffSoundAndListenerPos.z;

	float itemDegree;
	if (diffX >= 0.0f && diffZ < 0.0f) {
		float tan = diffX / -diffZ;
		itemDegree = glm::degrees(glm::atan(tan));
	}
	else if (diffX >= 0.0f && diffZ >= 0.0f) {
		float tan = diffX / diffZ;
		itemDegree = 180.0f - glm::degrees(glm::atan(tan));
	}
	else if (diffX < 0.0f && diffZ >= 0.0f) {
		float tan = -diffX / diffZ;
		itemDegree = 180.0f + glm::degrees(glm::atan(tan));
	}
	else if (diffX < 0.0f && diffZ < 0.0f) {
		float tan = -diffX / -diffZ;
		itemDegree = 360.0f - glm::degrees(glm::atan(tan));
	}

	float finalDegree = 360.0f - listenerRotationY + itemDegree;
	if (finalDegree >= 360.0f)
		finalDegree -= 360.0f;

	if (finalDegree < 90.0f) {
		float tan = glm::tan(glm::radians(finalDegree));
		diffX = tan;
		diffZ = -1.0f;
	}
	else if (finalDegree < 180.0f) {
		float tan = glm::tan(glm::radians(finalDegree - 90.0f));
		diffX = 1.0f;
		diffZ = tan;
	}
	else if (finalDegree < 270.0f) {
		float tan = glm::tan(glm::radians(finalDegree - 180.0f));
		diffX = -tan;
		diffZ = 1.0f;
	}
	else if (finalDegree < 360.0f) {
		float tan = glm::tan(glm::radians(finalDegree - 270.0f));
		diffX = -1.0f;
		diffZ = tan;
	}

	float maxDiff = std::max(glm::abs(diffX), glm::abs(diffZ));
	diffX /= maxDiff;
	diffZ /= maxDiff;

	return { diffX, 0.0f, diffZ };
}

float distanceFromSound(sf::Vector3f & diffSoundAndListenerPos)
{
	float diffX = diffSoundAndListenerPos.x;
	float diffY = diffSoundAndListenerPos.y;
	float diffZ = diffSoundAndListenerPos.z;

	float distance = glm::pow(diffX * diffX + diffY * diffY + diffZ * diffZ, 0.5f);

	return distance;
}

void makeBreakSound(BlockId id)
{
	SoundSet *soundSet = nullptr;

	switch (id)
	{
	case BlockId::Ice:
	case BlockId::Glowstone:
		soundSet = &SoundSet::BreakGlassSet;
		break;
	case BlockId::Stone:
	case BlockId::Cobblestone:
	case BlockId::Bedrock:
	case BlockId::CoalOre:
	case BlockId::IronOre:
	case BlockId::RedstoneOre:
	case BlockId::GoldOre:
	case BlockId::DiamondOre:
	case BlockId::EmeraldOre:
		soundSet = &SoundSet::BreakStoneSet;
		break;
	case BlockId::Dirt:
	case BlockId::Gravel:
	case BlockId::Clay:
		soundSet = &SoundSet::BreakGravelSet;
		break;
	case BlockId::Snow:
		soundSet = &SoundSet::BreakSnowSet;
		break;
	case BlockId::Sand:
		soundSet = &SoundSet::BreakSandSet;
		break;
	case BlockId::Grass:
	case BlockId::TundraGrass:
	case BlockId::BirchLeaf:
	case BlockId::OakLeaf:
	case BlockId::PalmLeaf:
	case BlockId::SpruceLeaf:
	case BlockId::Rose:
	case BlockId::TallGrass:
	case BlockId::DeadShrub:
	case BlockId::Dandelion:
	case BlockId::Fern:
	case BlockId::LargeFern1:
	case BlockId::LargeFern2:
	case BlockId::LargeFern:
	case BlockId::SugarCane:

	case BlockId::AzureBluet:
	case BlockId::BlueOrchid:
	case BlockId::CornFlower:
	case BlockId::LilyOfTheValley:
	case BlockId::OrangeTulip:
	case BlockId::OxeyeDaisy:
	case BlockId::PinkTulip:
	case BlockId::RedTulip:
	case BlockId::WhiteTulip:

	case BlockId::Lilac1:
	case BlockId::Lilac2:
	case BlockId::Lilac:
	case BlockId::Peony1:
	case BlockId::Peony2:
	case BlockId::Peony:
	case BlockId::RoseBush1:
	case BlockId::RoseBush2:
	case BlockId::RoseBush:
		soundSet = &SoundSet::BreakGrassSet;
		break;
	case BlockId::Cactus:
		soundSet = &SoundSet::StepWoolSet;
		break;
	case BlockId::OakBark:
	case BlockId::BirchBark:
	case BlockId::PalmBark:
	case BlockId::SpruceBark:
	case BlockId::OakPlanks:
	case BlockId::BirchPlanks:
	case BlockId::PalmPlanks:
	case BlockId::SprucePlanks:
	case BlockId::CraftingTable:
		soundSet = &SoundSet::BreakWoodSet;
		break;
	default:
		break;
	}

	if (soundSet) {
		g_SoundMaster.setVolumeAll(*soundSet, 50.0f);
		g_SoundMaster.setPitchAll(*soundSet, 0.8f);
		g_SoundMaster.playRandom(*soundSet);
	}
}

void makePlaceSound(BlockId id)
{
	SoundSet *soundSet = nullptr;

	switch (id)
	{
	case BlockId::Ice:
	case BlockId::Glowstone:
	case BlockId::Bedrock:
	case BlockId::Stone:
	case BlockId::Cobblestone:
	case BlockId::CoalOre:
	case BlockId::IronOre:
	case BlockId::RedstoneOre:
	case BlockId::GoldOre:
	case BlockId::DiamondOre:
	case BlockId::EmeraldOre:
		soundSet = &SoundSet::BreakStoneSet;
		break;
	case BlockId::Dirt:
	case BlockId::Gravel:
	case BlockId::Clay:
		soundSet = &SoundSet::BreakGravelSet;
		break;
	case BlockId::Snow:
		soundSet = &SoundSet::BreakSnowSet;
		break;
	case BlockId::Sand:
		soundSet = &SoundSet::BreakSandSet;
		break;
	case BlockId::Grass:
	case BlockId::TundraGrass:
	case BlockId::BirchLeaf:
	case BlockId::OakLeaf:
	case BlockId::PalmLeaf:
	case BlockId::SpruceLeaf:
	case BlockId::Rose:
	case BlockId::TallGrass:
	case BlockId::DeadShrub:
	case BlockId::Dandelion:
	case BlockId::Fern:
	case BlockId::LargeFern1:
	case BlockId::LargeFern2:
	case BlockId::LargeFern:
	case BlockId::SugarCane:

	case BlockId::AzureBluet:
	case BlockId::BlueOrchid:
	case BlockId::CornFlower:
	case BlockId::LilyOfTheValley:
	case BlockId::OrangeTulip:
	case BlockId::OxeyeDaisy:
	case BlockId::PinkTulip:
	case BlockId::RedTulip:
	case BlockId::WhiteTulip:

	case BlockId::Lilac1:
	case BlockId::Lilac2:
	case BlockId::Lilac:
	case BlockId::Peony1:
	case BlockId::Peony2:
	case BlockId::Peony:
	case BlockId::RoseBush1:
	case BlockId::RoseBush2:
	case BlockId::RoseBush:
		soundSet = &SoundSet::BreakGrassSet;
		break;
	case BlockId::Cactus:
		soundSet = &SoundSet::StepWoolSet;
		break;
	case BlockId::OakBark:
	case BlockId::BirchBark:
	case BlockId::PalmBark:
	case BlockId::SpruceBark:
	case BlockId::OakPlanks:
	case BlockId::BirchPlanks:
	case BlockId::PalmPlanks:
	case BlockId::SprucePlanks:
	case BlockId::CraftingTable:
		soundSet = &SoundSet::BreakWoodSet;
		break;
	default:
		break;
	}

	if (soundSet) {
		g_SoundMaster.setVolumeAll(*soundSet, 50.0f);
		//g_SoundMaster.setVolumeAll(*soundSet, 100.0f);
		g_SoundMaster.setPitchAll(*soundSet, 0.8f);
		g_SoundMaster.playRandom(*soundSet);
	}
}

void makeStepSound(BlockId id)
{
	SoundSet *soundSet = nullptr;

	switch (id)
	{
	case BlockId::Stone:
	case BlockId::Cobblestone:
	case BlockId::Bedrock:
	case BlockId::Ice:
	case BlockId::Glowstone:
	case BlockId::CoalOre:
	case BlockId::IronOre:
	case BlockId::RedstoneOre:
	case BlockId::GoldOre:
	case BlockId::DiamondOre:
	case BlockId::EmeraldOre:
		soundSet = &SoundSet::StepStoneSet;
		break;
	case BlockId::Grass:
	case BlockId::TundraGrass:
	case BlockId::BirchLeaf:
	case BlockId::OakLeaf:
	case BlockId::PalmLeaf:
	case BlockId::SpruceLeaf:
		soundSet = &SoundSet::StepGrassSet;
		break;
	case BlockId::Sand:
		soundSet = &SoundSet::StepSandSet;
		break;
	case BlockId::Snow:
		soundSet = &SoundSet::StepSnowSet;
		break;
	case BlockId::Gravel:
	case BlockId::Dirt:
	case BlockId::Clay:
		soundSet = &SoundSet::StepGravelSet;
		break;
	case BlockId::BirchBark:
	case BlockId::OakBark:
	case BlockId::PalmBark:
	case BlockId::SpruceBark:
	case BlockId::OakPlanks:
	case BlockId::BirchPlanks:
	case BlockId::PalmPlanks:
	case BlockId::SprucePlanks:
	case BlockId::CraftingTable:
		soundSet = &SoundSet::StepWoodSet;
		break;
	case BlockId::Cactus:
		soundSet = &SoundSet::StepWoolSet;
		break;
	default:
		break;
	}

	if (soundSet) {
		g_SoundMaster.setVolumeAll(*soundSet, 15.0f);
		g_SoundMaster.setPitchAll(*soundSet, 1.0f);
		g_SoundMaster.playRandom(*soundSet);
	}
}

void makeFallSound(BlockId id)
{
	SoundSet *soundSet = nullptr;

	switch (id)
	{
	case BlockId::Stone:
	case BlockId::Cobblestone:
	case BlockId::Bedrock:
	case BlockId::Ice:
	case BlockId::Glowstone:
	case BlockId::CoalOre:
	case BlockId::IronOre:
	case BlockId::RedstoneOre:
	case BlockId::GoldOre:
	case BlockId::DiamondOre:
	case BlockId::EmeraldOre:
		soundSet = &SoundSet::StepStoneSet;
	case BlockId::Dirt:
	case BlockId::Gravel:
	case BlockId::Clay:
		soundSet = &SoundSet::StepGravelSet;
		break;
	case BlockId::Snow:
		soundSet = &SoundSet::StepSnowSet;
		break;
	case BlockId::Sand:
		soundSet = &SoundSet::StepSandSet;
		break;
	case BlockId::Grass:
	case BlockId::TundraGrass:
	case BlockId::BirchLeaf:
	case BlockId::OakLeaf:
	case BlockId::PalmLeaf:
	case BlockId::SpruceLeaf:
		soundSet = &SoundSet::StepGrassSet;
		break;
		break;
	case BlockId::OakBark:
	case BlockId::BirchBark:
	case BlockId::PalmBark:
	case BlockId::SpruceBark:
	case BlockId::OakPlanks:
	case BlockId::BirchPlanks:
	case BlockId::PalmPlanks:
	case BlockId::SprucePlanks:
	case BlockId::CraftingTable:
		soundSet = &SoundSet::StepWoodSet;
		break;
	case BlockId::Cactus:
		soundSet = &SoundSet::StepWoolSet;
		break;
	default:
		break;
	}

	if (soundSet) {
		g_SoundMaster.setVolumeAll(*soundSet, 50.0f);
		g_SoundMaster.setPitchAll(*soundSet, 0.75f);
		g_SoundMaster.playRandom(*soundSet);
	}
}

void makeHitSound(BlockId id)
{
	SoundSet *soundSet = nullptr;

	switch (id)
	{
	case BlockId::Stone:
	case BlockId::Cobblestone:
	case BlockId::Bedrock:
	case BlockId::Ice:
	case BlockId::Glowstone:
	case BlockId::CoalOre:
	case BlockId::IronOre:
	case BlockId::RedstoneOre:
	case BlockId::GoldOre:
	case BlockId::DiamondOre:
	case BlockId::EmeraldOre:
		soundSet = &SoundSet::StepStoneSet;
		break;
	case BlockId::Dirt:
	case BlockId::Gravel:
	case BlockId::Clay:
		soundSet = &SoundSet::StepGravelSet;
		break;
	case BlockId::Snow:
		soundSet = &SoundSet::StepSnowSet;
		break;
	case BlockId::Sand:
		soundSet = &SoundSet::StepSandSet;
		break;
	case BlockId::Grass:
	case BlockId::TundraGrass:
	case BlockId::BirchLeaf:
	case BlockId::OakLeaf:
	case BlockId::PalmLeaf:
	case BlockId::SpruceLeaf:
	case BlockId::Rose:
	case BlockId::TallGrass:
	case BlockId::DeadShrub:
	case BlockId::Dandelion:
	case BlockId::Fern:
	case BlockId::LargeFern1:
	case BlockId::LargeFern2:
	case BlockId::LargeFern:
	case BlockId::SugarCane:

	case BlockId::AzureBluet:
	case BlockId::BlueOrchid:
	case BlockId::CornFlower:
	case BlockId::LilyOfTheValley:
	case BlockId::OrangeTulip:
	case BlockId::OxeyeDaisy:
	case BlockId::PinkTulip:
	case BlockId::RedTulip:
	case BlockId::WhiteTulip:

	case BlockId::Lilac1:
	case BlockId::Lilac2:
	case BlockId::Lilac:
	case BlockId::Peony1:
	case BlockId::Peony2:
	case BlockId::Peony:
	case BlockId::RoseBush1:
	case BlockId::RoseBush2:
	case BlockId::RoseBush:
		soundSet = &SoundSet::StepGrassSet;
		break;
	case BlockId::OakBark:
	case BlockId::BirchBark:
	case BlockId::PalmBark:
	case BlockId::SpruceBark:
	case BlockId::OakPlanks:
	case BlockId::BirchPlanks:
	case BlockId::PalmPlanks:
	case BlockId::SprucePlanks:
	case BlockId::CraftingTable:
		soundSet = &SoundSet::StepWoodSet;
		break;
	case BlockId::Cactus:
		soundSet = &SoundSet::StepWoolSet;
		break;
	default:
		break;
	}

	if (soundSet) {
		g_SoundMaster.setVolumeAll(*soundSet, 25.0f);
		g_SoundMaster.setPitchAll(*soundSet, 0.5f);
		g_SoundMaster.playRandom(*soundSet);
	}
}
