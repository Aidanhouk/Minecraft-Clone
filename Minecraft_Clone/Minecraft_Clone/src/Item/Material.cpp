#include "Material.h"

const Material Material::NOTHING(ID::Nothing,						0, false, "None");
const Material Material::GRASS_BLOCK(ID::Grass,						64, true, "Grass Block");
const Material Material::DIRT_BLOCK(ID::Dirt,						64, true, "Dirt");
const Material Material::STONE_BLOCK(ID::Stone,						64, true, "Stone");
const Material Material::OAK_BARK_BLOCK(ID::OakBark,				64, true, "Oak Log");
const Material Material::OAK_LEAF_BLOCK(ID::OakLeaf,				64, true, "Leaves");
const Material Material::SAND_BLOCK(ID::Sand,						64, true, "Sand");
const Material Material::CACTUS_BLOCK(ID::Cactus,					64, true, "Cactus");
const Material Material::ROSE(ID::Rose,								64, true, "Rose");
const Material Material::TALL_GRASS(ID::TallGrass,					64, true, "Grass");
const Material Material::DEAD_SHRUB(ID::DeadShrub,					64, true, "Dead Bush");
const Material Material::BIRCH_BARK(ID::BirchBark,					64, true, "Birch Log");
const Material Material::PALM_BARK(ID::PalmBark,					64, true, "Palm Log");
const Material Material::PALM_LEAF(ID::PalmLeaf,					64, true, "Palm Leaves");
const Material Material::BIRCH_LEAF(ID::BirchLeaf,					64, true, "Birch Leaves");
const Material Material::SUGAR_CANE(ID::SugarCane,					64, true, "Sugar Cane");
const Material Material::SNOW(ID::Snow,								64, true, "Snow");
const Material Material::SPRUCE_BARK(ID::SpruceBark,				64, true, "Spruce Log");
const Material Material::SPRUCE_LEAF(ID::SpruceLeaf,				64, true, "Spruce Leaves");
const Material Material::ICE(ID::Ice,								64, true, "Ice");
const Material Material::DANDELION(ID::Dandelion,					64, true, "Dandelion");
const Material Material::BEDROCK(ID::Bedrock,						64, true, "Bedrock");
const Material Material::TUNDRA_GRASS(ID::TundraGrass,				64, true, "Tundra Grass");
const Material Material::FERN(ID::Fern,								64, true, "Fern");
const Material Material::LARGE_FERN_1(ID::LargeFern1,				64, true, "Large Fern 1");
const Material Material::LARGE_FERN_2(ID::LargeFern2,				64, true, "Large Fern 2");
const Material Material::LARGE_FERN(ID::LargeFern,					64, true, "Large Fern");
const Material Material::GRAVEL(ID::Gravel,							64, true, "Gravel");
const Material Material::CLAY(ID::Clay,								64, true, "Clay");
const Material Material::AZURE_BLUET(ID::AzureBluet,				64, true, "Azure Bluet");
const Material Material::BLUE_ORCHID(ID::BlueOrchid,				64, true, "Blue Orchid");
const Material Material::CORN_FLOWER(ID::CornFlower,				64, true, "Corn Flower");
const Material Material::LILY_OF_THE_VALLEY(ID::LilyOfTheValley,	64, true, "Lily of the Valley");
const Material Material::ORANGLE_TULIP(ID::OrangeTulip,				64, true, "Orange Tulip");
const Material Material::OXEYE_DAISY(ID::OxeyeDaisy,				64, true, "Oxeye Daisy");
const Material Material::PINK_TULIP(ID::PinkTulip,					64, true, "Pink Tulip");
const Material Material::RED_TULIP(ID::RedTulip,					64, true, "Red Tulip");
const Material Material::WHITE_TULIP(ID::WhiteTulip,				64, true, "White Tulip");
const Material Material::LILAC_1(ID::Lilac1,						64, true, "Lilac 1");
const Material Material::LILAC_2(ID::Lilac2,						64, true, "Lilac 2");
const Material Material::LILAC(ID::Lilac,							64, true, "Lilac");
const Material Material::PEONY_1(ID::Peony1,						64, true, "Peony 1");
const Material Material::PEONY_2(ID::Peony2,						64, true, "Peony 2");
const Material Material::PEONY(ID::Peony,							64, true, "Peony");
const Material Material::ROSE_BUSH_1(ID::RoseBush1,					64, true, "Rose Bush 1");
const Material Material::ROSE_BUSH_2(ID::RoseBush2,					64, true, "Rose Bush 2");
const Material Material::ROSE_BUSH(ID::RoseBush,					64, true, "Rose Bush");
const Material Material::COAL_ORE(ID::CoalOre,						64, true, "Coal Ore");
const Material Material::IRON_ORE(ID::IronOre,						64, true, "Iron Ore");
const Material Material::REDSTONE_ORE(ID::RedstoneOre,				64, true, "Redstone Ore");
const Material Material::GOLD_ORE(ID::GoldOre,						64, true, "Gold Ore");
const Material Material::DIAMOND_ORE(ID::DiamondOre,				64, true, "Diamond Ore");
const Material Material::EMERALD_ORE(ID::EmeraldOre,				64, true, "Emerald Ore");
const Material Material::WOODEN_SWORD(ID::WoodenSword,				1, false, "Wooden Sword");
const Material Material::STONE_SWORD(ID::StoneSword,				1, false, "Stone Sword");
const Material Material::IRON_SWORD(ID::IronSword,					1, false, "Iron Sword");
const Material Material::GOLD_SWORD(ID::GoldSword,					1, false, "Gold Sword");
const Material Material::DIAMOND_SWORD(ID::DiamondSword,			1, false, "Diamond Sword");
const Material Material::APPLE(ID::Apple,							64, false, "Apple");

