#include "SoundMaster.h"

#include "SoundFunctions.h"
#include "GlobalInfo.h"
#include "Player/Player.h"
#include "Player/PlayerInfo.h"

#include <iostream>

SoundMaster g_SoundMaster;

const std::vector<SoundSet*> ALL_SOUND_SETS
{
	&SoundSet::StepGrassSet,
	&SoundSet::StepGravelSet,
	&SoundSet::StepSandSet,
	&SoundSet::StepSnowSet,
	&SoundSet::StepStoneSet,
	&SoundSet::StepWoodSet,
	&SoundSet::StepWoolSet,

	&SoundSet::BreakGlassSet,
	&SoundSet::BreakGravelSet,
	&SoundSet::BreakSnowSet,
	&SoundSet::BreakSandSet,
	&SoundSet::BreakGrassSet,
	&SoundSet::BreakStoneSet,
	&SoundSet::BreakWoodSet,

	&SoundSet::EnterWaterSet,
	&SoundSet::ExitWaterSet,

	&SoundSet::RainSet,

	&SoundSet::DrownDamageSet,

	&SoundSet::UnderwaterCommonSet,
	&SoundSet::UnderwaterRareSet,
	&SoundSet::UnderwaterUltraRareSet,
};

const std::vector<MusicSet*> ALL_MUSIC_SETS
{
	&MusicSet::DefaultInGameSet,
};

SoundMaster::SoundMaster()
{
	m_play = true;
	m_addedNewSound = false;
	m_soundThread = std::make_unique<std::thread>(std::bind(&SoundMaster::update, this));
}

void SoundMaster::stopEverything()
{
	for (int i = static_cast<SoundId>(0); i != SoundId::NUMBER_OF_SOUNDS; ++i)
		Sound::toSound(static_cast<SoundId>(i)).stop();

	for (auto & var : ALL_SOUND_SETS)
		var->stopAll();
	for (auto & var : ALL_MUSIC_SETS)
		var->stopAll();

	m_play = false;
	(*m_soundThread).join();
}



void SoundMaster::play(SoundId soundID)
{
	Sound::toSound(soundID).play();
}

void SoundMaster::play(SoundId soundID, sf::Vector3f & soundPosition)
{
	Sound::toSound(soundID).play(soundPosition);
}

void SoundMaster::keepPlaying(SoundId soundID)
{
	Sound::toSound(soundID).keepPlaying();
}

void SoundMaster::stop(SoundId soundID)
{
	Sound::toSound(soundID).stop();
}



void SoundMaster::playRandom(AudioSet& audioSet)
{
	audioSet.playRandom();
}

void SoundMaster::keepPlaying(AudioSet& audioSet)
{
	audioSet.keepPlaying();
}

void SoundMaster::stopAll(AudioSet& audioSet)
{
	audioSet.stopAll();
}

void SoundMaster::setVolumeAll(AudioSet& audioSet, float volume)
{
	audioSet.setVolumeAll(volume);
}

void SoundMaster::setDefaultVolumeAll(AudioSet& audioSet)
{
	audioSet.setDefaultVolumeAll();
}

void SoundMaster::setPitchAll(AudioSet & audioSet, float pitch)
{
	audioSet.setPitchAll(pitch);
}



void SoundMaster::rainStart()
{
	auto& soundSet = SoundSet::RainSet.getSoundSet();
	for (int i = 0; i < soundSet.size(); ++i) {
		soundSet[i].keepPlaying();
		auto offset = soundSet[i].getSoundDuration().asSeconds() / soundSet.size() * i;
		soundSet[i].setOffset(offset);
	}
}

void SoundMaster::rainStop()
{
	SoundSet::RainSet.stopAll();
}

void SoundMaster::rainSetVolume(float volume)
{
	SoundSet::RainSet.setVolumeAll(volume);
}

void SoundMaster::playUnderwaterSound()
{
	int rnd = m_random.intInRange(1, 10);
	if (rnd <= 6)
		SoundSet::UnderwaterCommonSet.playRandom();
	else if (rnd <= 9)
		SoundSet::UnderwaterRareSet.playRandom();
	else// if (rnd == 10)
		SoundSet::UnderwaterUltraRareSet.playRandom();
}

void SoundMaster::addSound(SoundId soundId, Entity * source)
{
	SoundAndSource s{ Sound(Sound::toSound(soundId)) , source };
	m_sounds.emplace_back(std::move(s));
	m_addedNewSound = true;
}

void SoundMaster::update()
{
	const static float MAX_DISTANCE = 16.0f;

	while (m_play) {
		if (m_addedNewSound) {
			for (auto iter = m_sounds.end(); iter != m_sounds.begin(); --iter) {
				if ((--iter)->sound.getSoundStatus() != sf::Sound::Status::Playing) {

					float diffX = iter->source->position.x - g_PlayerInfo.player->position.x;
					float diffY = iter->source->position.y - g_PlayerInfo.player->position.y;
					float diffZ = iter->source->position.z - g_PlayerInfo.player->position.z;
					sf::Vector3f diffXYZ{ diffX, diffY, diffZ };

					float distance = distanceFromSound(diffXYZ);
					if (distance <= MAX_DISTANCE) {
						float attenuation = (MAX_DISTANCE - distance) / MAX_DISTANCE;
						iter->sound.setVolume(iter->sound.getVolumeDefault() * attenuation);
					}
					else
						iter->sound.setVolume(0.0f);

					iter->sound.play();
				}
				else
					break;
			}
			m_addedNewSound = false;
		}

		for (auto iter = m_sounds.begin(); iter != m_sounds.end(); ++iter) {

			if (!m_addedNewSound &&// iter->source &&// @TODO If sound source is absent
				iter->sound.getSoundStatus() == sf::Sound::Status::Playing) {

				float diffX = iter->source->position.x - g_PlayerInfo.player->position.x;
				float diffY = iter->source->position.y - g_PlayerInfo.player->position.y;
				float diffZ = iter->source->position.z - g_PlayerInfo.player->position.z;
				sf::Vector3f diffXYZ{ diffX, diffY, diffZ };

				float distance = distanceFromSound(diffXYZ);
				if (distance <= MAX_DISTANCE) {

					float attenuation = (MAX_DISTANCE - distance) / MAX_DISTANCE;
					iter->sound.setVolume(iter->sound.getVolumeDefault() * attenuation);

					sf::Vector3f soundPosition = calculateSoundPosition(diffXYZ, g_PlayerInfo.player->rotation.y);
					iter->sound.setPosition(soundPosition);
				}
			}
			else if (!m_addedNewSound)
				m_sounds.erase(iter);
		}
	}
}