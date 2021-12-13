#include "BlockDatabase.h"

#include "ChunkBlock.h"

BlockDatabase::BlockDatabase()
    : textureAtlas("DefaultPack", 256, 16)
{
    m_blocks[(int)BlockId::Air] =				std::make_unique<DefaultBlock>("Air");
	//m_blocks[(int)BlockId::CaveAir] =			std::make_unique<DefaultBlock>("Air");
    m_blocks[(int)BlockId::Water] =				std::make_unique<DefaultBlock>("Water");
	m_blocks[(int)BlockId::Ice] =				std::make_unique<DefaultBlock>("Ice");

    m_blocks[(int)BlockId::OakBark] =			std::make_unique<DefaultBlock>("Wood/OakBark");
	m_blocks[(int)BlockId::BirchBark] =			std::make_unique<DefaultBlock>("Wood/BirchBark");
	m_blocks[(int)BlockId::PalmBark] =			std::make_unique<DefaultBlock>("Wood/PalmBark");
	m_blocks[(int)BlockId::SpruceBark] =		std::make_unique<DefaultBlock>("Wood/SpruceBark");
	m_blocks[(int)BlockId::OakPlanks] =			std::make_unique<DefaultBlock>("Wood/OakPlanks");
	m_blocks[(int)BlockId::BirchPlanks] =		std::make_unique<DefaultBlock>("Wood/BirchPlanks");
	m_blocks[(int)BlockId::PalmPlanks] =		std::make_unique<DefaultBlock>("Wood/PalmPlanks");
	m_blocks[(int)BlockId::SprucePlanks] =		std::make_unique<DefaultBlock>("Wood/SprucePlanks");

    m_blocks[(int)BlockId::OakLeaf] =			std::make_unique<DefaultBlock>("Leaves/OakLeaf");
	m_blocks[(int)BlockId::PalmLeaf] =			std::make_unique<DefaultBlock>("Leaves/PalmLeaf");
	m_blocks[(int)BlockId::BirchLeaf] =			std::make_unique<DefaultBlock>("Leaves/BirchLeaf");
	m_blocks[(int)BlockId::SpruceLeaf] =		std::make_unique<DefaultBlock>("Leaves/SpruceLeaf");

    m_blocks[(int)BlockId::Grass] =				std::make_unique<DefaultBlock>("CommonSolid/Grass");
    m_blocks[(int)BlockId::Dirt] =				std::make_unique<DefaultBlock>("CommonSolid/Dirt");
    m_blocks[(int)BlockId::Stone] =				std::make_unique<DefaultBlock>("CommonSolid/Stone");
    m_blocks[(int)BlockId::Sand] =				std::make_unique<DefaultBlock>("CommonSolid/Sand");
	m_blocks[(int)BlockId::Snow] =				std::make_unique<DefaultBlock>("CommonSolid/Snow");
	m_blocks[(int)BlockId::Bedrock] =			std::make_unique<DefaultBlock>("CommonSolid/Bedrock");
	m_blocks[(int)BlockId::TundraGrass] =		std::make_unique<DefaultBlock>("CommonSolid/TundraGrass");
	m_blocks[(int)BlockId::Gravel] =			std::make_unique<DefaultBlock>("CommonSolid/Gravel");
	m_blocks[(int)BlockId::Clay] =				std::make_unique<DefaultBlock>("CommonSolid/Clay");
	m_blocks[(int)BlockId::Cobblestone] =		std::make_unique<DefaultBlock>("CommonSolid/Cobblestone");
	m_blocks[(int)BlockId::Glowstone] =			std::make_unique<DefaultBlock>("CommonSolid/Glowstone");

    m_blocks[(int)BlockId::Cactus] =			std::make_unique<DefaultBlock>("Plants/Cactus");
    m_blocks[(int)BlockId::TallGrass] =			std::make_unique<DefaultBlock>("Plants/TallGrass");
	m_blocks[(int)BlockId::DeadShrub] =			std::make_unique<DefaultBlock>("Plants/DeadShrub");
	m_blocks[(int)BlockId::SugarCane] =			std::make_unique<DefaultBlock>("Plants/SugarCane");
	m_blocks[(int)BlockId::Fern] =				std::make_unique<DefaultBlock>("Plants/Fern");

    m_blocks[(int)BlockId::Rose] =				std::make_unique<DefaultBlock>("Plants/Flowers/Rose");
	m_blocks[(int)BlockId::Dandelion] =			std::make_unique<DefaultBlock>("Plants/Flowers/Dandelion");
	m_blocks[(int)BlockId::AzureBluet] =		std::make_unique<DefaultBlock>("Plants/Flowers/AzureBluet");
	m_blocks[(int)BlockId::BlueOrchid] =		std::make_unique<DefaultBlock>("Plants/Flowers/BlueOrchid");
	m_blocks[(int)BlockId::CornFlower] =		std::make_unique<DefaultBlock>("Plants/Flowers/CornFlower");
	m_blocks[(int)BlockId::LilyOfTheValley] =	std::make_unique<DefaultBlock>("Plants/Flowers/LilyOfTheValley");
	m_blocks[(int)BlockId::OrangeTulip] =		std::make_unique<DefaultBlock>("Plants/Flowers/OrangeTulip");
	m_blocks[(int)BlockId::OxeyeDaisy] =		std::make_unique<DefaultBlock>("Plants/Flowers/OxeyeDaisy");
	m_blocks[(int)BlockId::PinkTulip] =			std::make_unique<DefaultBlock>("Plants/Flowers/PinkTulip");
	m_blocks[(int)BlockId::RedTulip] =			std::make_unique<DefaultBlock>("Plants/Flowers/RedTulip");
	m_blocks[(int)BlockId::WhiteTulip] =		std::make_unique<DefaultBlock>("Plants/Flowers/WhiteTulip");


	m_blocks[(int)BlockId::LargeFern1] =		std::make_unique<DefaultBlock>("Plants/LargeFern1");
	m_blocks[(int)BlockId::LargeFern2] =		std::make_unique<DefaultBlock>("Plants/LargeFern2");
	m_blocks[(int)BlockId::LargeFern] =			std::make_unique<DefaultBlock>("Plants/LargeFern");
	m_blocks[(int)BlockId::Lilac1] =			std::make_unique<DefaultBlock>("Plants/Flowers/Lilac1");
	m_blocks[(int)BlockId::Lilac2] =			std::make_unique<DefaultBlock>("Plants/Flowers/Lilac2");
	m_blocks[(int)BlockId::Lilac] =				std::make_unique<DefaultBlock>("Plants/Flowers/Lilac");
	m_blocks[(int)BlockId::Peony1] =			std::make_unique<DefaultBlock>("Plants/Flowers/Peony1");
	m_blocks[(int)BlockId::Peony2] =			std::make_unique<DefaultBlock>("Plants/Flowers/Peony2");
	m_blocks[(int)BlockId::Peony] =				std::make_unique<DefaultBlock>("Plants/Flowers/Peony");
	m_blocks[(int)BlockId::RoseBush1] =			std::make_unique<DefaultBlock>("Plants/Flowers/RoseBush1");
	m_blocks[(int)BlockId::RoseBush2] =			std::make_unique<DefaultBlock>("Plants/Flowers/RoseBush2");
	m_blocks[(int)BlockId::RoseBush] =			std::make_unique<DefaultBlock>("Plants/Flowers/RoseBush");

	m_blocks[(int)BlockId::CoalOre] =			std::make_unique<DefaultBlock>("Ore/CoalOre");
	m_blocks[(int)BlockId::IronOre] =			std::make_unique<DefaultBlock>("Ore/IronOre");
	m_blocks[(int)BlockId::RedstoneOre] =		std::make_unique<DefaultBlock>("Ore/RedstoneOre");
	m_blocks[(int)BlockId::GoldOre] =			std::make_unique<DefaultBlock>("Ore/GoldOre");
	m_blocks[(int)BlockId::DiamondOre] =		std::make_unique<DefaultBlock>("Ore/DiamondOre");
	m_blocks[(int)BlockId::EmeraldOre] =		std::make_unique<DefaultBlock>("Ore/EmeraldOre");

	m_blocks[(int)BlockId::Apple] =				std::make_unique<DefaultBlock>("Food/Apple");

	m_blocks[(int)BlockId::Stick] =				std::make_unique<DefaultBlock>("Misc/Stick");

	m_blocks[(int)BlockId::Diamond] =			std::make_unique<DefaultBlock>("Material/Diamond");
	m_blocks[(int)BlockId::Coal] =				std::make_unique<DefaultBlock>("Material/Coal");

	m_blocks[(int)BlockId::WoodenSword] =		std::make_unique<DefaultBlock>("Tools/WoodenSword");
	m_blocks[(int)BlockId::WoodenPickaxe] =		std::make_unique<DefaultBlock>("Tools/WoodenPickaxe");
	m_blocks[(int)BlockId::WoodenShovel] =		std::make_unique<DefaultBlock>("Tools/WoodenShovel");
	m_blocks[(int)BlockId::WoodenAxe] =			std::make_unique<DefaultBlock>("Tools/WoodenAxe");

	m_blocks[(int)BlockId::StoneSword] =		std::make_unique<DefaultBlock>("Tools/StoneSword");
	m_blocks[(int)BlockId::StonePickaxe] =		std::make_unique<DefaultBlock>("Tools/StonePickaxe");
	m_blocks[(int)BlockId::StoneShovel] =		std::make_unique<DefaultBlock>("Tools/StoneShovel");
	m_blocks[(int)BlockId::StoneAxe] =			std::make_unique<DefaultBlock>("Tools/StoneAxe");

	m_blocks[(int)BlockId::IronSword] =			std::make_unique<DefaultBlock>("Tools/IronSword");
	m_blocks[(int)BlockId::IronPickaxe] =		std::make_unique<DefaultBlock>("Tools/IronPickaxe");
	m_blocks[(int)BlockId::IronShovel] =		std::make_unique<DefaultBlock>("Tools/IronShovel");
	m_blocks[(int)BlockId::IronAxe] =			std::make_unique<DefaultBlock>("Tools/IronAxe");

	m_blocks[(int)BlockId::GoldSword] =			std::make_unique<DefaultBlock>("Tools/GoldSword");
	m_blocks[(int)BlockId::GoldPickaxe] =		std::make_unique<DefaultBlock>("Tools/GoldPickaxe");
	m_blocks[(int)BlockId::GoldShovel] =		std::make_unique<DefaultBlock>("Tools/GoldShovel");
	m_blocks[(int)BlockId::GoldAxe] =			std::make_unique<DefaultBlock>("Tools/GoldAxe");

	m_blocks[(int)BlockId::DiamondSword] =		std::make_unique<DefaultBlock>("Tools/DiamondSword");
	m_blocks[(int)BlockId::DiamondPickaxe] =	std::make_unique<DefaultBlock>("Tools/DiamondPickaxe");
	m_blocks[(int)BlockId::DiamondShovel] =		std::make_unique<DefaultBlock>("Tools/DiamondShovel");
	m_blocks[(int)BlockId::DiamondAxe] =		std::make_unique<DefaultBlock>("Tools/DiamondAxe");

	m_blocks[(int)BlockId::CraftingTable] =		std::make_unique<DefaultBlock>("Interactable/CraftingTable");
}

