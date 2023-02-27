#include "BlockUtils.h"

#include "../ChunkBlock.h"
#include "../BlockTypes/BlockType.h"

bool BlockUtils::canPlaceOnBlock(BlockId blockId, BlockId placeOnThisBlockId)
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

BlockId BlockUtils::getRandomFlower(Rand& rand)
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

BlockId BlockUtils::getRandomDoubleFlower(Rand& rand)
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

BlockId BlockUtils::getDoublePlantMain(BlockId blockId)
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
    default:
        return BlockId();
    }
}

BlockId BlockUtils::getDoublePlantSecondPart(BlockId blockId)
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
    default:
        return BlockId();
    }
}

bool BlockUtils::isLiquid(BlockId blockId)
{
    switch (blockId)
    {
    case BlockId::Water:
        // case BlockId::Lava:
        return true;
    default:
        return false;
    }
}

bool BlockUtils::isDoublePlant(BlockId blockId)
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

bool BlockUtils::isWoodPlanks(BlockId blockId)
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

bool BlockUtils::isSword(BlockId blockId)
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

bool BlockUtils::isPickaxe(BlockId blockId)
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

bool BlockUtils::isShovel(BlockId blockId)
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

bool BlockUtils::isAxe(BlockId blockId)
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

bool BlockUtils::canPickaxeMine(BlockId toolId, BlockId blockId)
{
    auto& blockData = ChunkBlock(blockId).getData();

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