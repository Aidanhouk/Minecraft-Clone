#include "Music.h"

#include "Config.h"

#include <iostream>

//Music::Music(MusicId id, std::string && filePath)
//	: m_id(id)
//{
//	m_music.openFromFile("Res/Sounds/" + filePath + ".ogg");
//
//	setDefaultVolumes(id);
//}

void Music::create(MusicId id, std::string && filePath)
{
	m_id = id;

	m_music.openFromFile("Res/Sounds/" + filePath + ".ogg");

	setDefaultVolumes(id);
}

void Music::play()
{
	m_music.play();
}

void Music::play(sf::Vector3f & soundPosition)
{
	m_music.setPosition(soundPosition);
	m_music.play();
}

void Music::keepPlaying()
{
	if (m_music.getStatus() != sf::SoundSource::Status::Playing)
		m_music.play();
}

void Music::stop()
{
	m_music.stop();
}

void Music::setVolume(float volume)
{
	m_music.setVolume(volume);
}

void Music::setVolumeToDefault()
{
	m_music.setVolume(m_defaultVolume);
}

void Music::setOffset(float offset)
{
	m_music.setPlayingOffset(sf::seconds(offset));
}

void Music::setOffset(sf::Time offset)
{
	m_music.setPlayingOffset(offset);
}

sf::Time Music::getSoundDuration()
{
	return m_music.getDuration();
}

sf::SoundSource::Status Music::getSoundStatus()
{
	return m_music.getStatus();
}

void Music::setDefaultVolumes(MusicId id)
{
	m_defaultVolume = 100.0f;

	setVolume(m_defaultVolume);
}

//Music Music::LAST(MusicId::NUMBER_OF_MUSIC, "NONE");
//
//Music & Music::toMusic(MusicId id)
//{
//	switch (id)
//	{
//	default:
//		return LAST;
//	}
//}
