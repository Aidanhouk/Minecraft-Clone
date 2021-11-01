#pragma once

#include "../../Util/NonCopyable.h"
#include "BlockId.h"
#include <SFML/Graphics.hpp>

// this should be refactored to much more classes with inheritance and so on
enum class ItemType {
	Default = 0,
	Block = 1,
	Tool = 2,
	Food = 3,
};

enum class BlockMeshType {
    Cube = 0,
    X = 1,
	Cactus = 2,
	Default = 3,
};

enum class BlockShaderType {
    Chunk = 0,
    Liquid = 1,
    Flora = 2,
};

struct BlockDataHolder : public NonCopyable {
    BlockId id;
    sf::Vector2i texTopCoord;
    sf::Vector2i texSideCoord;
    sf::Vector2i texBottomCoord;

	ItemType itemType;
    BlockMeshType meshType;
    BlockShaderType shaderType;

    bool isOpaque;
    bool isCollidable;

	float hardness;

	int hunger;
	float saturation;
};

class BlockData : public NonCopyable {
public:
    BlockData(const std::string &fileName);

    const BlockDataHolder &getBlockData() const;

private:
    BlockDataHolder m_data;
};