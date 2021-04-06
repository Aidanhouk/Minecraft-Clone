#pragma once

#include "Util/Singleton.h"

#include "Texture/TextureAtlas.h"

class IconDatabase : public Singleton {
public:
	static IconDatabase &get();

	TextureAtlas textureAtlas;
private:
	IconDatabase();
};