#include "ItemStack.h"

#include "Material.h"

ItemStack::ItemStack(BlockId blockId, int amount)
    : m_blockId(blockId)
    , m_numInStack(amount)
{
}

int ItemStack::add(int amount)
{
    m_numInStack += amount;
	auto &material = Material::toMaterial(m_blockId);
	const int maxStackSize = material.maxStackSize;
    if (m_numInStack > maxStackSize) {
        int leftOver = m_numInStack - maxStackSize;
        m_numInStack = maxStackSize;
        return leftOver;
    }
    else {
        return 0;
    }
}

void ItemStack::remove(int number)
{
	// can't be negative
	m_numInStack -= number;
    if (m_numInStack == 0) {
        m_blockId = EMPTY_SLOT_ID;
    }
}

void ItemStack::setData(BlockId blockId, int number)
{
	m_blockId = blockId;
	m_numInStack = number;
}

void ItemStack::clear()
{
	m_blockId = EMPTY_SLOT_ID;
	m_numInStack = 0;
}

BlockId ItemStack::getBlockId() const
{
	return m_blockId;
}

int ItemStack::getNumInStack() const
{
    return m_numInStack;
}

int ItemStack::getMaxStackSize() const
{
	//auto &material = Material::toMaterial(m_blockId);
	return Material::toMaterial(m_blockId).maxStackSize;
}