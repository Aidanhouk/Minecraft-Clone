#pragma once

#include "SoundId.h"
#include "SFML/Audio.hpp"

#include <string>

class Sound {
public:
	Sound(SoundId id, std::string &&filePath);

	static Sound &toSound(SoundId id);

	void play();
	void play(sf::Vector3f &soundPosition);
	void keepPlaying();
	void stop();

	float getVolumeDefault() { return m_defaultVolume; }

	// volume values are from 0 to 100
	void setVolume(float volume);
	void setVolumeToDefault();
	void setPosition(sf::Vector3f &soundPosition);
	void setOffset(float offset);
	void setOffset(sf::Time offset);
	void setPitch(float pitch);

	sf::Time getSoundDuration();
	sf::SoundSource::Status getSoundStatus();
private:
	void setDefaultVolumes(SoundId id);
	float m_defaultVolume;

	const SoundId m_id;

	sf::Sound m_sound;
	sf::SoundBuffer m_buffer;

public:
	static Sound
		UNDERWATER_AMBIENCE,
		WATER_SPLASH_NORMAL,
		PLOP,
		FALL_DAMAGE,
		CLASSIC_HURT,
		PLAYER_WAS_HIT,
		BURP,
		ITEM_BREAK,

		LAST;
};