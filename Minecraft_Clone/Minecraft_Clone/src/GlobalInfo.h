#pragma once

class Camera;

struct GlobalInfo{
	bool fog;
	bool weather;
    float lighting;
    int tickTime;
    int dayTime;
    float elapsedTime;
    float deltaTime;
    Camera* cam;
};

extern GlobalInfo g_Info;