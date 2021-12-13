#include "ChunkBlock.h"

#include "BlockDatabase.h"

ChunkBlock::ChunkBlock(Block_t id)
    : id(id)
{
}

ChunkBlock::ChunkBlock(BlockId id)
    : id(static_cast<Block_t>(id))
{
}

ChunkBlock::ChunkBlock(Block_t id, unsigned char light)
	: id(id), light(light)
{
}

ChunkBlock::ChunkBlock(BlockId id, unsigned char light)
	: id(static_cast<Block_t>(id)), light(light)
{
}

const BlockDataHolder &ChunkBlock::getData() const
{
    return BlockDatabase::get().getData((BlockId)id).getBlockData();
}

const BlockType &ChunkBlock::getType() const
{
    return BlockDatabase::get().getBlock((BlockId)id);
}

bool ChunkBlock::operator==(ChunkBlock other) const
{
	return id == other.id;
}

bool ChunkBlock::operator!=(ChunkBlock other) const
{
	return !(*this == other);
}



int ChunkBlock::getSunLight() const
{
	return (light >> 4) & 0xF;
}

int ChunkBlock::getTorchLight() const
{
	return light & 0xF;
}

int ChunkBlock::getBlockLight() const
{
	return light;
}



int ChunkBlock::getAO_Top_LeftBack() const
{
	return (AO_TopBottom >> 14) & 0b11;
}
void ChunkBlock::setAO_Top_LeftBack(int value)
{
	AO_TopBottom = (AO_TopBottom & 0b0011111111111111) | (value << 14);
}
int ChunkBlock::getAO_Top_LeftFront() const
{
	return (AO_TopBottom >> 12) & 0b11;
}
void ChunkBlock::setAO_Top_LeftFront(int value)
{
	AO_TopBottom = (AO_TopBottom & 0b1100111111111111) | (value << 12);
}
int ChunkBlock::getAO_Top_RightBack() const
{
	return (AO_TopBottom >> 10) & 0b11;
}
void ChunkBlock::setAO_Top_RightBack(int value)
{
	AO_TopBottom = (AO_TopBottom & 0b1111001111111111) | (value << 10);
}
int ChunkBlock::getAO_Top_RightFront() const
{
	return (AO_TopBottom >> 8) & 0b11;
}
void ChunkBlock::setAO_Top_RightFront(int value)
{
	AO_TopBottom = (AO_TopBottom & 0b1111110011111111) | (value << 8);
}



int ChunkBlock::getAO_Bottom_LeftBack() const
{
	return (AO_TopBottom >> 6) & 0b11;
}
void ChunkBlock::setAO_Bottom_LeftBack(int value)
{
	AO_TopBottom = (AO_TopBottom & 0b1111111100111111) | (value << 6);
}
int ChunkBlock::getAO_Bottom_LeftFront() const
{
	return (AO_TopBottom >> 4) & 0b11;
}
void ChunkBlock::setAO_Bottom_LeftFront(int value)
{
	AO_TopBottom = (AO_TopBottom & 0b1111111111001111) | (value << 4);
}
int ChunkBlock::getAO_Bottom_RightBack() const
{
	return (AO_TopBottom >> 2) & 0b11;
}
void ChunkBlock::setAO_Bottom_RightBack(int value)
{
	AO_TopBottom = (AO_TopBottom & 0b1111111111110011) | (value << 2);
}
int ChunkBlock::getAO_Bottom_RightFront() const
{
	return AO_TopBottom & 0b11;
}
void ChunkBlock::setAO_Bottom_RightFront(int value)
{
	AO_TopBottom = (AO_TopBottom & 0b1111111111111100) | value;
}



