#ifndef TERRAINGENERATOR_H_INCLUDED
#define TERRAINGENERATOR_H_INCLUDED

enum class BiomeId;

class Chunk;

class TerrainGenerator {
public:
	virtual ~TerrainGenerator() = default;

    virtual void generateTerrainFor(Chunk &chunk) = 0;
    virtual int getMinimumSpawnHeight() const noexcept = 0;
};

#endif // TERRAINGENERATOR_H_INCLUDED
