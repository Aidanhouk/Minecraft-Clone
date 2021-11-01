#pragma once

#include "../AudioSet.h"
#include "Music.h"

#include "Util/Random.h"

#include <vector>
#include <memory>

class MusicSet : public AudioSet
{
public:
	MusicSet(MusicId id, std::vector<std::string>&& filePaths);

	virtual void playRandom();
	virtual void keepPlaying();
	virtual void stopAll();

	virtual void setVolumeAll(float volume);
	virtual void setDefaultVolumeAll();

	std::vector<std::unique_ptr<Music>>& getMusicSet() { return m_musicSet; }
private:
	std::vector<std::unique_ptr<Music>> m_musicSet;
public:
	static MusicSet DefaultInGameSet;
};