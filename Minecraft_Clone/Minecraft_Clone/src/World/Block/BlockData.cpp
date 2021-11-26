#include "BlockData.h"

#include <fstream>

BlockData::BlockData(const std::string &fileName)
{
    std::ifstream inFile("Res/Blocks/" + fileName + ".block");

    if (!inFile.is_open()) {
        throw std::runtime_error("Unable to open block file: " + fileName +
                                 "!");
    }

    std::string line;
    while (std::getline(inFile, line)) {
        if (line == "TexTop") {
            int x, y;
            inFile >> x >> y;
            m_data.texTopCoord = {x, y};
        }
        else if (line == "TexSide") {
            int x, y;
            inFile >> x >> y;
            m_data.texSideCoord = {x, y};
        }
        else if (line == "TexBottom") {
            int x, y;
            inFile >> x >> y;
            m_data.texBottomCoord = {x, y};
        }
        else if (line == "TexAll") {
            int x, y;
            inFile >> x >> y;
            m_data.texTopCoord = {x, y};
            m_data.texSideCoord = {x, y};
            m_data.texBottomCoord = {x, y};
        }
        else if (line == "Id") {
            int id;
            inFile >> id;
            m_data.id = static_cast<BlockId>(id);
        }
        else if (line == "Opaque") {
            inFile >> m_data.isOpaque;
        }
        else if (line == "Collidable") {
            inFile >> m_data.isCollidable;
        }
		else if (line == "ItemType") {
			//m_data.itemType = ItemType::Block;
			int id;
			inFile >> id;
			m_data.itemType = static_cast<ItemType>(id);
		}
        else if (line == "MeshType") {
            int id;
            inFile >> id;
            m_data.meshType = static_cast<BlockMeshType>(id);
        }
        else if (line == "ShaderType") {
            int id;
            inFile >> id;
            m_data.shaderType = static_cast<BlockShaderType>(id);
        }
		else if (line == "Hardness") {
			float hardness;
			inFile >> hardness;
			m_data.hardness = hardness;
		}
		else if (line == "ToolToMine") {
			int toolToMine;
			inFile >> toolToMine;
			m_data.toolToMine = static_cast<ToolToMine>(toolToMine);
		}
		else if (line == "ToolLevelToMine") {
			int toolLevelToMine;
			inFile >> toolLevelToMine;
			m_data.toolLevelToMine = static_cast<ToolLevelToMine>(toolLevelToMine);
		}
		else if (line == "EffieciencyCoef") {
			float effieciencyCoef;
			inFile >> effieciencyCoef;
			m_data.effieciencyCoef = effieciencyCoef;
		}

		else if (line == "Hunger") {
			float hunger;
			inFile >> hunger;
			m_data.hunger = hunger;
		}
		else if (line == "Saturation") {
			float saturation;
			inFile >> saturation;
			m_data.saturation = saturation;
		}
    }
}

const BlockDataHolder &BlockData::getBlockData() const
{
    return m_data;
}
