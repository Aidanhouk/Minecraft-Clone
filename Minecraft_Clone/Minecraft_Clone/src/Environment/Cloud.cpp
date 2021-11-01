#include "Cloud.h"

#include <vector>
#include "SkyManager.h"
#include "../Application.h"

#include <iostream>

Clouds::Clouds()
	: m_cloudTexture("Environment/clouds")
{
    std::vector<GLfloat> mvertexCoords {
        -2048, 172, 2048,
        2048, 172, 2048,
        2048, 172, -2048,
        -2048, 172, -2048
    };

    std::vector<GLfloat> mtextureCoords {
        0, 1,
        1, 1,
        1, 0,
        0, 0,
    };

    std::vector<GLuint> mindexCoords {
        0, 1, 2,
        2, 3, 0
    };

	m_cloud.addData({ mvertexCoords, mtextureCoords, mindexCoords });
}

void Clouds::Render(const Camera& camera, glm::vec3 pos)
{
    m_shader.useProgram();
    m_cloud.bindVAO();
    m_cloudTexture.bindTexture();

	m_movement += g_Info.deltaTime;

	if (m_movement > 1024) {
		m_movement = 0;
	}
	if (m_oldPos == glm::vec3(0.0f) && pos != glm::vec3(0.0f)) {
		m_oldPos = pos;
	}

	if (abs(pos.x - m_oldPos.x) > 1024) {
		m_oldPos.x = pos.x;
	}

	if (abs(pos.z - m_oldPos.z) > 1024) {
		m_oldPos.z = pos.z;
	}

    m_shader.loadModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(m_oldPos.x, 0, m_oldPos.z + m_movement)));
    m_shader.loadProjectionViewMatrix(camera.getProjectionViewMatrix());

    m_shader.loadLighting(g_Info.lighting);

    GL::drawElements(m_cloud.getIndicesCount());
}
