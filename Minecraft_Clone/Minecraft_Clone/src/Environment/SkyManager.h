#pragma once

#include "../Tick/TickObject.h"
#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include "../Model.h"
#include "../Shaders/Environment/SunShader.h"
#include "../Texture/BasicTexture.h"
#include "ProcSky.h"
#include "Cloud.h"
#include "Snowfall.h"
#include "Rain.h"

class Camera;

enum class PrecipitationType {
	NONE = 0,
	RAIN = 1,
	SNOW = 2,
};

class SkyManager : public TickObject
{
public:
	SkyManager();
	
	void tickUpdate(unsigned int tickTime);
	void update(glm::vec3 playerPosition, PrecipitationType precipitaionType);
	
	void setTime(unsigned int time);
	unsigned int getTime();
	
	void renderSkyBox(const Camera& camera);
	void render(const Camera& camera);
private:
	void playMusic(unsigned int dayTime);

	float precipitationLightLevel;
	unsigned int dayTime;
	
	unsigned int m_prevTime; // Ticks can skip around sometimes so...
	glm::vec3 playerPos;
	glm::mat4 transformMatrix;
	
	Model m_SunModel;
	Model m_MoonModel;
	SunShader m_shader;
	
	BasicTexture sun;
	BasicTexture moon;
	
	ProcSky skyBox;
	Clouds clouds;
	Snowfall snowfall;
	Rain rain;

	PrecipitationType m_currentPrecipitation;
	float m_precipitationVisibility;
};