#include "WormCave.h"

#include <noise/mathconsts.h>
//#include "World/Chunk/Chunk.h"
#include "World/World/World.h"

#include <iostream>

// Default values
const int CAVE_SEGMENT_COUNT = 50;
const int CAVE_THICKNESS = 6;
const float CAVE_TWISTINESS = (1.0f / 64.0f);

const ChunkBlock CAVE_BLOCK(BlockId::Air, 0x00);

enum class Rotation
{
	Forward = 0,
	ForwardRight = 1,
	Right = 2,
	BackwardRight = 3,

	Backward = 4,
	BackwardLeft = 5,
	Left = 6,
	ForwardLeft = 7,

	NONE = 8,
};

WormCave::WormCave(const sf::Vector3f& pos)
{
	m_caveStartPos = pos;

	// The coordinates of the input value for the head segment must not
	// start at an integer boundary (for example, (0, 0, 0)).  At integer
	// boundaries, the coherent-noise values are always zero (blame gradient
	// noise for that), which would cause the worm to unrealistically
	// straighten those boundaries.
	m_headNoisePos.x = 7.0f / 2048.0f;
	m_headNoisePos.y = 1163.0f / 2048.0f;
	m_headNoisePos.z = 409.0f / 2048.0f;

	// Set up us the Perlin-noise module.
	m_noise.SetSeed(0);
	m_noise.SetFrequency(1.0);
	m_noise.SetLacunarity(2.375);
	m_noise.SetOctaveCount(3);
	m_noise.SetPersistence(0.5);
	m_noise.SetNoiseQuality(noise::QUALITY_STD);

	// Set the cave parameters with their default values.
	m_segmentCount = CAVE_SEGMENT_COUNT;
	m_thickness = CAVE_THICKNESS;
	m_twistiness = CAVE_TWISTINESS;
}

