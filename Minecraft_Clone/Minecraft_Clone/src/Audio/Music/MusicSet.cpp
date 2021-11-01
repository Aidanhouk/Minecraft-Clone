#include "MusicSet.h"

#include <iostream>

MusicSet::MusicSet(MusicId id, std::vector<std::string>&& filePaths)
{
	m_size = filePaths.size();
	
	for (auto & filePath : filePaths) {
		auto ptr = std::make_unique<Music>();
		ptr->create(id, std::move(filePath));
		m_musicSet.emplace_back(std::move(ptr));
	}
}

void MusicSet::playRandom()
{
	int rnd = rand() % m_size;
	m_lastPlayed = rnd;
	m_musicSet[rnd].get()->play();
}

void MusicSet::keepPlaying()
{
	if (m_lastPlayed == -1 ||
		m_musicSet[m_lastPlayed].get()->getSoundStatus() == sf::Sound::Status::Stopped) {
		int rnd = m_random.intInRange(0, m_size - 1);
		m_lastPlayed = rnd;
		m_musicSet[rnd].get()->play();
	}
}

void MusicSet::stopAll()
{
	for (auto & var : m_musicSet)
		var.get()->stop();
}

void MusicSet::setVolumeAll(float volume)
{
	for (auto & var : m_musicSet)
		var.get()->setVolume(volume);
}

void MusicSet::setDefaultVolumeAll()
{
	for (auto & var : m_musicSet)
		var.get()->setVolumeToDefault();
}

MusicSet MusicSet::DefaultInGameSet(MusicId::DefaultInGame, {
	{ "music/Clark" },
	{ "music/Danny" },
	{ "music/Dry Hands" },
	{ "music/Haggstorm" },
	{ "music/Living Mice" },
	{ "music/Mice on Venus" },
	{ "music/Minecraft" },
	{ "music/Subwoofer Lullaby" },
	{ "music/Sweden" },
	{ "music/Wet Hands" },
	});