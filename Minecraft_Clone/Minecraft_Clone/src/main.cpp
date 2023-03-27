#include "Application.h"

#include <fstream>
#include <iostream>

#include "UserSettings/Config.h"

#ifdef __WIN32
extern "C" {
// Enable dedicated graphics
__declspec(dllexport) bool NvOptimusEnablement = true;
__declspec(dllexport) bool AmdPowerXpressRequestHighPerformance = true;
}
#endif // __WIN32

namespace
{
    static constexpr int RENDER_DISTANCE_MIN = 4;
    static constexpr int FOV_MIN = 30, FOV_MAX = 115;

	void loadConfig(Config &config);
	void loadShaders();
	void displayInfo();
}

int main()
{
	Config config;
	loadConfig(config);
	loadShaders();
	displayInfo();

	std::cin.ignore();
	std::cout << "Loading game...\n";

	Application app(config);

	app.runLoop();

	return EXIT_SUCCESS;
}

namespace {
	void loadConfig(Config &config)
	{
		std::ifstream configFile("config.txt");
		std::string key;

		if (configFile.is_open()) {
            std::cout << "config:\n";
			while (configFile >> key) {
				if (key == "render_distance") {
					configFile >> config.renderDistance;
					if (config.renderDistance < RENDER_DISTANCE_MIN)
						config.renderDistance = RENDER_DISTANCE_MIN;
#ifdef _DEBUG
					config.renderDistance = RENDER_DISTANCE_MIN;
#endif // _DEBUG
					std::cout << "Render distance: "
						<< config.renderDistance << '\n';
					// formula is not that good, would be nice to hardcore every render distance option
					config.fogDensity = 0.014f * 8.0f / config.renderDistance;
				}
				else if (key == "fullscreen") {
					configFile >> config.isFullscreen;
					std::cout << "Fullscreen mode: " << std::boolalpha
						<< config.isFullscreen << '\n';
				}
				else if (key == "resolution") {
					configFile >> config.windowX >> config.windowY;
					if (config.windowX == 0 || config.windowY == 0) {
						config.customResolution = false;
						std::cout << "Resolution: screen resolution\n";
					}
					else {
						config.customResolution = true;
						std::cout << "Resolution: " << config.windowX << " x "
							<< config.windowY << '\n';
					}
				}
				else if (key == "fov") {
					int fov;
					configFile >> fov;
                    std::clamp(fov, FOV_MIN, FOV_MAX);
					config.fov = fov;
					std::cout << "Field of view: " << config.fov << '\n';
				}

				else if (key == "gamma") {
					configFile >> config.gamma;
					std::cout << "Gamma: " << config.gamma << std::endl;
				}
				else if (key == "contrast") {
					configFile >> config.contrast;
					std::cout << "Contrast: " << config.contrast << std::endl;
				}
				else if (key == "saturation") {
					configFile >> config.saturation;
					std::cout << "Saturation: " << config.saturation << std::endl;
				}
				else if (key == "brightness") {
					configFile >> config.value;
					std::cout << "Brightness(value): " << config.value << std::endl;
				}
				else if (key == "post_process") {
					configFile >> config.postProcess;
					std::cout << "Post Processing: " << config.postProcess << std::endl;
				}

				else if (key == "mouse_sensitivity") {
					configFile >> config.mouseSensitivity;
					std::cout << "Mouse sensitivity: "
						<< config.mouseSensitivity << '\n';
				}

				else if (key == "music_volume") {
					configFile >> config.musicVolume;
					std::cout << "Music volume: "
						<< config.musicVolume << '\n';
				}
			}
			}
		else {
			std::cerr << "Error: Could not find config.txt file! Using defaults.\n";

			config.renderDistance = 12;
			// formula is not that good, would be nice to hardcore every render distance option
			config.fogDensity = 0.014f * 8.0f / config.renderDistance;
			config.isFullscreen = true;
			config.customResolution = false;
			config.windowX = 0;
			config.windowY = 0;
			config.fov = 70;
			config.gamma = 1.0f;
			config.contrast = 1.0f;
			config.saturation = 1.2f;
			config.value = 1.0f;
			config.postProcess = true;
			
			config.mouseSensitivity = 5.0f;
			
			config.musicVolume = 50.0f;
		}
		std::cout << "\n";
		// set settings which aren't in config.txt
		config.brightness = 1.0f;
	}

	void loadShaders()
	{
		std::ifstream shaderFile("shaders.txt");
		std::string key;

		if (shaderFile.is_open()) {
			while (shaderFile >> key) {
				if (key == "fxaa") {
					shaderFile >> g_ShaderSettings.fxaa;
					std::cout << "FXAA: " << g_ShaderSettings.fxaa << "\n";
				}
				else if (key == "bloom") {
					shaderFile >> g_ShaderSettings.bloom;
					std::cout << "Bloom: " << g_ShaderSettings.bloom << "\n";
				}
				else if (key == "motion_blur") {
					shaderFile >> g_ShaderSettings.motionblur;
					std::cout << "Motion Blur: " << g_ShaderSettings.motionblur << "\n";
				}
				else if (key == "anisotropic_filtration") {
					shaderFile >> g_ShaderSettings.aniso;
					std::cout << "Anisotropic Filtering: " << g_ShaderSettings.aniso << "\n";
				}
			}
		}
		else {
			std::cerr << "Error: Could not find shaders.txt file. Using defaults...\n";
		}
		std::cout << "\n";
	}

	void displayInfo()
	{
		std::ifstream infoFile("Res/info.txt");
		if (infoFile.is_open()) {
			std::string line;
			while (std::getline(infoFile, line))
				std::cout << line << "\n";
		}
        std::cout << "========== = Press enter to begin ============" << "\n";
	}
} // namespace