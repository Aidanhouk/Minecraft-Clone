#pragma once

#include "Sound/SoundSet.h"
#include "Music/MusicSet.h"

#include "Entity.h"

#include <memory>
#include <thread>
#include <list>

struct SoundAndSource
{
	Sound sound;
	Entity* source;
};

class SoundMaster
{
public:
	SoundMaster();

	void stopEverything();

	void play(SoundId soundID);
	void play(SoundId soundID, sf::Vector3f &soundPosition);
	void keepPlaying(SoundId soundID);
	void stop(SoundId soundID);

	void playRandom(AudioSet& audioSet);
	void keepPlaying(AudioSet& audioSet);
	void stopAll(AudioSet& audioSet);

	void setVolumeAll(AudioSet& audioSet, float volume);
	void setDefaultVolumeAll(AudioSet& audioSet);
	void setPitchAll(AudioSet& audioSet, float pitch);

	void rainStart();
	void rainStop();
	void rainSetVolume(float volume);

	void playUnderwaterSound();
private:
	void update();


	Random<std::minstd_rand> m_random;

	std::list<SoundAndSource> m_sounds;

	std::unique_ptr<std::thread> m_soundThread;
	bool m_play;
	bool m_addedNewSound;
public:
	void addSound(SoundId soundId, Entity *source);
};

extern SoundMaster g_SoundMaster;