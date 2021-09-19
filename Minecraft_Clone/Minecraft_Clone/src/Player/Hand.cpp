#include "Hand.h"

#include "World/Block/BlockDatabase.h"

#include <iostream>

const std::vector<GLfloat> idleVertexCoords{
	// left
	0.40f, -1.20f, -1.0f, // 0 left-bottom
	0.60f, -1.10f, -0.7f, // 1 rigth-bottom
	0.53f, -0.40f, -0.7f, // 2 right-top
	0.40f, -0.50f, -1.0f, // 3 left-top
	// right
	0.60f, -1.10f, -0.7f, // 4 left-bottom
	0.85f, -1.25f, -1.0f, // 5 right-bottom
	0.70f, -0.55f, -1.0f, // 6 right-top
	0.53f, -0.40f, -0.7f, // 7 left-top
};
const std::vector<GLuint> indices{
	0, 1, 2,
	2, 3, 0,

	4, 5, 6,
	6, 7, 4,
};

const float	m_animationTime = 1.0f / 5;
const unsigned int m_animationFrames = 12;
const std::array<std::vector<GLfloat>, m_animationFrames> breakingVertexCoords{
	// 1
	std::vector<GLfloat> {
	// left
	0.50f, -1.20f, -1.0f, // 0 left-bottom
	0.90f, -1.10f, -0.7f, // 1 rigth-bottom
	0.20f,  0.10f, -0.7f, // 2 right-top
	0.10f, -0.10f, -1.0f, // 3 left-top
	// right
	0.90f, -1.10f, -0.7f, // 4 left-bottom
	0.90f, -1.10f, -1.0f, // 5 right-bottom
	0.20f,  0.10f, -1.0f, // 6 right-top
	0.20f,  0.10f, -0.7f, // 7 left-top
		},
	// 2
	std::vector<GLfloat> {
	// left
	0.50f, -1.20f, -1.0f, // 0 left-bottom
	0.75f, -1.00f, -0.7f, // 1 rigth-bottom
	0.12f,  0.08f, -0.7f, // 2 right-top
	0.00f, -0.12f, -1.0f, // 3 left-top
	// right
	0.75f, -1.00f, -0.7f, // 4 left-bottom
	0.75f, -1.00f, -1.0f, // 5 right-bottom
	0.12f,  0.08f, -1.0f, // 6 right-top
	0.12f,  0.08f, -0.7f, // 7 left-top
		},
	// 3
	std::vector<GLfloat> {
	// left
	0.47f, -1.30f, -1.0f, // 0 left-bottom
	0.67f, -1.00f, -0.7f, // 1 rigth-bottom
	0.09f, -0.02f, -0.7f, // 2 right-top
	-0.03f, -0.22f, -1.0f, // 3 left-top
	// right
	0.67f, -1.00f, -0.7f, // 4 left-bottom
	0.67f, -1.00f, -1.0f, // 5 right-bottom
	0.09f, -0.02f, -1.0f, // 6 right-top
	0.09f, -0.02f, -0.7f, // 7 left-top
		},
	// 4
	std::vector<GLfloat> {
	// left
	0.45f, -1.50f, -1.0f, // 0 left-bottom
	0.65f, -1.20f, -0.7f, // 1 rigth-bottom
	0.07f, -0.22f, -0.7f, // 2 right-top
	-0.05f, -0.42f, -1.0f, // 3 left-top
	// right
	0.65f, -1.20f, -0.7f, // 4 left-bottom
	0.65f, -1.20f, -1.0f, // 5 right-bottom
	0.07f, -0.22f, -1.0f, // 6 right-top
	0.07f, -0.22f, -0.7f, // 7 left-top
		},
	// 5
	std::vector<GLfloat> {
	// left
	0.45f, -1.73f, -1.0f, // 0 left-bottom
	0.65f, -1.43f, -0.7f, // 1 rigth-bottom
	0.07f, -0.45f, -0.7f, // 2 right-top
	-0.05f, -0.65f, -1.0f, // 3 left-top
	// right
	0.65f, -1.43f, -0.7f, // 4 left-bottom
	0.65f, -1.43f, -1.0f, // 5 right-bottom
	0.07f, -0.45f, -1.0f, // 6 right-top
	0.07f, -0.45f, -0.7f, // 7 left-top
		},
	// 6
	std::vector<GLfloat> {
	// left
	0.45f, -1.83f, -1.0f, // 0 left-bottom
	0.65f, -1.53f, -0.7f, // 1 rigth-bottom
	0.07f, -0.55f, -0.7f, // 2 right-top
	-0.05f, -0.75f, -1.0f, // 3 left-top
	// right
	0.65f, -1.53f, -0.7f, // 4 left-bottom
	0.70f, -1.58f, -1.0f, // 5 right-bottom
	0.12f, -0.60f, -1.0f, // 6 right-top
	0.07f, -0.55f, -0.7f, // 7 left-top
		},
	// 7
	std::vector<GLfloat> {
	// left
	0.47f, -1.90f, -1.0f, // 0 left-bottom
	0.67f, -1.60f, -0.7f, // 1 rigth-bottom
	0.09f, -0.62f, -0.7f, // 2 right-top
	-0.03f, -0.82f, -1.0f, // 3 left-top
	// right
	0.67f, -1.60f, -0.7f, // 4 left-bottom
	0.72f, -1.65f, -1.0f, // 5 right-bottom
	0.14f, -0.67f, -1.0f, // 6 right-top
	0.09f, -0.62f, -0.7f, // 7 left-top
		},
	// 8
	std::vector<GLfloat> {
	// left
	0.49f, -1.93f, -1.0f, // 0 left-bottom
	0.69f, -1.63f, -0.7f, // 1 rigth-bottom
	0.11f, -0.65f, -0.7f, // 2 right-top
	-0.01f, -0.85f, -1.0f, // 3 left-top
	// right
	0.69f, -1.63f, -0.7f, // 4 left-bottom
	0.74f, -1.68f, -1.0f, // 5 right-bottom
	0.16f, -0.70f, -1.0f, // 6 right-top
	0.11f, -0.65f, -0.7f, // 7 left-top
		},
	// 9
	std::vector<GLfloat> {
	// left
	0.52f, -1.95f, -1.0f, // 0 left-bottom
	0.72f, -1.65f, -0.7f, // 1 rigth-bottom
	0.14f, -0.67f, -0.7f, // 2 right-top
	0.04f, -0.87f, -1.0f, // 3 left-top
	// right
	0.72f, -1.65f, -0.7f, // 4 left-bottom
	0.77f, -1.70f, -1.0f, // 5 right-bottom
	0.19f, -0.72f, -1.0f, // 6 right-top
	0.14f, -0.67f, -0.7f, // 7 left-top
		},
	// 10
	std::vector<GLfloat> {
	// left
	0.62f, -1.95f, -1.0f, // 0 left-bottom
	0.82f, -1.65f, -0.7f, // 1 rigth-bottom
	0.24f, -0.67f, -0.7f, // 2 right-top
	0.14f, -0.87f, -1.0f, // 3 left-top
	// right
	0.82f, -1.65f, -0.7f, // 4 left-bottom
	0.87f, -1.70f, -1.0f, // 5 right-bottom
	0.29f, -0.72f, -1.0f, // 6 right-top
	0.24f, -0.67f, -0.7f, // 7 left-top
		},
	// 11
	std::vector<GLfloat> {
	// left
	0.72f, -1.88f, -1.0f, // 0 left-bottom
	0.92f, -1.58f, -0.7f, // 1 rigth-bottom
	0.34f, -0.60f, -0.7f, // 2 right-top
	0.24f, -0.80f, -1.0f, // 3 left-top
	// right
	0.92f, -1.58f, -0.7f, // 4 left-bottom
	1.02f, -1.68f, -1.0f, // 5 right-bottom
	0.44f, -0.70f, -1.0f, // 6 right-top
	0.34f, -0.60f, -0.7f, // 7 left-top
		},
	// 12
	std::vector<GLfloat> {
	// left
	0.40f, -1.20f, -1.0f, // 0 left-bottom
	0.60f, -1.10f, -0.7f, // 1 rigth-bottom
	0.53f, -0.40f, -0.7f, // 2 right-top
	0.40f, -0.50f, -1.0f, // 3 left-top
	// right
	0.60f, -1.10f, -0.7f, // 4 left-bottom
	0.85f, -1.25f, -1.0f, // 5 right-bottom
	0.70f, -0.55f, -1.0f, // 6 right-top
	0.53f, -0.40f, -0.7f, // 7 left-top
		},
};

