#pragma once

#include "../AudioSet.h"
#include "Sound.h"

#include "Util/Random.h"

#include <vector>

class SoundSet : public AudioSet
{
public:
	SoundSet(SoundId id, std::string &&filePath, int number);
	SoundSet(SoundId id, std::vector<std::string>&& filePaths);

	virtual void playRandom();
	virtual void keepPlaying();
	virtual void stopAll();

	virtual void setVolumeAll(float volume);
	virtual void setDefaultVolumeAll();
	virtual void setPitchAll(float pitch);

	std::vector<Sound>& getSoundSet() { return m_soundSet; }
private:
	std::vector<Sound> m_soundSet;
public:
	static SoundSet StepGrassSet;
	static SoundSet StepGravelSet;
	static SoundSet StepSandSet;
	static SoundSet StepSnowSet;
	static SoundSet StepStoneSet;
	static SoundSet StepWoodSet;
	static SoundSet StepWoolSet;

	static SoundSet BreakGlassSet;
	static SoundSet BreakGravelSet;
	static SoundSet BreakSnowSet;
	static SoundSet BreakSandSet;
	static SoundSet BreakGrassSet;
	static SoundSet BreakStoneSet;
	static SoundSet BreakWoodSet;

	static SoundSet EnterWaterSet;
	static SoundSet ExitWaterSet;

	static SoundSet UnderwaterCommonSet;
	static SoundSet UnderwaterRareSet;
	static SoundSet UnderwaterUltraRareSet;

	static SoundSet DrownDamageSet;

	static SoundSet RainSet;

	static SoundSet EatSet;
};