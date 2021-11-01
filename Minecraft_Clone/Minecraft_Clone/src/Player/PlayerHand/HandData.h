#pragma once

enum class HandType
{
	Empty = 0,
	Default = 1,
	Cube = 2,
	Cactus = 3,
};

enum class AnimationType
{
	Idle = 0,
	Breaking = 1,
	Eating = 2,
};

struct HandData
{
	HandType handType;
	AnimationType animationType;
	float animationStage;
};