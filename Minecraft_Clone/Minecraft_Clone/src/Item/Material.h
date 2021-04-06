#ifndef MATERIAL_H_INCLUDED
#define MATERIAL_H_INCLUDED

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
    };

	const static Material NOTHING, GRASS_BLOCK, DIRT_BLOCK, STONE_BLOCK,
		OAK_BARK_BLOCK, OAK_LEAF_BLOCK, SAND_BLOCK, CACTUS_BLOCK, ROSE,
		TALL_GRASS, DEAD_SHRUB, BIRCH_BARK, PALM_BARK, PALM_LEAF, BIRCH_LEAF,
		SUGAR_CANE, SNOW, SPRUCE_BARK, SPRUCE_LEAF, ICE;

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

#endif // MATERIAL_H_INCLUDED
