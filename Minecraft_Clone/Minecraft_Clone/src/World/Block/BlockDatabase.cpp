#include "BlockDatabase.h"

BlockDatabase::BlockDatabase()
    : textureAtlas("DefaultPack", 256, 16)
{
    m_blocks[(int)BlockId::Air] = std::make_unique<DefaultBlock>("Air");
    m_blocks[(int)BlockId::Grass] = std::make_unique<DefaultBlock>("Grass");
    m_blocks[(int)BlockId::Dirt] = std::make_unique<DefaultBlock>("Dirt");
    m_blocks[(int)BlockId::Stone] = std::make_unique<DefaultBlock>("Stone");
    m_blocks[(int)BlockId::OakBark] = std::make_unique<DefaultBlock>("OakBark");
    m_blocks[(int)BlockId::OakLeaf] = std::make_unique<DefaultBlock>("OakLeaf");
    m_blocks[(int)BlockId::Sand] = std::make_unique<DefaultBlock>("Sand");
    m_blocks[(int)BlockId::Water] = std::make_unique<DefaultBlock>("Water");
    m_blocks[(int)BlockId::Cactus] = std::make_unique<DefaultBlock>("Cactus");
    m_blocks[(int)BlockId::TallGrass] = std::make_unique<DefaultBlock>("TallGrass");
    m_blocks[(int)BlockId::Rose] = std::make_unique<DefaultBlock>("Rose");
	m_blocks[(int)BlockId::DeadShrub] = std::make_unique<DefaultBlock>("DeadShrub");
	m_blocks[(int)BlockId::BirchBark] = std::make_unique<DefaultBlock>("BirchBark");
	m_blocks[(int)BlockId::PalmBark] = std::make_unique<DefaultBlock>("PalmBark");
	m_blocks[(int)BlockId::PalmLeaf] = std::make_unique<DefaultBlock>("PalmLeaf");
	m_blocks[(int)BlockId::BirchLeaf] = std::make_unique<DefaultBlock>("BirchLeaf");
	m_blocks[(int)BlockId::SugarCane] = std::make_unique<DefaultBlock>("SugarCane");
	m_blocks[(int)BlockId::Snow] = std::make_unique<DefaultBlock>("Snow");
	m_blocks[(int)BlockId::SpruceBark] = std::make_unique<DefaultBlock>("SpruceBark");
	m_blocks[(int)BlockId::SpruceLeaf] = std::make_unique<DefaultBlock>("SpruceLeaf");
	m_blocks[(int)BlockId::Ice] = std::make_unique<DefaultBlock>("Ice");
	m_blocks[(int)BlockId::Dandelion] = std::make_unique<DefaultBlock>("Dandelion");
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
