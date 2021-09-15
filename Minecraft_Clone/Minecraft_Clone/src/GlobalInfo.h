#ifndef GLOBAL_INFO_H
#define GLOBAL_INFO_H

enum class Weather
{
	None = 0,
	Rain = 1,
	Snowfall = 2,
};

class Camera;

struct GlobalInfo{
    float lighting;
    int tickTime;
    int dayTime;
    float elapsedTime;
    float deltaTime;
	Weather weather;
    Camera* cam;
};

extern GlobalInfo g_info;

#endif