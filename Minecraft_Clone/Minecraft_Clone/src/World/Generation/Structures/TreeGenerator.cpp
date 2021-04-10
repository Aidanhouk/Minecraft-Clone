#include "TreeGenerator.h"

#include "../../Chunk/Chunk.h"
#include "StructureBuilder.h"

constexpr BlockId OAKLEAF = BlockId::OakLeaf;
constexpr BlockId BIRCHLEAF = BlockId::BirchLeaf;
constexpr BlockId SPRUCELEAF = BlockId::SpruceLeaf;
constexpr BlockId PALMLEAF = BlockId::PalmLeaf;

namespace {

void makeSpruceTree1(Chunk & chunk, Random<std::minstd_rand>& rand, int x, int y, int z)
{
	StructureBuilder builder;

	int h = 8;

	int newY = h + y;

	builder.fill(newY - 6, x - 2, x + 3, z - 1, z + 2,
		SPRUCELEAF);
	builder.fill(newY - 6, x - 1, x + 2, z - 2, z + 3,
		SPRUCELEAF);

	builder.fill(newY - 5, x - 1, x + 2, z - 0, z + 1,
		SPRUCELEAF);
	builder.fill(newY - 5, x - 0, x + 1, z - 1, z + 2,
		SPRUCELEAF);

	builder.fill(newY - 4, x - 2, x + 3, z - 1, z + 2,
		SPRUCELEAF);
	builder.fill(newY - 4, x - 1, x + 2, z - 2, z + 3,
		SPRUCELEAF);

	builder.fill(newY - 3, x - 1, x + 2, z - 0, z + 1,
		SPRUCELEAF);
	builder.fill(newY - 3, x - 0, x + 1, z - 1, z + 2,
		SPRUCELEAF);

	builder.fill(newY - 2, x - 2, x + 3, z - 1, z + 2,
		SPRUCELEAF);
	builder.fill(newY - 2, x - 1, x + 2, z - 2, z + 3,
		SPRUCELEAF);

	builder.fill(newY - 1, x - 1, x + 2, z - 0, z + 1,
		SPRUCELEAF);
	builder.fill(newY - 1, x - 0, x + 1, z - 1, z + 2,
		SPRUCELEAF);

	builder.addBlock(x, newY, z, SPRUCELEAF);

	builder.fill(newY + 1, x - 1, x + 2, z - 0, z + 1,
		SPRUCELEAF);
	builder.fill(newY + 1, x - 0, x + 1, z - 1, z + 2,
		SPRUCELEAF);

	builder.makeColumn(x, z, y, h, BlockId::SpruceBark);
	builder.build(chunk);
}

void makeSpruceTree2(Chunk & chunk, Random<std::minstd_rand>& rand, int x, int y, int z)
{
	StructureBuilder builder;

	int h = 7;

	int newY = h + y;

	builder.fill(newY - 5, x - 1, x + 2, z - 0, z + 1,
		SPRUCELEAF);
	builder.fill(newY - 5, x - 0, x + 1, z - 1, z + 2,
		SPRUCELEAF);

	builder.fill(newY - 4, x - 2, x + 3, z - 1, z + 2,
		SPRUCELEAF);
	builder.fill(newY - 4, x - 1, x + 2, z - 2, z + 3,
		SPRUCELEAF);

	builder.fill(newY - 3, x - 1, x + 2, z - 0, z + 1,
		SPRUCELEAF);
	builder.fill(newY - 3, x - 0, x + 1, z - 1, z + 2,
		SPRUCELEAF);

	builder.fill(newY - 1, x - 1, x + 2, z - 0, z + 1,
		SPRUCELEAF);
	builder.fill(newY - 1, x - 0, x + 1, z - 1, z + 2,
		SPRUCELEAF);

	builder.addBlock(x, newY, z, SPRUCELEAF);

	builder.makeColumn(x, z, y, h, BlockId::SpruceBark);
	builder.build(chunk);
}

void makeSpruceTree3(Chunk & chunk, Random<std::minstd_rand>& rand, int x, int y, int z)
{
	StructureBuilder builder;

	int h = 6;

	int newY = h + y;

	builder.fill(newY - 5, x - 3, x + 4, z - 2, z + 3,
		SPRUCELEAF);
	builder.fill(newY - 5, x - 2, x + 3, z - 3, z + 4,
		SPRUCELEAF);

	builder.fill(newY - 4, x - 2, x + 3, z - 1, z + 2,
		SPRUCELEAF);
	builder.fill(newY - 4, x - 1, x + 2, z - 2, z + 3,
		SPRUCELEAF);

	builder.fill(newY - 3, x - 1, x + 2, z - 0, z + 1,
		SPRUCELEAF);
	builder.fill(newY - 3, x - 0, x + 1, z - 1, z + 2,
		SPRUCELEAF);

	builder.fill(newY - 2, x - 2, x + 3, z - 1, z + 2,
		SPRUCELEAF);
	builder.fill(newY - 2, x - 1, x + 2, z - 2, z + 3,
		SPRUCELEAF);

	builder.fill(newY - 1, x - 1, x + 2, z - 0, z + 1,
		SPRUCELEAF);
	builder.fill(newY - 1, x - 0, x + 1, z - 1, z + 2,
		SPRUCELEAF);

	builder.addBlock(x, newY, z, SPRUCELEAF);
	builder.fill(newY + 1, x - 1, x + 2, z - 0, z + 1,
		SPRUCELEAF);
	builder.fill(newY + 1, x - 0, x + 1, z - 1, z + 2,
		SPRUCELEAF);

	builder.makeColumn(x, z, y, h, BlockId::SpruceBark);
	builder.build(chunk);
}

} // namespace

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
	builder.fill(newY - 1, x - 1, x + 2, z - 1, z + 2,
		OAKLEAF);

	for (int32_t zLeaf = -1; zLeaf <= 1; ++zLeaf) {
		builder.addBlock(x, newY, z + zLeaf, OAKLEAF);
	}
	for (int32_t xLeaf = -1; xLeaf <= 1; ++xLeaf) {
		builder.addBlock(x + xLeaf, newY, z, OAKLEAF);
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
	builder.fill(newY - 1, x - 1, x + 2, z - 1, z + 2,
		BIRCHLEAF);

	for (int32_t zLeaf = -1; zLeaf <= 1; ++zLeaf) {
		builder.addBlock(x, newY, z + zLeaf, BIRCHLEAF);
	}
	for (int32_t xLeaf = -1; xLeaf <= 1; ++xLeaf) {
		builder.addBlock(x + xLeaf, newY, z, BIRCHLEAF);
	}

	builder.makeColumn(x, z, y - 2, h, BlockId::BirchBark);
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
	int spruce = rand.intInRange(0, 2);

	switch (spruce) {
	case 0:
		makeSpruceTree1(chunk, rand, x, y, z);
		break;

	case 1:
		makeSpruceTree2(chunk, rand, x, y, z);
		break;

	case 2:
		makeSpruceTree3(chunk, rand, x, y, z);
	}
}

void makeCactus(Chunk &chunk, Random<std::minstd_rand> &rand, int x, int y, int z)
{
	StructureBuilder builder;
	builder.makeColumn(x, z, y, rand.intInRange(3, 5), BlockId::Cactus);
	builder.build(chunk);
}

void makeSugarCane(Chunk &chunk, Random<std::minstd_rand> &rand, int x, int y, int z)
{
	StructureBuilder builder;

	int height = rand.intInRange(2, 4);

	builder.makeColumn(x, z, y, height, BlockId::SugarCane);
	builder.build(chunk);
}
