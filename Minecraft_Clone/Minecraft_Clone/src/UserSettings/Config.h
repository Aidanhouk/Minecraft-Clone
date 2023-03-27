#pragma once

// defaults are initialized in main.cpp
struct Config {
	int renderDistance;
	bool isFullscreen;
	bool customResolution;
	int windowX;
	int windowY;
	int fov;
	float gamma;
	float contrast;
	float brightness;
	float saturation;
	float value;
	bool postProcess;

	float mouseSensitivity;

	float musicVolume;
	// this should be somewhere else
	float fogDensity;
};

extern Config g_Config;