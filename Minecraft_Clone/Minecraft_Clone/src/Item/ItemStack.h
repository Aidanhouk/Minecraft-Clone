#pragma once

#include "World/Block/BlockId.h"

const BlockId EMPTY_SLOT_ID = BlockId::Air;

class ItemStack {
public:
	ItemStack(BlockId blockId, int amount);
	ItemStack(BlockId blockId, int amount, int durability);
	ItemStack() {}

	ItemStack(const ItemStack& x);

	ItemStack& operator=(const ItemStack& x);

	// returns leftover
    int add(int amount);
    void remove(int number);

	// returns if item is broken
	bool loseDurability(int loseDur);

	void setData(BlockId blockId, int amount);
	void setData(BlockId blockId, int amount, int durability);
	void clear();

    BlockId getBlockId() const;
    int getNumInStack() const;
	int getMaxStackSize() const;

	int getMaxToolDurability() const;
	int getToolDurability() const;
private:
    BlockId m_blockId;
    int m_numInStack;
	// this is very badly implemented but I don't want to refactor everything
	int m_maxToolDurability;
	int m_toolDurability;
};