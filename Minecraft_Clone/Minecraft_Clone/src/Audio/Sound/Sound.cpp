#include "Sound.h"

#include <iostream>

Sound::Sound(SoundId id, std::string && filePath)
	: m_id{ id }
{
	if (id == SoundId::NUMBER_OF_SOUNDS)
		return;

	if (m_buffer.loadFromFile("Res/Sounds/" + filePath + ".ogg"))
		m_sound.setBuffer(m_buffer);

	setDefaultVolumes(id);
}

void Sound::play()
{
	m_sound.play();
}

void Sound::play(sf::Vector3f & soundPosition)
{
	m_sound.setPosition(soundPosition);
	m_sound.play();
}

void Sound::keepPlaying()
{
	if (m_sound.getStatus() != sf::SoundSource::Status::Playing)
		m_sound.play();
}

void Sound::stop()
{
	m_sound.stop();
}

void Sound::setVolume(float volume)
{
	m_sound.setVolume(volume);
}

void Sound::setVolumeToDefault()
{
	m_sound.setVolume(m_defaultVolume);
}

void Sound::setPosition(sf::Vector3f & soundPosition)
{
	m_sound.setPosition(soundPosition);
}

void Sound::setOffset(float offset)
{
	m_sound.setPlayingOffset(sf::seconds(offset));
}

void Sound::setOffset(sf::Time offset)
{
	m_sound.setPlayingOffset(offset);
}

void Sound::setPitch(float pitch)
{
	m_sound.setPitch(pitch);
}

sf::Time Sound::getSoundDuration()
{
	return m_buffer.getDuration();
}

sf::SoundSource::Status Sound::getSoundStatus()
{
	return m_sound.getStatus();
}

void Sound::setDefaultVolumes(SoundId id)
{
	m_defaultVolume = 100.0f;

	switch (id)
	{
	case SoundId::RainSound:
		m_sound.setLoop(true);
		break;
	case SoundId::Plop:
		m_defaultVolume = 30.0f;
		m_sound.setPitch(1.2f);
		break;
	case SoundId::FallDamage:
	case SoundId::WaterSplashNormal:
	case SoundId::PlayerWasHit:
	case SoundId::Eat:
	//case SoundId::Burp:
		m_defaultVolume = 50.0f;
		break;
	default:
		break;
	}

	m_sound.setVolume(m_defaultVolume);
}

Sound Sound::UNDERWATER_AMBIENCE(SoundId::UnderwaterAmbience, "Water/underwater_ambience");
Sound Sound::WATER_SPLASH_NORMAL(SoundId::WaterSplashNormal, "Water/water_splash_normal");
Sound Sound::PLOP(SoundId::Plop, "Misc/plop");
Sound Sound::FALL_DAMAGE(SoundId::FallDamage, "Player/fallbig");
Sound Sound::CLASSIC_HURT(SoundId::ClassicHurt, "Player/classic_hurt");
Sound Sound::PLAYER_WAS_HIT(SoundId::PlayerWasHit, "Player/player_was_hit");
Sound Sound::BURP(SoundId::Burp, "Player/burp");
Sound Sound::ITEM_BREAK(SoundId::ItemBreak, "Misc/itemBreak");

Sound Sound::LAST(SoundId::NUMBER_OF_SOUNDS, "NONE");

Sound & Sound::toSound(SoundId id)
{
	switch (id)
	{
	case SoundId::UnderwaterAmbience:
		return UNDERWATER_AMBIENCE;
	case SoundId::WaterSplashNormal:
		return WATER_SPLASH_NORMAL;
	case SoundId::Plop:
		return PLOP;
	case SoundId::FallDamage:
		return FALL_DAMAGE;
	case SoundId::ClassicHurt:
		return CLASSIC_HURT;
	case SoundId::PlayerWasHit:
		return PLAYER_WAS_HIT;
	case SoundId::Burp:
		return BURP;
	case SoundId::ItemBreak:
		return ITEM_BREAK;

	default:
		return LAST;
	}
}
