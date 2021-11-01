#pragma once

#include "Util/Random.h"

class AudioSet
{
public:
	virtual void playRandom() = 0;
	virtual void keepPlaying() = 0;
	virtual void stopAll() = 0;

	virtual void setVolumeAll(float volume) = 0;
	virtual void setDefaultVolumeAll() = 0;
	virtual void setPitchAll(float pitch) {}

protected:
	Random<std::minstd_rand> m_random;
	int m_size;
	int m_lastPlayed = -1;
};