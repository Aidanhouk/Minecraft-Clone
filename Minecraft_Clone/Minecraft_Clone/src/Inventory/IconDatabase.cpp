#include "IconDatabase.h"

IconDatabase::IconDatabase()
	: textureAtlas("Interface/InventoryPack", 4096, 256)
{
}

IconDatabase &IconDatabase::get()
{
	static IconDatabase d;
	return d;
}