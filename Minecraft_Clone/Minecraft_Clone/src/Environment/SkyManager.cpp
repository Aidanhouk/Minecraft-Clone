#include "SkyManager.h"

#include "../Camera.h"
#include "../GlobalInfo.h"
#include "Player/PlayerInfo.h"

#include <iostream>

#define degreesToRadians(x) x*(3.141592f/180.0f)

float blend(float x, float y, float factor)
{
    return x + ((y-x) * factor);
}

SkyManager::SkyManager()
    : sun("Environment/Sun"), moon("Environment/Moon")
{
    dayTime = 0; // Dawn
    m_prevTime = 0; //Time now

    std::vector<GLfloat> sVertexCoords {
        -75, 75, 400,
        75, 75, 400,
        75, -75, 400,
        -75, -75, 400
    };

    std::vector<GLfloat> sTextureCoords {
        0, 1,
        1, 1,
        1, 0,
        0, 0,
    };

    std::vector<GLuint> sIndexCoords {
        0, 1, 2,
        2, 3, 0
    };

	m_SunModel.addData({ sVertexCoords, sTextureCoords, sIndexCoords });

    std::vector<GLfloat> mVertexCoords {
        -45, 45, -400,
        45, 45, -400,
        45, -45, -400,
        -45, -45, -400
    };

    std::vector<GLfloat> mTextureCoords {
        0, 1,
        1, 1,
        1, 0,
        0, 0,
    };

    std::vector<GLuint> mIndexCoords {
        0, 1, 2,
        2, 3, 0
    };

	m_MoonModel.addData({ mVertexCoords, mTextureCoords, mIndexCoords });

}

void SkyManager::TickUpdate(unsigned int tickTime)
{
    //Day Time Management
    dayTime += tickTime - m_prevTime;
    m_prevTime = tickTime;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add)) {
		dayTime += 500;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract)) {
		dayTime += 23500;
		if (dayTime > 24000) {
			dayTime -= 24000;
		}
	}

    if(dayTime > 23999) {
        dayTime = 0;
    }

    //Ambient light
    // 0 = 6 am
	// 6000 = 12 am
    // 12000 = 6 pm
	// 18000 = 12 pm
    if(dayTime < 1500) { //6am - 9am sun gets brighter
        g_info.lighting = blend(0.6f, 1.0f, (float)dayTime / 1500);
    }
    if(dayTime > 1500 && dayTime < 10500) { //9am - 3pm sun is brightest
        g_info.lighting = 1.0f;
    }
    if(dayTime > 10500 && dayTime < 12000) { //3pm - 6pm sun gets dimmer
        g_info.lighting = blend(1.0f, 0.6f, (float)((float)dayTime - 10500) / 1500);
    }
    if(dayTime > 12000 && dayTime < 13500) { //6pm - 9pm sun light fades
        g_info.lighting = blend(0.6f, 0.1f, (float)((float)dayTime - 12000) / 1500);
    }
    if(dayTime > 13500 && dayTime < 22500) { //9pm - 3am is night
        g_info.lighting = 0.1f;
    }
    if(dayTime > 22500 && dayTime < 24000) {
        g_info.lighting = blend(0.1f, 0.6f, (float)((float)dayTime - 22500) / 1500);
    }

    //Update Sun/Moon matrix
    transformMatrix = glm::translate(glm::mat4(1.0f), playerPos);

    g_info.dayTime = dayTime;
}

void SkyManager::setTime(unsigned int tickTime)
{
    dayTime = tickTime;
}

unsigned int SkyManager::getTime()
{
    return dayTime;
}

void SkyManager::Update(glm::vec3 position)
{
    playerPos = position;
}

void SkyManager::render(const Camera& camera)
{
	glDisable(GL_CULL_FACE);
	
	skyBox.Render(camera);
	
	glEnable(GL_BLEND);
	m_shader.useProgram();
	m_SunModel.bindVAO();
	sun.bindTexture();
	
	m_shader.loadModelMatrix(transformMatrix);
	m_shader.loadProjectionViewMatrix(camera.getProjectionViewMatrix());
	m_shader.loadTime(degreesToRadians(((float)dayTime / 24000) * 360));
	
	GL::drawElements(m_SunModel.getIndicesCount());
	
	//m_shader.useProgram();
	m_MoonModel.bindVAO();
	moon.bindTexture();
	
	m_shader.loadModelMatrix(transformMatrix);
	m_shader.loadProjectionViewMatrix(camera.getProjectionViewMatrix());
	m_shader.loadTime(degreesToRadians(((float)dayTime / 24000) * 360));
	GL::drawElements(m_MoonModel.getIndicesCount());
	
	clouds.Render(camera, playerPos);
	if (!p_info.underwater) {
		if (g_info.weather == Weather::Rain)
			rain.Render(camera, playerPos);
		else if (g_info.weather == Weather::Snowfall)
			snowfall.Render(camera, playerPos);
	}

	glDisable(GL_BLEND);
	
	g_info.cam = (Camera*)&camera;
}