BlockDatabase &BlockDatabase::get()
{
    static BlockDatabase d;
    return d;
}

const BlockType &BlockDatabase::getBlock(BlockId id) const
{
    return *m_blocks[(int)id];
}

const BlockData &BlockDatabase::getData(BlockId id) const
{
    return m_blocks[(int)id]->getData();
}

bool BlockDatabase::canPlaceOnBlock(BlockId blockId, BlockId placeOnThisBlockId)
{
	switch (blockId)
	{
	case BlockId::Cactus:
		switch (placeOnThisBlockId)
		{
		case BlockId::Cactus:
		case BlockId::Sand:
			return true;
		default:
			return false;
		}
	case BlockId::SugarCane:
		if (placeOnThisBlockId == BlockId::SugarCane)
			return true;
	case BlockId::DeadShrub:
		if (placeOnThisBlockId == BlockId::Sand)
			return true;
	case BlockId::AzureBluet:
	case BlockId::BlueOrchid:
	case BlockId::CornFlower:
	case BlockId::Dandelion:
	case BlockId::Fern:
	case BlockId::LargeFern:
	case BlockId::LargeFern1:
	case BlockId::LargeFern2:
	case BlockId::LilyOfTheValley:
	case BlockId::OrangeTulip:
	case BlockId::OxeyeDaisy:
	case BlockId::PinkTulip:
	case BlockId::RedTulip:
	case BlockId::Rose:
	case BlockId::TallGrass:
	case BlockId::WhiteTulip:
	case BlockId::Lilac:
	case BlockId::Lilac1:
	case BlockId::Lilac2:
	case BlockId::Peony:
	case BlockId::Peony1:
	case BlockId::Peony2:
	case BlockId::RoseBush:
	case BlockId::RoseBush1:
	case BlockId::RoseBush2:
		switch (placeOnThisBlockId)
		{
		case BlockId::Dirt:
		case BlockId::Grass:
		case BlockId::Snow:
		case BlockId::TundraGrass:
			return true;
		default:
			return false;
		}
		break;
	default:
		///
		return true;
	}
}

