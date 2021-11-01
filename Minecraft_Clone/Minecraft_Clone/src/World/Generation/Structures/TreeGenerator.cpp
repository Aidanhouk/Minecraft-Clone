#include "TreeGenerator.h"

#include "../../Chunk/Chunk.h"
#include "World/Block/BlockDatabase.h"
#include "StructureBuilder.h"

constexpr BlockId OAKLEAF = BlockId::OakLeaf;
constexpr BlockId BIRCHLEAF = BlockId::BirchLeaf;
constexpr BlockId SPRUCELEAF = BlockId::SpruceLeaf;
constexpr BlockId PALMLEAF = BlockId::PalmLeaf;

namespace {
	void makeCross1(StructureBuilder &builder, int y, const BlockId &blockId, int x, int z)
	{
		builder.fill(y, x - 1, x + 2, z - 0, z + 1,
			blockId);
		builder.fill(y, x - 0, x + 1, z - 1, z + 2,
			blockId);
	}
	void makeCross2(StructureBuilder &builder, int y, const BlockId &blockId, int x, int z)
	{
		builder.fill(y, x - 2, x + 3, z - 1, z + 2,
			blockId);
		builder.fill(y, x - 1, x + 2, z - 2, z + 3,
			blockId);
	}
	void makeCross3(StructureBuilder &builder, int y, const BlockId &blockId, int x, int z)
	{
		builder.fill(y, x - 3, x + 4, z - 2, z + 3,
			blockId);
		builder.fill(y, x - 2, x + 3, z - 3, z + 4,
			blockId);
	}
} // namespace

