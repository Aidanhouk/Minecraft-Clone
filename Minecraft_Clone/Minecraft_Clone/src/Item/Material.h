#pragma once

#include "../Util/NonCopyable.h"
#include "../World/Block/BlockId.h"

#include <string>

struct Material : public NonCopyable {
    enum ID {
        Nothing,
		Ice,

		Grass,
		Dirt,
		Stone,
		Gravel,
		Clay,
		Bedrock,
		TundraGrass,
		Sand,
		Snow,
		Cobblestone,

		OakBark,
		BirchBark,
		PalmBark,
		SpruceBark,
		OakPlanks,
		BirchPlanks,
		PalmPlanks,
		SprucePlanks,

        OakLeaf,
		PalmLeaf,
		BirchLeaf,
		SpruceLeaf,

		Fern,
		Cactus,
		TallGrass,
		DeadShrub,
		SugarCane,
		Rose,
		Dandelion,

		AzureBluet,
		BlueOrchid,
		CornFlower,
		LilyOfTheValley,
		OrangeTulip,
		OxeyeDaisy,
		PinkTulip,
		RedTulip,
		WhiteTulip,

		LargeFern1,
		LargeFern2,
		LargeFern,
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

		Stick,

		Diamond,

		WoodenPickaxe,
		StonePickaxe,
		IronPickaxe,
		GoldPickaxe,
		DiamondPickaxe,
		WoodenShovel,
		StoneShovel,
		IronShovel,
		GoldShovel,
		DiamondShovel,
		WoodenAxe,
		StoneAxe,
		IronAxe,
		GoldAxe,
		DiamondAxe,

		CraftingTable,
    };

	const static Material NOTHING, ICE,
		GRASS, DIRT, STONE, SAND, BEDROCK, TUNDRA_GRASS, SNOW, GRAVEL, CLAY, COBBLESTONE,
		OAK_BARK, BIRCH_BARK, SPRUCE_BARK, PALM_BARK,
		OAK_PLANKS, BIRCH_PLANKS, PALM_PLANKS, SPRUCE_PLANKS,
		OAK_LEAF, BIRCH_LEAF, SPRUCE_LEAF, PALM_LEAF,
		CACTUS, SUGAR_CANE,
		TALL_GRASS, DEAD_SHRUB, FERN, ROSE, DANDELION,
		AZURE_BLUET, BLUE_ORCHID, CORN_FLOWER, LILY_OF_THE_VALLEY, ORANGLE_TULIP, OXEYE_DAISY, PINK_TULIP, RED_TULIP, WHITE_TULIP,
		LARGE_FERN_1, LARGE_FERN_2,	LARGE_FERN,
		LILAC_1, LILAC_2, LILAC,
		PEONY_1, PEONY_2, PEONY,
		ROSE_BUSH_1, ROSE_BUSH_2, ROSE_BUSH,
		COAL_ORE, IRON_ORE, REDSTONE_ORE, GOLD_ORE, DIAMOND_ORE, EMERALD_ORE,
		APPLE,
		STICK,
		DIAMOND,
		WOODEN_SWORD, WOODEN_PICKAXE, WOODEN_SHOVEL, WOODEN_AXE,
		STONE_SWORD, STONE_PICKAXE, STONE_SHOVEL, STONE_AXE,
		IRON_SWORD, IRON_PICKAXE, IRON_SHOVEL, IRON_AXE,
		GOLD_SWORD, GOLD_PICKAXE, GOLD_SHOVEL, GOLD_AXE,
		DIAMOND_SWORD, DIAMOND_PICKAXE, DIAMOND_SHOVEL, DIAMOND_AXE,
		CRAFTING_TABLE
		;

    Material(Material::ID id, int maxStack, bool isBlock, std::string &&name, int durability = 0);

    BlockId toBlockID() const;

    static const Material &toMaterial(BlockId id);

    const Material::ID id;
    const int maxStackSize;
    const bool isBlock;
    const std::string name;
	const int durability;
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