int ChunkBlock::getAO_Left_TopBack() const
{
	return (AO_LeftRight >> 14) & 0b11;
}
void ChunkBlock::setAO_Left_TopBack(int value)
{
	AO_LeftRight = (AO_LeftRight & 0b0011111111111111) | (value << 14);
}
int ChunkBlock::getAO_Left_TopFront() const
{
	return (AO_LeftRight >> 12) & 0b11;
}
void ChunkBlock::setAO_Left_TopFront(int value)
{
	AO_LeftRight = (AO_LeftRight & 0b1100111111111111) | (value << 12);
}
int ChunkBlock::getAO_Left_BottomBack() const
{
	return (AO_LeftRight >> 10) & 0b11;
}
void ChunkBlock::setAO_Left_BottomBack(int value)
{
	AO_LeftRight = (AO_LeftRight & 0b1111001111111111) | (value << 10);
}
int ChunkBlock::getAO_Left_BottomFront() const
{
	return (AO_LeftRight >> 8) & 0b11;
}
void ChunkBlock::setAO_Left_BottomFront(int value)
{
	AO_LeftRight = (AO_LeftRight & 0b1111110011111111) | (value << 8);
}



int ChunkBlock::getAO_Right_TopBack() const
{
	return (AO_LeftRight >> 6) & 0b11;
}
void ChunkBlock::setAO_Right_TopBack(int value)
{
	AO_LeftRight = (AO_LeftRight & 0b1111111100111111) | (value << 6);
}
int ChunkBlock::getAO_Right_TopFront() const
{
	return (AO_LeftRight >> 4) & 0b11;
}
void ChunkBlock::setAO_Right_TopFront(int value)
{
	AO_LeftRight = (AO_LeftRight & 0b1111111111001111) | (value << 4);
}
int ChunkBlock::getAO_Right_BottomBack() const
{
	return (AO_LeftRight >> 2) & 0b11;
}
void ChunkBlock::setAO_Right_BottomBack(int value)
{
	AO_LeftRight = (AO_LeftRight & 0b1111111111110011) | (value << 2);
}
int ChunkBlock::getAO_Right_BottomFront() const
{
	return AO_LeftRight & 0b11;
}
void ChunkBlock::setAO_Right_BottomFront(int value)
{
	AO_LeftRight = (AO_LeftRight & 0b1111111111111100) | value;
}



int ChunkBlock::getAO_Back_TopLeft() const
{
	return (AO_BackFront >> 14) & 0b11;
}
void ChunkBlock::setAO_Back_TopLeft(int value)
{
	AO_BackFront = (AO_BackFront & 0b0011111111111111) | (value << 14);
}
int ChunkBlock::getAO_Back_TopRight() const
{
	return (AO_BackFront >> 12) & 0b11;
}
void ChunkBlock::setAO_Back_TopRight(int value)
{
	AO_BackFront = (AO_BackFront & 0b1100111111111111) | (value << 12);
}
int ChunkBlock::getAO_Back_BottomLeft() const
{
	return (AO_BackFront >> 10) & 0b11;
}
void ChunkBlock::setAO_Back_BottomLeft(int value)
{
	AO_BackFront = (AO_BackFront & 0b1111001111111111) | (value << 10);
}
int ChunkBlock::getAO_Back_BottomRight() const
{
	return (AO_BackFront >> 8) & 0b11;
}
void ChunkBlock::setAO_Back_BottomRight(int value)
{
	AO_BackFront = (AO_BackFront & 0b1111110011111111) | (value << 8);
}



int ChunkBlock::getAO_Front_TopLeft() const
{
	return (AO_BackFront >> 6) & 0b11;
}
void ChunkBlock::setAO_Front_TopLeft(int value)
{
	AO_BackFront = (AO_BackFront & 0b1111111100111111) | (value << 6);
}
int ChunkBlock::getAO_Front_TopRight() const
{
	return (AO_BackFront >> 4) & 0b11;
}
void ChunkBlock::setAO_Front_TopRight(int value)
{
	AO_BackFront = (AO_BackFront & 0b1111111111001111) | (value << 4);
}
int ChunkBlock::getAO_Front_BottomLeft() const
{
	return (AO_BackFront >> 2) & 0b11;
}
void ChunkBlock::setAO_Front_BottomLeft(int value)
{
	AO_BackFront = (AO_BackFront & 0b1111111111110011) | (value << 2);
}
int ChunkBlock::getAO_Front_BottomRight() const
{
	return AO_BackFront & 0b11;
}
void ChunkBlock::setAO_Front_BottomRight(int value)
{
	AO_BackFront = (AO_BackFront & 0b1111111111111100) | value;
}