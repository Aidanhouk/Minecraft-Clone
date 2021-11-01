#pragma once

#include "MusicId.h"
#include "SFML/Audio.hpp"

#include <string>

class Music {
public:
	//Music(MusicId id, std::string &&filePath);
	void create(MusicId id, std::string &&filePath);

	//static Music &toMusic(MusicId id);

	void play();
	void play(sf::Vector3f &soundPosition);
	void keepPlaying();
	void stop();

	float getVolumeDefault() { return m_defaultVolume; }

	// volume values are from 0 to 100
	void setVolume(float volume);
	void setVolumeToDefault();
	void setOffset(float offset);
	void setOffset(sf::Time offset);

	sf::Time getSoundDuration();
	sf::SoundSource::Status getSoundStatus();
private:
	void setDefaultVolumes(MusicId id);

	sf::Music m_music;
	MusicId m_id;

	float m_defaultVolume;

//public:
//	static Music
//
//		LAST;
};