#include "SoundSet.h"

#include <iostream>

SoundSet::SoundSet(SoundId id, std::string && filePath, int number)
{
	m_size = number;
	m_soundSet.reserve(number);
	for (int i = 1; i <= number; ++i)
		m_soundSet.emplace_back(id, filePath + std::to_string(i));
}

SoundSet::SoundSet(SoundId id, std::vector<std::string>&& filePaths)
{
	m_size = filePaths.size();

	m_soundSet.reserve(m_size);
	for (auto & filePath: filePaths)
		m_soundSet.emplace_back(id, std::move(filePath));
}

void SoundSet::playRandom()
{
	int rnd = m_random.intInRange(0, m_size - 1);
	m_lastPlayed = rnd;
	m_soundSet[rnd].play();
}

void SoundSet::keepPlaying()
{
	if (m_lastPlayed == -1 ||
		m_soundSet[m_lastPlayed].getSoundStatus() == sf::Sound::Status::Stopped) {
		int rnd = m_random.intInRange(0, m_size - 1);
		m_lastPlayed = rnd;
		m_soundSet[rnd].play();
	}
}

void SoundSet::stopAll()
{
	for (auto & var : m_soundSet)
		var.stop();
}

void SoundSet::setVolumeAll(float volume)
{
	for (auto & var : m_soundSet)
		var.setVolume(volume);
}

void SoundSet::setDefaultVolumeAll()
{
	for (auto & var : m_soundSet)
		var.setVolumeToDefault();
}

void SoundSet::setPitchAll(float pitch)
{
	for (auto & var : m_soundSet)
		var.setPitch(pitch);
}

SoundSet SoundSet::StepGrassSet		(SoundId::StepGrass,	"step/grass", 6);
SoundSet SoundSet::StepGravelSet	(SoundId::StepGravel,	"step/gravel", 4);
SoundSet SoundSet::StepSandSet		(SoundId::StepSand,		"step/sand", 5);
SoundSet SoundSet::StepSnowSet		(SoundId::StepSnow,		"step/snow", 4);
SoundSet SoundSet::StepStoneSet		(SoundId::StepStone,	"step/stone", 6);
SoundSet SoundSet::StepWoodSet		(SoundId::StepWood,		"step/wood", 6);
SoundSet SoundSet::StepWoolSet		(SoundId::StepWool,		"step/wool", 4);

SoundSet SoundSet::BreakGlassSet	(SoundId::BreakGlass,	"break/glass", 3);
SoundSet SoundSet::BreakGravelSet	(SoundId::BreakGravel,	"break/gravel", 4);
SoundSet SoundSet::BreakSnowSet		(SoundId::BreakSnow,	"break/snow", 4);
SoundSet SoundSet::BreakSandSet		(SoundId::BreakSand,	"break/sand", 4);
SoundSet SoundSet::BreakGrassSet	(SoundId::BreakGrass,	"break/grass", 4);
SoundSet SoundSet::BreakStoneSet	(SoundId::BreakStone,	"break/stone", 4);
SoundSet SoundSet::BreakWoodSet		(SoundId::BreakWood,	"break/wood", 4);

SoundSet SoundSet::EnterWaterSet	(SoundId::EnterWater,	"water/enter", 3);
SoundSet SoundSet::ExitWaterSet		(SoundId::ExitWater,	"water/exit", 3);

SoundSet SoundSet::DrownDamageSet	(SoundId::DrownDamage,	"player/drown", 4);

SoundSet SoundSet::RainSet			(SoundId::RainSound,	"weather/rain", 8);

SoundSet SoundSet::EatSet			(SoundId::Eat,			"player/eat", 3);

//SoundSet SoundSet::UnderwaterCommonSet(SoundId::UnderwaterCommon, "water/common/common", 8);
//SoundSet SoundSet::UnderwaterRareSet(SoundId::UnderwaterRare, "water/rare/rare", 8);
//SoundSet SoundSet::UnderwaterUltraRareSet(SoundId::UnderwaterUltraRare, "water/ultra_rare/ultra_rare", 5);

SoundSet SoundSet::UnderwaterCommonSet	(SoundId::UnderwaterCommon, {
	{ "water/common/bubbles1" },
	{ "water/common/bubbles2" },
	{ "water/common/bubbles3" },
	{ "water/common/bubbles4" },
	{ "water/common/bubbles5" },
	{ "water/common/bubbles6" },
	{ "water/common/water1" },
	{ "water/common/water2" },
	});
SoundSet SoundSet::UnderwaterRareSet(SoundId::UnderwaterRare, {
	{ "water/rare/animal1" },
	{ "water/rare/bass_whale1" },
	{ "water/rare/bass_whale2" },
	{ "water/rare/crackles1" },
	{ "water/rare/crackles2" },
	{ "water/rare/driplets1" },
	{ "water/rare/driplets2" },
	{ "water/rare/earth_crack" },
	});
SoundSet SoundSet::UnderwaterUltraRareSet(SoundId::UnderwaterUltraRare, {
	{ "water/ultra_rare/animal2" },
	{ "water/ultra_rare/dark1" },
	{ "water/ultra_rare/dark2" },
	{ "water/ultra_rare/dark3" },
	{ "water/ultra_rare/dark4" },
	});