void WormCave::createCave(Chunk &chunk)
{
	// Position of the current segment being drawn, in screen space.
	sf::Vector3f curSegmentPos = m_caveStartPos;

	// The width of the worm's body at the current segment being drawn.
	sf::Vector3f offsetPos;

	// Coordinates of the input value, in "noise space", that specifies the
	// current segment's angle.
	sf::Vector3f curNoisePos;

	int thicknessHalf = m_thickness >> 1;
	int thicknessHalf2 = m_thickness - thicknessHalf;

	for (int curSegment = 0; curSegment < m_segmentCount; ++curSegment) {

		double noiseValueX = m_noise.GetValue(
			curNoisePos.x + (curSegment * m_twistiness),
			curNoisePos.y,
			curNoisePos.z);
		double noiseValueY = m_noise.GetValue(
			curNoisePos.x,
			curNoisePos.y + (curSegment * m_twistiness),
			curNoisePos.z);
		double noiseValueZ = m_noise.GetValue(
			curNoisePos.x,
			curNoisePos.y,
			curNoisePos.z + (curSegment * m_twistiness));

		// doesn't really matter weather to use sin or cos here
		offsetPos.x = sin(noiseValueX * 2.0 * noise::PI);
		offsetPos.z = sin(noiseValueZ * 2.0 * noise::PI);
		offsetPos.y = sin(noiseValueY * 0.25 * noise::PI);

		Rotation rotation = Rotation::NONE; // NONE if offset x and z == 0

		if (offsetPos.x != 0.0f && offsetPos.z != 0.0f) {
			if (offsetPos.x > 0.0f) {
				float ratio = offsetPos.x / offsetPos.z;
				if (ratio >= 3.0f)
					rotation = Rotation::Right;
				else if (ratio <= 1.0f / 3)
					rotation = Rotation::Forward;
				else
					rotation = Rotation::ForwardRight;
			}
			else {
				float ratio = offsetPos.x / -offsetPos.z;
				if (ratio >= 3.0f)
					rotation = Rotation::Right;
				else if (ratio <= 1.0f / 3)
					rotation = Rotation::Backward;
				else
					rotation = Rotation::BackwardRight;
			}
		}
		else {
			if (offsetPos.z > 0.0f) {
				float ratio = -offsetPos.x / offsetPos.z;
				if (ratio >= 3.0f)
					rotation = Rotation::Left;
				else if (ratio <= 1.0f / 3)
					rotation = Rotation::Backward;
				else
					rotation = Rotation::BackwardLeft;
			}
			else {
				float ratio = -offsetPos.x / -offsetPos.z;
				if (ratio >= 3.0f)
					rotation = Rotation::Left;
				else if (ratio <= 1.0f / 3)
					rotation = Rotation::Forward;
				else
					rotation = Rotation::ForwardLeft;
			}
		}

		switch (rotation)
		{
		case Rotation::Forward:
		case Rotation::Backward:
			for (int yy = curSegmentPos.y - thicknessHalf; yy < curSegmentPos.y + thicknessHalf2; ++yy)
				for (int xx = curSegmentPos.x - thicknessHalf; xx < curSegmentPos.x + thicknessHalf2; ++xx) {
					chunk.getWorldPtr()->addUnloadedBlock(xx, yy, curSegmentPos.z, CAVE_BLOCK);
					//chunk.setBlock(xx, yy, curSegmentPos.z, CAVE_BLOCK);
				}
			break;
		case Rotation::Right:
		case Rotation::Left:
			for (int yy = curSegmentPos.y - thicknessHalf; yy < curSegmentPos.y + thicknessHalf2; ++yy)
				for (int zz = curSegmentPos.z - thicknessHalf; zz < curSegmentPos.z + thicknessHalf2; ++zz) {
					chunk.getWorldPtr()->addUnloadedBlock(curSegmentPos.x, yy, zz, CAVE_BLOCK);
					//chunk.setBlock(curSegmentPos.x, yy, zz, CAVE_BLOCK);
				}
			break;
		
		case Rotation::ForwardRight:
			for (int yy = curSegmentPos.y - thicknessHalf; yy < curSegmentPos.y + thicknessHalf2; ++yy) {
				for (int xz = -thicknessHalf; xz <= thicknessHalf2; ++xz) {
					chunk.getWorldPtr()->addUnloadedBlock(curSegmentPos.x + xz, yy, curSegmentPos.z - xz, CAVE_BLOCK);
					//chunk.setBlock(curSegmentPos.x + xz, yy, curSegmentPos.z - xz, CAVE_BLOCK);
				}
				for (int xz = -thicknessHalf + 1; xz <= thicknessHalf2; ++xz) {
					chunk.getWorldPtr()->addUnloadedBlock(curSegmentPos.x + xz, yy, curSegmentPos.z + 1 - xz, CAVE_BLOCK);
					//chunk.setBlock(curSegmentPos.x + xz, yy, curSegmentPos.z + 1 - xz, CAVE_BLOCK);
				}
			}
			break;
		case Rotation::BackwardLeft:
			for (int yy = curSegmentPos.y - thicknessHalf; yy < curSegmentPos.y + thicknessHalf2; ++yy) {
				for (int xz = -thicknessHalf; xz <= thicknessHalf2; ++xz) {
					chunk.getWorldPtr()->addUnloadedBlock(curSegmentPos.x + xz, yy, curSegmentPos.z + xz, CAVE_BLOCK);
					//chunk.setBlock(curSegmentPos.x + xz, yy, curSegmentPos.z + xz, CAVE_BLOCK);
				}
				for (int xz = -thicknessHalf + 1; xz <= thicknessHalf2; ++xz) {
					chunk.getWorldPtr()->addUnloadedBlock(curSegmentPos.x - 1 + xz, yy, curSegmentPos.z + xz, CAVE_BLOCK);
					//chunk.setBlock(curSegmentPos.x - 1 + xz, yy, curSegmentPos.z + xz, CAVE_BLOCK);
				}
			}
			break;
		case Rotation::BackwardRight:
			for (int yy = curSegmentPos.y - thicknessHalf; yy < curSegmentPos.y + thicknessHalf2; ++yy) {
				for (int xz = -thicknessHalf; xz <= thicknessHalf2; ++xz) {
					chunk.getWorldPtr()->addUnloadedBlock(curSegmentPos.x + xz, yy, curSegmentPos.z + xz, CAVE_BLOCK);
					//chunk.setBlock(curSegmentPos.x + xz, yy, curSegmentPos.z + xz, CAVE_BLOCK);
				}
				for (int xz = -thicknessHalf + 1; xz <= thicknessHalf2; ++xz) {
					chunk.getWorldPtr()->addUnloadedBlock(curSegmentPos.x - 1 + xz, yy, curSegmentPos.z + xz, CAVE_BLOCK);
					//chunk.setBlock(curSegmentPos.x - 1 + xz, yy, curSegmentPos.z + xz, CAVE_BLOCK);
				}
			}
			break;
		case Rotation::ForwardLeft:
			for (int yy = curSegmentPos.y - thicknessHalf; yy < curSegmentPos.y + thicknessHalf2; ++yy) {
				for (int xz = -thicknessHalf; xz <= thicknessHalf2; ++xz) {
					chunk.getWorldPtr()->addUnloadedBlock(curSegmentPos.x + xz, yy, curSegmentPos.z - xz, CAVE_BLOCK);
					//chunk.setBlock(curSegmentPos.x + xz, yy, curSegmentPos.z - xz, CAVE_BLOCK);
				}
				for (int xz = -thicknessHalf + 1; xz <= thicknessHalf2; ++xz) {
					chunk.getWorldPtr()->addUnloadedBlock(curSegmentPos.x + xz, yy, curSegmentPos.z + 1 - xz, CAVE_BLOCK);
					//chunk.setBlock(curSegmentPos.x + xz, yy, curSegmentPos.z + 1 - xz, CAVE_BLOCK);
				}
			}
			break;
		case Rotation::NONE:
			for (int yy = curSegmentPos.y - thicknessHalf; yy < curSegmentPos.y + thicknessHalf2; ++yy) {
				chunk.getWorldPtr()->addUnloadedBlock(curSegmentPos.x, yy, curSegmentPos.z, CAVE_BLOCK);
				//chunk.setBlock(curSegmentPos.x, yy, curSegmentPos.z, CAVE_BLOCK);
			}
			break;
		default:
			break;
		}

		curSegmentPos.x += offsetPos.x;
		curSegmentPos.y += -abs(offsetPos.y);
		curSegmentPos.z += offsetPos.z;

		if (curSegmentPos.y <= 7)
			return;
	}
}