BlockId BlockDatabase::getRandomFlower(Rand &rand)
{
	int rnd = rand.intInRange(1, 11);
	switch (rnd)
	{
	case 1:
		return BlockId::Rose;
	case 2:
		return BlockId::Dandelion;
	case 3:
		return BlockId::AzureBluet;
	case 4:
		return BlockId::BlueOrchid;
	case 5:
		return BlockId::CornFlower;
	case 6:
		return BlockId::LilyOfTheValley;
	case 7:
		return BlockId::OrangeTulip;
	case 8:
		return BlockId::OxeyeDaisy;
	case 9:
		return BlockId::PinkTulip;
	case 10:
		return BlockId::RedTulip;
	case 11:
		return BlockId::WhiteTulip;
	}
}

BlockId BlockDatabase::getRandomDoubleFlower(Rand & rand)
{
	int rnd = rand.intInRange(1, 3);
	switch (rnd)
	{
	case 1:
		return BlockId::Lilac;
	case 2:
		return BlockId::Peony;
	case 3:
		return BlockId::RoseBush;
	}
}

BlockId BlockDatabase::getDoublePlantMain(BlockId blockId)
{
	switch (blockId)
	{
	case BlockId::LargeFern:
	case BlockId::LargeFern1:
	case BlockId::LargeFern2:
		return BlockId::LargeFern;
	case BlockId::Lilac:
	case BlockId::Lilac1:
	case BlockId::Lilac2:
		return BlockId::Lilac;
	case BlockId::Peony:
	case BlockId::Peony1:
	case BlockId::Peony2:
		return BlockId::Peony;
	case BlockId::RoseBush:
	case BlockId::RoseBush1:
	case BlockId::RoseBush2:
		return BlockId::RoseBush;
	}
	//return BlockId();
}