namespace {

void makeSpruceTree1(Chunk & chunk, int x, int y, int z)
{
	StructureBuilder builder;

	int h = 8;
	int newY = h + y;

	makeCross2(builder, newY - 6, SPRUCELEAF, x, z);
	makeCross1(builder, newY - 5, SPRUCELEAF, x, z);
	makeCross2(builder, newY - 4, SPRUCELEAF, x, z);
	makeCross1(builder, newY - 3, SPRUCELEAF, x, z);
	makeCross2(builder, newY - 2, SPRUCELEAF, x, z);
	makeCross1(builder, newY - 1, SPRUCELEAF, x, z);
	builder.addBlock(x, newY, z, SPRUCELEAF);
	makeCross1(builder, newY + 1, SPRUCELEAF, x, z);

	builder.makeColumn(x, z, y, h, BlockId::SpruceBark);
	builder.build(chunk);
}

void makeSpruceTree2(Chunk & chunk, int x, int y, int z)
{
	StructureBuilder builder;

	int h = 6;
	int newY = h + y;

	makeCross1(builder, newY - 4, SPRUCELEAF, x, z);
	makeCross2(builder, newY - 3, SPRUCELEAF, x, z);
	makeCross1(builder, newY - 2, SPRUCELEAF, x, z);
	makeCross1(builder, newY, SPRUCELEAF, x, z);

	builder.makeColumn(x, z, y, h, BlockId::SpruceBark);
	builder.build(chunk);
}

void makeSpruceTree3(Chunk & chunk, int x, int y, int z)
{
	StructureBuilder builder;

	int h = 6;
	int newY = h + y;

	makeCross3(builder, newY - 5, SPRUCELEAF, x, z);
	makeCross2(builder, newY - 4, SPRUCELEAF, x, z);
	makeCross1(builder, newY - 3, SPRUCELEAF, x, z);
	makeCross2(builder, newY - 2, SPRUCELEAF, x, z);
	makeCross1(builder, newY - 1, SPRUCELEAF, x, z);
	builder.addBlock(x, newY, z, SPRUCELEAF);
	makeCross1(builder, newY + 1, SPRUCELEAF, x, z);

	builder.makeColumn(x, z, y, h, BlockId::SpruceBark);
	builder.build(chunk);
}

void makeSpruceTree4(Chunk & chunk, Random<std::minstd_rand>& rand, int x, int y, int z)
{
	StructureBuilder builder;

	int h = rand.intInRange(6, 10);
	int newY = h + y;

	makeCross1(builder, newY - 2, SPRUCELEAF, x, z);
	makeCross2(builder, newY - 1, SPRUCELEAF, x, z);
	makeCross1(builder, newY, SPRUCELEAF, x, z);
	builder.addBlock(x, newY + 1, z, SPRUCELEAF);

	builder.makeColumn(x, z, y, h, BlockId::SpruceBark);
	builder.build(chunk);
}

void makeSpruceTree5(Chunk & chunk, Random<std::minstd_rand>& rand, int x, int y, int z)
{
	StructureBuilder builder;

	int h = rand.intInRange(7, 9);
	int newY = h + y;

	makeCross1(builder, newY - 2, SPRUCELEAF, x, z);
	makeCross1(builder, newY - 1, SPRUCELEAF, x, z);
	makeCross1(builder, newY, SPRUCELEAF, x, z);
	builder.addBlock(x, newY + 1, z, SPRUCELEAF);

	builder.makeColumn(x, z, y, h, BlockId::SpruceBark);
	builder.build(chunk);
}

void makeSpruceTree6(Chunk & chunk, int x, int y, int z)
{
	StructureBuilder builder;

	int h = 6;

	int newY = h + y;

	makeCross1(builder, newY - 5, SPRUCELEAF, x, z);
	makeCross2(builder, newY - 4, SPRUCELEAF, x, z);
	makeCross1(builder, newY - 3, SPRUCELEAF, x, z);
	makeCross1(builder, newY - 1, SPRUCELEAF, x, z);
	builder.addBlock(x, newY, z, SPRUCELEAF);

	builder.makeColumn(x, z, y, h, BlockId::SpruceBark);
	builder.build(chunk);
}

void makeSpruceTree7(Chunk & chunk, int x, int y, int z)
{
	StructureBuilder builder;

	int h = 8;
	int newY = h + y;

	makeCross1(builder, newY - 6, SPRUCELEAF, x, z);
	makeCross3(builder, newY - 5, SPRUCELEAF, x, z);
	makeCross2(builder, newY - 4, SPRUCELEAF, x, z);
	makeCross1(builder, newY - 3, SPRUCELEAF, x, z);
	makeCross2(builder, newY - 2, SPRUCELEAF, x, z);
	makeCross1(builder, newY - 1, SPRUCELEAF, x, z);
	builder.addBlock(x, newY, z, SPRUCELEAF);
	makeCross1(builder, newY + 1, SPRUCELEAF, x, z);

	builder.makeColumn(x, z, y, h, BlockId::SpruceBark);
	builder.build(chunk);
}

} // namespace

namespace {
	bool outOfBounds(int x, int z)
	{
		return (x < 0 || x >= CHUNK_SIZE || z < 0 || z >= CHUNK_SIZE);
	}
}

void makeOakTree(Chunk &chunk, Random<std::minstd_rand> &rand, int x, int y, int z)
{
	StructureBuilder builder;

	int h = rand.intInRange(1, 10);
	if (h == 1)
		h = 4;
	else if (h <= 4)
		h = 5;
	else if (h <= 7)
		h = 6;
	else if (h <= 10)
		h = 7;

	int newY = h + y;

	builder.fill(newY - 3, x - 2, x + 3, z - 2, z + 3,
		OAKLEAF);
	builder.fill(newY - 2, x - 2, x + 3, z - 2, z + 3,
		OAKLEAF);

	for (int zLeaf = -1; zLeaf <= 1; ++zLeaf) {
		builder.addBlock(x, newY - 1, z + zLeaf, OAKLEAF);
		builder.addBlock(x, newY, z + zLeaf, OAKLEAF);
	}
	for (int xLeaf = -1; xLeaf <= 1; ++xLeaf) {
		builder.addBlock(x + xLeaf, newY - 1, z, OAKLEAF);
		builder.addBlock(x + xLeaf, newY, z, OAKLEAF);
	}

	if (rand.intInRange(0, 2) > 0) {
		builder.addBlock(x - 1, newY - 1, z - 1, OAKLEAF);
		builder.addBlock(x - 1, newY - 1, z + 1, OAKLEAF);
		builder.addBlock(x + 1, newY - 1, z - 1, OAKLEAF);
		builder.addBlock(x + 1, newY - 1, z + 1, OAKLEAF);
	}

	builder.makeColumn(x, z, y, h, BlockId::OakBark);
	builder.build(chunk);
}