Material::Material(Material::ID id, int maxStack, bool isBlock, std::string &&name)
    : id(id)
    , maxStackSize(maxStack)
    , isBlock(isBlock)
    , name(std::move(name))
{
}

BlockId Material::toBlockID() const
{
    switch (id) {
        case Nothing:
            return BlockId::Air;

        case Grass:
            return BlockId::Grass;
        case Dirt:
            return BlockId::Dirt;
        case Stone:
            return BlockId::Stone;
		case Sand:
			return BlockId::Sand;
		case Ice:
			return BlockId::Ice;
		case Bedrock:
			return BlockId::Bedrock;
		case TundraGrass:
			return BlockId::TundraGrass;
		case Snow:
			return BlockId::Snow;
		case Gravel:
			return BlockId::Gravel;
		case Clay:
			return BlockId::Clay;

        case OakBark:
            return BlockId::OakBark;
		case BirchBark:
			return BlockId::BirchBark;
		case PalmBark:
			return BlockId::PalmBark;
		case SpruceBark:
			return BlockId::SpruceBark;

		case OakLeaf:
			return BlockId::OakLeaf;
		case PalmLeaf:
			return BlockId::PalmLeaf;
		case BirchLeaf:
			return BlockId::BirchLeaf;
		case SpruceLeaf:
			return BlockId::SpruceLeaf;

		case Cactus:
			return BlockId::Cactus;
		case SugarCane:
			return BlockId::SugarCane;

		case TallGrass:
			return BlockId::TallGrass;
		case DeadShrub:
			return BlockId::DeadShrub;
		case Fern:
			return BlockId::Fern;
		case Rose:
			return BlockId::Rose;
		case Dandelion:
			return BlockId::Dandelion;
		case AzureBluet:
			return BlockId::AzureBluet;
		case BlueOrchid:
			return BlockId::BlueOrchid;
		case CornFlower:
			return BlockId::CornFlower;
		case LilyOfTheValley:
			return BlockId::LilyOfTheValley;
		case OrangeTulip:
			return BlockId::OrangeTulip;
		case OxeyeDaisy:
			return BlockId::OxeyeDaisy;
		case PinkTulip:
			return BlockId::PinkTulip;
		case RedTulip:
			return BlockId::RedTulip;
		case WhiteTulip:
			return BlockId::WhiteTulip;

		case LargeFern1:
			return BlockId::LargeFern1;
		case LargeFern2:
			return BlockId::LargeFern2;
		case LargeFern:
			return BlockId::LargeFern;
		case Lilac1:
			return BlockId::Lilac1;
		case Lilac2:
			return BlockId::Lilac2;
		case Lilac:
			return BlockId::Lilac;
		case Peony1:
			return BlockId::Peony1;
		case Peony2:
			return BlockId::Peony2;
		case Peony:
			return BlockId::Peony;
		case RoseBush1:
			return BlockId::RoseBush1;
		case RoseBush2:
			return BlockId::RoseBush2;
		case RoseBush:
			return BlockId::RoseBush;

		case CoalOre:
			return BlockId::CoalOre;
		case IronOre:
			return BlockId::IronOre;
		case RedstoneOre:
			return BlockId::RedstoneOre;
		case GoldOre:
			return BlockId::GoldOre;
		case DiamondOre:
			return BlockId::DiamondOre;
		case EmeraldOre:
			return BlockId::EmeraldOre;

		case WoodenSword:
			return BlockId::WoodenSword;
		case StoneSword:
			return BlockId::StoneSword;
		case IronSword:
			return BlockId::IronSword;
		case GoldSword:
			return BlockId::GoldSword;
		case DiamondSword:
			return BlockId::DiamondSword;

		case Apple:
			return BlockId::Apple;

        default:
            return BlockId::NUM_TYPES;
    }
}

