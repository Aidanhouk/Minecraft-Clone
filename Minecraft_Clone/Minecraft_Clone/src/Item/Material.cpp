#include "Material.h"

const Material Material::NOTHING(ID::Nothing,			0, false, "None");
const Material Material::GRASS_BLOCK(ID::Grass,			64, true, "Grass Block");
const Material Material::DIRT_BLOCK(ID::Dirt,			64, true, "Dirt");
const Material Material::STONE_BLOCK(ID::Stone,			64, true, "Stone");
const Material Material::OAK_BARK_BLOCK(ID::OakBark,	64, true, "Oak Log");
const Material Material::OAK_LEAF_BLOCK(ID::OakLeaf,	64, true, "Leaves");
const Material Material::SAND_BLOCK(ID::Sand,			64, true, "Sand");
const Material Material::CACTUS_BLOCK(ID::Cactus,		64, true, "Cactus");
const Material Material::ROSE(ID::Rose,					64, true, "Rose");
const Material Material::TALL_GRASS(ID::TallGrass,		64, true, "Grass");
const Material Material::DEAD_SHRUB(ID::DeadShrub,		64, true, "Dead Bush");
const Material Material::BIRCH_BARK(ID::BirchBark,		64, true, "Birch Log");
const Material Material::PALM_BARK(ID::PalmBark,		64, true, "Palm Log");
const Material Material::PALM_LEAF(ID::PalmLeaf,		64, true, "Palm Leaves");
const Material Material::BIRCH_LEAF(ID::BirchLeaf,		64, true, "Birch Leaves");
const Material Material::SUGAR_CANE(ID::SugarCane,		64, true, "Sugar Cane");
const Material Material::SNOW(ID::Snow,					64, true, "Snow");
const Material Material::SPRUCE_BARK(ID::SpruceBark,	64, true, "Spruce Log");
const Material Material::SPRUCE_LEAF(ID::SpruceLeaf,	64, true, "Spruce Leaves");
const Material Material::ICE(ID::Ice,					64, true, "Ice");

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

        case OakBark:
            return BlockId::OakBark;

        case OakLeaf:
            return BlockId::OakLeaf;

        case Sand:
            return BlockId::Sand;

        case Cactus:
            return BlockId::Cactus;

        case TallGrass:
            return BlockId::TallGrass;

        case Rose:
            return BlockId::Rose;

		case DeadShrub:
			return BlockId::DeadShrub;

		case BirchBark:
			return BlockId::BirchBark;

		case PalmBark:
			return BlockId::PalmBark;

		case PalmLeaf:
			return BlockId::PalmLeaf;

		case BirchLeaf:
			return BlockId::BirchLeaf;

		case SugarCane:
			return BlockId::SugarCane;

		case Snow:
			return BlockId::Snow;

		case SpruceBark:
			return BlockId::SpruceBark;

		case SpruceLeaf:
			return BlockId::SpruceLeaf;

		case Ice:
			return BlockId::Ice;

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

        case BlockId::OakBark:
            return OAK_BARK_BLOCK;

        case BlockId::OakLeaf:
            return OAK_LEAF_BLOCK;

        case BlockId::Sand:
            return SAND_BLOCK;

        case BlockId::Cactus:
            return CACTUS_BLOCK;

        case BlockId::Rose:
            return ROSE;

        case BlockId::TallGrass:
            return TALL_GRASS;

		case BlockId::DeadShrub:
			return DEAD_SHRUB;

		case BlockId::BirchBark:
			return BIRCH_BARK;

		case BlockId::PalmBark:
			return PALM_BARK;

		case BlockId::PalmLeaf:
			return PALM_LEAF;

		case BlockId::BirchLeaf:
			return BIRCH_LEAF;

		case BlockId::SugarCane:
			return SUGAR_CANE;

		case BlockId::Snow:
			return SNOW;

		case BlockId::SpruceBark:
			return SPRUCE_BARK;

		case BlockId::SpruceLeaf:
			return SPRUCE_LEAF;

		case BlockId::Ice:
			return ICE;

        default:
            return NOTHING;
    }
}
