#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

struct Config {
	bool customResolution = false;
    int windowX = 1920;
    int windowY = 1080;
    bool isFullscreen = true;
	float mouse_sensitivity = 5.0f;
    int renderDistance = 8;
    int fov = 70;
	float gamma = 1.0f;
	float contrast = 1.0f;
	float brightness = 1.0f;
	float saturation = 1.2f;
	float value = 1.0f;
	bool postProcess = true;
};

extern Config g_Config;

#endif // CONFIG_H_INCLUDED