Hand::Hand()
	:m_state{ HandState::IDLE }, m_leftMouseHolded{ false }
{
}

void Hand::_break()
{
	if (m_state != HandState::BREAKING) {
		m_state = HandState::BREAKING;
		m_animationTimer.restart();
	}
}

void Hand::swing()
{
	if (m_state != HandState::BREAKING && m_leftMouseHolded == false) {
		m_state = HandState::BREAKING;
		m_animationTimer.restart();
	}
}

void Hand::setMeshToDraw()
{
	m_model.deleteData();

	std::array<GLfloat, 8> textureCoordsArray1 =
		BlockDatabase::get().textureAtlas.getTextureCoords({ 1, 15 });
	std::array<GLfloat, 8> textureCoordsArray2 =
		BlockDatabase::get().textureAtlas.getTextureCoords({ 2, 15 });

	std::vector<GLfloat> textureCoords;
	textureCoords.insert(textureCoords.end(), textureCoordsArray1.begin(), textureCoordsArray1.end());
	textureCoords.insert(textureCoords.end(), textureCoordsArray2.begin(), textureCoordsArray2.end());

	if (m_state == HandState::IDLE) {
		m_model.addData({ idleVertexCoords, textureCoords, indices });
	}
	else {
		bool animationFinished = false;
		for (int i = 1; i <= m_animationFrames; ++i) {
			if (m_animationTimer.getElapsedTime().asSeconds() < i * m_animationTime / m_animationFrames) {
				m_model.addData({ breakingVertexCoords[i - 1], textureCoords, indices });
				animationFinished = true;
				break;
			}
		}
		if (!animationFinished) {
			m_state = HandState::IDLE;
			m_model.addData({ idleVertexCoords, textureCoords, indices });
		}
	}
}