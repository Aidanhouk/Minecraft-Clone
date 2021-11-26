#include "ItemStack.h"

#include "Material.h"

#include <iostream>

ItemStack::ItemStack(BlockId blockId, int amount)
    : m_blockId(blockId)
    , m_numInStack(amount)
{
	m_maxToolDurability = Material::toMaterial(blockId).durability;
	m_toolDurability = m_maxToolDurability;
}

ItemStack::ItemStack(BlockId blockId, int amount, int durability)
	: m_blockId(blockId)
	, m_numInStack(1)
{
	m_maxToolDurability = Material::toMaterial(blockId).durability;
	m_toolDurability = durability;
}

ItemStack::ItemStack(const ItemStack & x)
	: m_blockId(x.m_blockId),
	m_numInStack(x.m_numInStack)
{
	if (x.m_toolDurability == 0) {
		m_maxToolDurability = Material::toMaterial(m_blockId).durability;
		m_toolDurability = m_maxToolDurability;
	}
	else {
		m_maxToolDurability = x.m_maxToolDurability;
		m_toolDurability = x.m_toolDurability;
	}
}

ItemStack & ItemStack::operator=(const ItemStack & x)
{
	if (&x == this)
		return *this;

	m_blockId = x.m_blockId;
	m_numInStack = x.m_numInStack;

	if (x.m_toolDurability == 0) {
		m_maxToolDurability = Material::toMaterial(m_blockId).durability;
		m_toolDurability = m_maxToolDurability;
	}
	else {
		m_maxToolDurability = x.m_maxToolDurability;
		m_toolDurability = x.m_toolDurability;
	}

	return *this;
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

bool ItemStack::loseDurability(int loseDur)
{
	m_toolDurability -= loseDur;

	if (m_toolDurability <= 0)
		return true;
	else
		return false;
}

void ItemStack::setData(BlockId blockId, int amount)
{
	m_blockId = blockId;
	m_numInStack = amount;

	m_maxToolDurability = Material::toMaterial(blockId).durability;
	m_toolDurability = m_maxToolDurability;
}

void ItemStack::setData(BlockId blockId, int amount, int durability)
{
	m_blockId = blockId;
	m_numInStack = 1;

	m_maxToolDurability = Material::toMaterial(blockId).durability;
	m_toolDurability = durability;
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
	return Material::toMaterial(m_blockId).maxStackSize;
}

int ItemStack::getMaxToolDurability() const
{
	return m_maxToolDurability;
}

int ItemStack::getToolDurability() const
{
	return m_toolDurability;
}
