#pragma once

#include "World/Block/BlockId.h"

const BlockId EMPTY_SLOT_ID = BlockId::Air;

class ItemStack {
public:
    ItemStack(BlockId blockId, int amount);
	ItemStack() {}

	// returns leftover
    int add(int amount);
    void remove(int number);

	void setData(BlockId blockId, int number);
	void clear();

    BlockId getBlockId() const;
    int getNumInStack() const;
	int getMaxStackSize() const;
private:
    BlockId m_blockId;
    int m_numInStack;
};