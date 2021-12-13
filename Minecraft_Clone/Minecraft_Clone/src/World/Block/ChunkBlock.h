#pragma once

#include "BlockId.h"

struct BlockDataHolder;
class BlockType;

struct ChunkBlock {
	ChunkBlock() = default;

	ChunkBlock(Block_t id);
	ChunkBlock(BlockId id);
	ChunkBlock(Block_t id, unsigned char light);
	ChunkBlock(BlockId id, unsigned char light);

	const BlockDataHolder &getData() const;
	const BlockType &getType() const;

	const Block_t getId() const
	{
		return id;
	}

	bool operator==(ChunkBlock other) const;
	bool operator!=(ChunkBlock other) const;

	int getSunLight() const;
	int getTorchLight() const;
	int getBlockLight() const;

	int getAO_Top_LeftBack() const;
	void setAO_Top_LeftBack(int value);
	int getAO_Top_LeftFront() const;
	void setAO_Top_LeftFront(int value);
	int getAO_Top_RightBack() const;
	void setAO_Top_RightBack(int value);
	int getAO_Top_RightFront() const;
	void setAO_Top_RightFront(int value);

	int getAO_Bottom_LeftBack() const;
	void setAO_Bottom_LeftBack(int value);
	int getAO_Bottom_LeftFront() const;
	void setAO_Bottom_LeftFront(int value);
	int getAO_Bottom_RightBack() const;
	void setAO_Bottom_RightBack(int value);
	int getAO_Bottom_RightFront() const;
	void setAO_Bottom_RightFront(int value);

	int getAO_Left_TopBack() const;
	void setAO_Left_TopBack(int value);
	int getAO_Left_TopFront() const;
	void setAO_Left_TopFront(int value);
	int getAO_Left_BottomBack() const;
	void setAO_Left_BottomBack(int value);
	int getAO_Left_BottomFront() const;
	void setAO_Left_BottomFront(int value);

	int getAO_Right_TopBack() const;
	void setAO_Right_TopBack(int value);
	int getAO_Right_TopFront() const;
	void setAO_Right_TopFront(int value);
	int getAO_Right_BottomBack() const;
	void setAO_Right_BottomBack(int value);
	int getAO_Right_BottomFront() const;
	void setAO_Right_BottomFront(int value);

	int getAO_Back_TopLeft() const;
	void setAO_Back_TopLeft(int value);
	int getAO_Back_TopRight() const;
	void setAO_Back_TopRight(int value);
	int getAO_Back_BottomLeft() const;
	void setAO_Back_BottomLeft(int value);
	int getAO_Back_BottomRight() const;
	void setAO_Back_BottomRight(int value);

	int getAO_Front_TopLeft() const;
	void setAO_Front_TopLeft(int value);
	int getAO_Front_TopRight() const;
	void setAO_Front_TopRight(int value);
	int getAO_Front_BottomLeft() const;
	void setAO_Front_BottomLeft(int value);
	int getAO_Front_BottomRight() const;
	void setAO_Front_BottomRight(int value);

	Block_t id = 0;
	// First 4 bits are sunLight values, fully lit by default
	// Second 4 bits are torchLight values
	unsigned char light = 0xf0;
	// AO values must be store for each 4 vertices of all 6 faces = 24 vertices.
	// Vertex can be fully occluded, half occluded or fully lit -> 3 stages -> 2 bits for every 24 vertices.
	// That's 48 bits or 6 bytes. I use 3 shorts here for convenince but it might be a little ineffective.
	// 11 - fully lit, 10 - 66%, 01 - 33%, 00 - N/A
	unsigned short AO_TopBottom = 0xffffff;
	unsigned short AO_LeftRight = 0xffffff;
	unsigned short AO_BackFront = 0xffffff;
};