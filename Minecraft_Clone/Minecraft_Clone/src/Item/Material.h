#pragma once

#include "../World/Block/BlockId.h"
#include <string>

#include "../Util/NonCopyable.h"

struct Material : public NonCopyable {
    enum ID {
        Nothing,
        Grass,
        Dirt,
        Stone,
        OakBark,
        OakLeaf,
        Sand,
        Cactus,
        Rose,
        TallGrass,
        DeadShrub,
		BirchBark,
		PalmBark,
		PalmLeaf,
		BirchLeaf,
		SugarCane,
		Snow,
		SpruceBark,
		SpruceLeaf,
		Ice,
		Dandelion,
		Bedrock,
		TundraGrass,
		Fern,
		LargeFern1,
		LargeFern2,
		LargeFern,
		Gravel,
		Clay,
		AzureBluet,
		BlueOrchid,
		CornFlower,
		LilyOfTheValley,
		OrangeTulip,
		OxeyeDaisy,
		PinkTulip,
		RedTulip,
		WhiteTulip,
		Lilac1,
		Lilac2,
		Lilac,
		Peony1,
		Peony2,
		Peony,
		RoseBush1,
		RoseBush2,
		RoseBush,
		CoalOre,
		IronOre,
		RedstoneOre,
		GoldOre,
		DiamondOre,
		EmeraldOre,
		WoodenSword,
		StoneSword,
		IronSword,
		GoldSword,
		DiamondSword,
		Apple,
    };

	const static Material NOTHING, GRASS_BLOCK, DIRT_BLOCK, STONE_BLOCK,
		OAK_BARK_BLOCK, OAK_LEAF_BLOCK, SAND_BLOCK, CACTUS_BLOCK, ROSE,
		TALL_GRASS, DEAD_SHRUB, BIRCH_BARK, PALM_BARK, PALM_LEAF, BIRCH_LEAF,
		SUGAR_CANE, SNOW, SPRUCE_BARK, SPRUCE_LEAF, ICE, DANDELION, BEDROCK,
		TUNDRA_GRASS, FERN, LARGE_FERN_1, LARGE_FERN_2, LARGE_FERN, GRAVEL,
		CLAY, AZURE_BLUET, BLUE_ORCHID,	CORN_FLOWER, LILY_OF_THE_VALLEY,
		ORANGLE_TULIP, OXEYE_DAISY, PINK_TULIP, RED_TULIP, WHITE_TULIP,
		LILAC_1, LILAC_2, LILAC, PEONY_1, PEONY_2, PEONY, ROSE_BUSH_1,
		ROSE_BUSH_2, ROSE_BUSH, COAL_ORE, IRON_ORE, REDSTONE_ORE, GOLD_ORE,
		DIAMOND_ORE, EMERALD_ORE, WOODEN_SWORD, STONE_SWORD, IRON_SWORD,
		GOLD_SWORD, DIAMOND_SWORD, APPLE;

    Material(Material::ID id, int maxStack, bool isBlock, std::string &&name);

    BlockId toBlockID() const;

    static const Material &toMaterial(BlockId id);

    const Material::ID id;
    const int maxStackSize;
    const bool isBlock;
    const std::string name;
};

namespace std {
template <> struct hash<Material::ID> {
    size_t operator()(const Material::ID &id) const
    {
        std::hash<Material::ID> hasher;

        return hasher(id);
    }
};
} // namespace std