void makeBirchTree(Chunk &chunk, Random<std::minstd_rand> &rand, int x, int y, int z)
{
	StructureBuilder builder;

	int h = rand.intInRange(6, 7);

	int newY = h + y;

	builder.fill(newY - 4, x - 2, x + 3, z - 2, z + 3,
		BIRCHLEAF);
	builder.fill(newY - 3, x - 2, x + 3, z - 2, z + 3,
		BIRCHLEAF);
	builder.fill(newY - 2, x - 1, x + 2, z - 1, z + 2,
		BIRCHLEAF);

	for (int zLeaf = -1; zLeaf <= 1; ++zLeaf) {
		builder.addBlock(x, newY - 1, z + zLeaf, BIRCHLEAF);
		builder.addBlock(x, newY, z + zLeaf, BIRCHLEAF);
	}
	for (int xLeaf = -1; xLeaf <= 1; ++xLeaf) {
		builder.addBlock(x + xLeaf, newY - 1, z, BIRCHLEAF);
		builder.addBlock(x + xLeaf, newY, z, BIRCHLEAF);
	}

	if (rand.intInRange(0, 2) > 0) {
		builder.addBlock(x - 1, newY - 1, z - 1, BIRCHLEAF);
		builder.addBlock(x - 1, newY - 1, z + 1, BIRCHLEAF);
		builder.addBlock(x + 1, newY - 1, z - 1, BIRCHLEAF);
		builder.addBlock(x + 1, newY - 1, z + 1, BIRCHLEAF);
	}

	builder.makeColumn(x, z, y, h - 1, BlockId::BirchBark);
	builder.build(chunk);
}

void makePalmTree(Chunk &chunk, Random<std::minstd_rand> &rand, int x, int y, int z)
{
    StructureBuilder builder;

    int height = rand.intInRange(6, 8);

	builder.addBlock(x, y + height + 1, z, PALMLEAF);

    for (int xLeaf = -3; xLeaf < 4; xLeaf++) {
        builder.addBlock(xLeaf + x, y + height, z, PALMLEAF);
    }
    for (int zLeaf = -3; zLeaf < 4; zLeaf++) {
        builder.addBlock(x, y + height, zLeaf + z, PALMLEAF);
    }

    builder.addBlock(x, y + height - 1, z + 4, PALMLEAF);
    builder.addBlock(x, y + height - 1, z - 4, PALMLEAF);
    builder.addBlock(x + 4, y + height - 1, z, PALMLEAF);
	builder.addBlock(x - 4, y + height - 1, z, PALMLEAF);

	builder.addBlock(x + 1, y + height, z + 1, PALMLEAF);
	builder.addBlock(x + 2, y + height, z + 2, PALMLEAF);
	builder.addBlock(x + 3, y + height - 1, z + 3, PALMLEAF);

	builder.addBlock(x + 1, y + height, z - 1, PALMLEAF);
	builder.addBlock(x + 2, y + height, z - 2, PALMLEAF);
	builder.addBlock(x + 3, y + height - 1, z - 3, PALMLEAF);

	builder.addBlock(x - 1, y + height, z + 1, PALMLEAF);
	builder.addBlock(x - 2, y + height, z + 2, PALMLEAF);
	builder.addBlock(x - 3, y + height - 1, z + 3, PALMLEAF);

	builder.addBlock(x - 1, y + height, z - 1, PALMLEAF);
	builder.addBlock(x - 2, y + height, z - 2, PALMLEAF);
	builder.addBlock(x - 3, y + height - 1, z - 3, PALMLEAF);

    builder.makeColumn(x, z, y, height + 1, BlockId::PalmBark);
    builder.build(chunk);
}