BlockId BlockDatabase::getDoublePlantSecondPart(BlockId blockId)
{
	switch (blockId)
	{
	case BlockId::LargeFern1:
		return BlockId::LargeFern2;
	case BlockId::LargeFern2:
		return BlockId::LargeFern1;
	case BlockId::Lilac1:
		return BlockId::Lilac2;
	case BlockId::Lilac2:
		return BlockId::Lilac1;
	case BlockId::Peony1:
		return BlockId::Peony2;
	case BlockId::Peony2:
		return BlockId::Peony1;
	case BlockId::RoseBush1:
		return BlockId::RoseBush2;
	case BlockId::RoseBush2:
		return BlockId::RoseBush1;
	}
	//return BlockId();
}

bool BlockDatabase::isDoublePlant(BlockId blockId)
{
	switch (blockId)
	{
	case BlockId::LargeFern1:
	case BlockId::LargeFern2:
	case BlockId::LargeFern:

	case BlockId::Lilac1:
	case BlockId::Lilac2:
	case BlockId::Lilac:

	case BlockId::Peony1:
	case BlockId::Peony2:
	case BlockId::Peony:

	case BlockId::RoseBush1:
	case BlockId::RoseBush2:
	case BlockId::RoseBush:
		return true;
	default:
		return false;
	}
}

bool BlockDatabase::isWoodPlanks(BlockId blockId)
{
	switch (blockId)
	{
	case BlockId::OakPlanks:
	case BlockId::BirchPlanks:
	case BlockId::PalmPlanks:
	case BlockId::SprucePlanks:
		return true;
	default:
		return false;
	}
}

bool BlockDatabase::isSword(BlockId blockId)
{
	switch (blockId)
	{
	case BlockId::DiamondSword:
	case BlockId::GoldSword:
	case BlockId::IronSword:
	case BlockId::StoneSword:
	case BlockId::WoodenSword:
		return true;
	default:
		return false;
	}
}

bool BlockDatabase::isPickaxe(BlockId blockId)
{
	switch (blockId)
	{
	case BlockId::DiamondPickaxe:
	case BlockId::GoldPickaxe:
	case BlockId::IronPickaxe:
	case BlockId::StonePickaxe:
	case BlockId::WoodenPickaxe:
		return true;
	default:
		return false;
	}
}

bool BlockDatabase::isShovel(BlockId blockId)
{
	switch (blockId)
	{
	case BlockId::DiamondShovel:
	case BlockId::GoldShovel:
	case BlockId::IronShovel:
	case BlockId::StoneShovel:
	case BlockId::WoodenShovel:
		return true;
	default:
		return false;
	}
}

bool BlockDatabase::isAxe(BlockId blockId)
{
	switch (blockId)
	{
	case BlockId::DiamondAxe:
	case BlockId::GoldAxe:
	case BlockId::IronAxe:
	case BlockId::StoneAxe:
	case BlockId::WoodenAxe:
		return true;
	default:
		return false;
	}
}

bool BlockDatabase::canPickaxeMine(BlockId toolId, BlockId blockId)
{
	auto &blockData = ChunkBlock(blockId).getData();

	if (toolId == BlockId::DiamondPickaxe || blockData.toolLevelToMine == ToolLevelToMine::Any)
		return true;
	else if (toolId == BlockId::IronPickaxe) {
		if (blockData.toolLevelToMine <= ToolLevelToMine::Iron)
			return true;
	}
	else if (toolId == BlockId::StonePickaxe || toolId == BlockId::GoldPickaxe) {
		if (blockData.toolLevelToMine <= ToolLevelToMine::Stone)
			return true;
	}
	else if (toolId == BlockId::WoodenPickaxe) {
		if (blockData.toolLevelToMine <= ToolLevelToMine::Wood)
			return true;
	}

	return false;
}