const Material &Material::toMaterial(BlockId id)
{
    switch (id) {
        case BlockId::Grass:
            return GRASS_BLOCK;
        case BlockId::Dirt:
            return DIRT_BLOCK;
        case BlockId::Stone:
            return STONE_BLOCK;
		case BlockId::Sand:
			return SAND_BLOCK;
		case BlockId::Ice:
			return ICE;
		case BlockId::Bedrock:
			return BEDROCK;
		case BlockId::TundraGrass:
			return TUNDRA_GRASS;
		case BlockId::Snow:
			return SNOW;
		case BlockId::Gravel:
			return GRAVEL;
		case BlockId::Clay:
			return CLAY;

        case BlockId::OakBark:
            return OAK_BARK_BLOCK;
		case BlockId::BirchBark:
			return BIRCH_BARK;
		case BlockId::PalmBark:
			return PALM_BARK;
		case BlockId::SpruceBark:
			return SPRUCE_BARK;

        case BlockId::OakLeaf:
            return OAK_LEAF_BLOCK;
		case BlockId::PalmLeaf:
			return PALM_LEAF;
		case BlockId::BirchLeaf:
			return BIRCH_LEAF;
		case BlockId::SpruceLeaf:
			return SPRUCE_LEAF;

		case BlockId::Cactus:
			return CACTUS_BLOCK;
		case BlockId::SugarCane:
			return SUGAR_CANE;
		case BlockId::TallGrass:
			return TALL_GRASS;
		case BlockId::DeadShrub:
			return DEAD_SHRUB;
		case BlockId::Fern:
			return FERN;
		case BlockId::Rose:
			return ROSE;
		case BlockId::Dandelion:
			return DANDELION;

		case BlockId::AzureBluet:
			return AZURE_BLUET;
		case BlockId::BlueOrchid:
			return BLUE_ORCHID;
		case BlockId::CornFlower:
			return CORN_FLOWER;
		case BlockId::LilyOfTheValley:
			return LILY_OF_THE_VALLEY;
		case BlockId::OrangeTulip:
			return ORANGLE_TULIP;
		case BlockId::OxeyeDaisy:
			return OXEYE_DAISY;
		case BlockId::PinkTulip:
			return PINK_TULIP;
		case BlockId::RedTulip:
			return RED_TULIP;
		case BlockId::WhiteTulip:
			return WHITE_TULIP;

		case BlockId::LargeFern1:
			return LARGE_FERN_1;
		case BlockId::LargeFern2:
			return LARGE_FERN_2;
		case BlockId::LargeFern:
			return LARGE_FERN;
		case BlockId::Lilac1:
			return LILAC_1;
		case BlockId::Lilac2:
			return LILAC_2;
		case BlockId::Lilac:
			return LILAC;
		case BlockId::Peony1:
			return PEONY_1;
		case BlockId::Peony2:
			return PEONY_2;
		case BlockId::Peony:
			return PEONY;
		case BlockId::RoseBush1:
			return ROSE_BUSH_1;
		case BlockId::RoseBush2:
			return ROSE_BUSH_2;
		case BlockId::RoseBush:
			return ROSE_BUSH;

		case BlockId::CoalOre:
			return COAL_ORE;
		case BlockId::IronOre:
			return IRON_ORE;
		case BlockId::RedstoneOre:
			return REDSTONE_ORE;
		case BlockId::GoldOre:
			return GOLD_ORE;
		case BlockId::DiamondOre:
			return DIAMOND_ORE;
		case BlockId::EmeraldOre:
			return EMERALD_ORE;

		case BlockId::WoodenSword:
			return WOODEN_SWORD;
		case BlockId::StoneSword:
			return STONE_SWORD;
		case BlockId::IronSword:
			return IRON_SWORD;
		case BlockId::GoldSword:
			return GOLD_SWORD;
		case BlockId::DiamondSword:
			return DIAMOND_SWORD;

		case BlockId::Apple:
			return APPLE;

        default:
            return NOTHING;
    }
}
