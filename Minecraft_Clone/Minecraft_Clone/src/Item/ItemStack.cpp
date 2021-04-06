#include "ItemStack.h"

#include <algorithm>

ItemStack::ItemStack(const Material &material, int amount)
    : m_pMaterial(&material)
    , m_numInStack(amount)
{
}

int ItemStack::add(int amount)
{
    m_numInStack += amount;
	// adding more than one item
    if (m_numInStack > m_pMaterial->maxStackSize) {
        int leftOver = m_numInStack - m_pMaterial->maxStackSize;
        m_numInStack = m_pMaterial->maxStackSize;
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
        m_pMaterial = &Material::NOTHING;
    }
}

int ItemStack::getNumInStack() const
{
    return m_numInStack;
}

const Material &ItemStack::getMaterial() const
{
    return *m_pMaterial;
}