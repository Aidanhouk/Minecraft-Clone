#pragma once

#include "Inventory/ItemSlot.h"

#include <array>

class ItemSlot;

void computeCrafting4(std::array<ItemSlot, 4>& slots, ItemSlot &result);
void computeCrafting9(std::array<ItemSlot, 9>& slots, ItemSlot &result);