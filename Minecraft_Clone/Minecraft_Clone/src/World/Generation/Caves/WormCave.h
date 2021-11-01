#pragma once

#include "SFML/System.hpp"
#include <noise/noise.h>

class Chunk;

class WormCave
{
public:
	WormCave(const sf::Vector3f& pos = { 0.0f, 0.0f, 0.0f });

	void createCave(Chunk &chunk);

	// Sets the position of the worm's head segment in screen space.
	void SetCaveStartPos(const sf::Vector3f& pos) { m_caveStartPos = pos; }
	// Sets the seed of the Perlin-noise module.
	void SetSeed(int seed) { m_noise.SetSeed(seed); }
	// Sets the number of segments that make up the worm.
	void SetSegmentCount(double segmentCount) { m_segmentCount = segmentCount; }
	// Sets the worm's thickness, in screen units.
	void SetThickness(int thickness) { m_thickness = thickness; }
	// Defines the "twistiness" of the worms.  Higher values produce more
	// contorted worms.
	void SetTwistiness(double twistiness) { m_twistiness = twistiness; }

private:
	// Coordinates of the input value that generates the Perlin noise in
	// "noise space".  This is used to specify the angles of the worm's
	// segments.
	sf::Vector3f m_headNoisePos;
	// Position of the worm's head segment, in screen space.
	sf::Vector3f m_caveStartPos;
	// Noise module used to draw the worm.
	noise::module::Perlin m_noise;
	// Number of segments that make up the worm.
	int m_segmentCount;
	// Worm thickness.
	int m_thickness;
	// "Twistiness" of the worm.
	double m_twistiness;
};