void makeSpruceTree(Chunk & chunk, Random<std::minstd_rand>& rand, int x, int y, int z)
{
	int spruce = rand.intInRange(0, 6);

	switch (spruce) {
	case 0:
		makeSpruceTree1(chunk, x, y, z);
		break;
	case 1:
		makeSpruceTree2(chunk, x, y, z);
		break;
	case 2:
		makeSpruceTree3(chunk, x, y, z);
		break;
	case 3:
		makeSpruceTree4(chunk, rand, x, y, z);
		break;
	case 4:
		makeSpruceTree5(chunk, rand, x, y, z);
		break;
	case 5:
		makeSpruceTree6(chunk, x, y, z);
		break;
	case 6:
		makeSpruceTree7(chunk, x, y, z);
		break;
	}
}

void makeCactus(Chunk &chunk, Random<std::minstd_rand> &rand, int x, int y, int z)
{
	if (!BlockDatabase::canPlaceOnBlock(BlockId::Cactus, chunk.getBlock(x, y - 1, z).getData().id))
		return;

	StructureBuilder builder;
	if (chunk.getBlock(x + 1, y, z) == 0 &&
		chunk.getBlock(x - 1, y, z) == 0 &&
		chunk.getBlock(x, y, z + 1) == 0 &&
		chunk.getBlock(x, y, z - 1) == 0)
	{
		builder.makeColumn(x, z, y, rand.intInRange(2, 5), BlockId::Cactus);
		builder.build(chunk);
	}
}

void makeSugarCane(Chunk &chunk, Random<std::minstd_rand> &rand, int x, int z,
	Array2D<int, CHUNK_SIZE> &heightMap)
{
	StructureBuilder builder;

	for (int xx = x - 1; xx <= x + 1; ++xx)
		for (int zz = z - 1; zz <= z + 1; ++zz) {
			if (outOfBounds(xx, zz))
				continue;

			int y = heightMap.get(xx, zz);

			if (outOfBounds(xx, zz) ||
				y != WATER_LEVEL ||
				rand.intInRange(0, 3) == 0)
				continue;
			if (!BlockDatabase::canPlaceOnBlock(BlockId::SugarCane, chunk.getBlock(x, y - 1, z).getData().id))
				continue;

			int height = rand.intInRange(2, 4);
			bool collidesOtherBlock = false;
			for (int i = 1; i <= height; ++i)
				if (chunk.getBlock(xx, y + i, zz) != 0) {
					collidesOtherBlock = true;
					break;
				}
			if (collidesOtherBlock)
				continue;

			bool nearWater = false;
			for (int i = xx - 1; i <= xx + 1; ++i)
				for (int j = zz - 1; j <= zz + 1; ++j) {
					if (!nearWater && !outOfBounds(i, j) && heightMap.get(i, j) < WATER_LEVEL) {
						builder.makeColumn(xx, zz, WATER_LEVEL + 1, height, BlockId::SugarCane);
						nearWater = true;
					}
				}
		}

	builder.build(chunk);
}

void makeDoublePlant(Chunk &chunk, int x, int y, int z, BlockId id)
{
	BlockId id1, id2;

	switch (id)
	{
	case BlockId::LargeFern:
	case BlockId::LargeFern1:
	case BlockId::LargeFern2:
		id1 = BlockId::LargeFern1;
		id2 = BlockId::LargeFern2;
		break;
	case BlockId::Lilac:
	case BlockId::Lilac1:
	case BlockId::Lilac2:
		id1 = BlockId::Lilac1;
		id2 = BlockId::Lilac2;
		break;
	case BlockId::Peony:
	case BlockId::Peony1:
	case BlockId::Peony2:
		id1 = BlockId::Peony1;
		id2 = BlockId::Peony2;
		break;
	case BlockId::RoseBush:
	case BlockId::RoseBush1:
	case BlockId::RoseBush2:
		id1 = BlockId::RoseBush1;
		id2 = BlockId::RoseBush2;
		break;
	default:
		return;
	}

	StructureBuilder builder;
	builder.addBlock(x, y, z, id1);
	builder.addBlock(x, y + 1, z, id2);
	builder.build(chunk);
}