#pragma once

#include "GL/glew.h"
#include "SFML/System.hpp"

#include <array>

using FaceCoords = std::array<GLfloat, 12>;

const float	ANIMATION_TIME =
// 0.2 sec
1.0f / 5;
//1.0f;

namespace {

	constexpr GLfloat LIGHT_TOP = 1.0f;
	constexpr GLfloat LIGHT_RIGHT = 0.85f;
	constexpr GLfloat LIGHT_FRONT = 0.95f;
	constexpr GLfloat LIGHT_BOT = 0.8f;

} // namespace

const sf::Vector2i HAND_TEXTURE_1_COORDS{ 1, 15 };
const sf::Vector2i HAND_TEXTURE_2_COORDS{ 2, 15 };

namespace emptyHandData {
	const FaceCoords leftFace{
		0.0f, -1.0f, -1.0f, // 0 left-bottom
		0.2f, -1.0f, -1.0f, // 1 rigth-bottom
		0.2f,  0.0f, -1.0f, // 2 right-top
		0.0f,  0.0f, -1.0f, // 3 left-top
	};
	const FaceCoords rightFace{
		0.2f, -1.0f, -1.0f, // 0 left-bottom
		0.2f, -1.0f, -1.2f, // 1 rigth-bottom
		0.2f,  0.0f, -1.2f, // 2 right-top
		0.2f,  0.0f, -1.0f, // 3 left-top
	};
}

namespace defaultItemData {
	const FaceCoords mainFace{
		0.0f, -1.0f, -1.0f, // 0 left-bottom
		1.0f, -1.0f, -1.0f, // 1 rigth-bottom
		1.0f,  0.0f, -1.0f, // 2 right-top
		0.0f,  0.0f, -1.0f, // 3 left-top
	};

	const float PIXEL_SIZE = 1.0f / 16.0f;

	const FaceCoords sideFaceLeft{
		0.0f, 0.0f,			0.0f,
		0.0f, 0.0f,			PIXEL_SIZE,
		0.0f, PIXEL_SIZE,	PIXEL_SIZE,
		0.0f, PIXEL_SIZE,	0.0f,
	};
	const FaceCoords sideFaceRight{
		PIXEL_SIZE,	0.0f,		PIXEL_SIZE,
		PIXEL_SIZE,	0.0f,		0.0f,
		PIXEL_SIZE,	PIXEL_SIZE,	0.0f,
		PIXEL_SIZE,	PIXEL_SIZE,	PIXEL_SIZE,
	};
	const FaceCoords sideFaceTop{
		0.0f,		PIXEL_SIZE,	PIXEL_SIZE,
		PIXEL_SIZE,	PIXEL_SIZE,	PIXEL_SIZE,
		PIXEL_SIZE,	PIXEL_SIZE,	0.0f,
		0.0f,		PIXEL_SIZE,	0.0f,
	};
	const FaceCoords sideFaceBottom{
		0.0f,		0.0f, 0.0f,
		PIXEL_SIZE,	0.0f, 0.0f,
		PIXEL_SIZE,	0.0f, PIXEL_SIZE,
		0.0f,		0.0f, PIXEL_SIZE,
	};
}

namespace cubeData {
	const float scale = 0.5f;

	const FaceCoords frontFace{
		0.0f * scale, -1.0f * scale, 0.0f * scale, // 0 left-bottom
		1.0f * scale, -1.0f * scale, 0.0f * scale, // 1 rigth-bottom
		1.0f * scale,  0.0f * scale, 0.0f * scale, // 2 right-top
		0.0f * scale,  0.0f * scale, 0.0f * scale, // 3 left-top
	};
	const FaceCoords backFace{
		1.0f * scale, -1.0f * scale, -1.0f * scale, // 0 left-bottom
		0.0f * scale, -1.0f * scale, -1.0f * scale, // 1 rigth-bottom
		0.0f * scale,  0.0f * scale, -1.0f * scale, // 2 right-top
		1.0f * scale,  0.0f * scale, -1.0f * scale, // 3 left-top
	};
	const FaceCoords rightFace{
		1.0f * scale, -1.0f * scale, -0.0f * scale, // 0 left-bottom
		1.0f * scale, -1.0f * scale, -1.0f * scale, // 1 rigth-bottom
		1.0f * scale,  0.0f * scale, -1.0f * scale, // 2 right-top
		1.0f * scale,  0.0f * scale, -0.0f * scale, // 3 left-top
	};
	const FaceCoords leftFace{
		0.0f * scale, -1.0f * scale, -1.0f * scale, // 0 left-bottom
		0.0f * scale, -1.0f * scale, -0.0f * scale, // 1 rigth-bottom
		0.0f * scale,  0.0f * scale, -0.0f * scale, // 2 right-top
		0.0f * scale,  0.0f * scale, -1.0f * scale, // 3 left-top
	};
	const FaceCoords topFace{
		0.0f * scale,  0.0f * scale, -0.0f * scale, // 0 left-bottom
		1.0f * scale,  0.0f * scale, -0.0f * scale, // 1 rigth-bottom
		1.0f * scale,  0.0f * scale, -1.0f * scale, // 2 right-top
		0.0f * scale,  0.0f * scale, -1.0f * scale, // 3 left-top
	};
	const FaceCoords bottomFace{
		0.0f * scale, -1.0f * scale, -0.0f * scale, // 0 left-bottom
		1.0f * scale, -1.0f * scale, -0.0f * scale, // 1 rigth-bottom
		1.0f * scale, -1.0f * scale, -1.0f * scale, // 2 right-top
		0.0f * scale, -1.0f * scale, -1.0f * scale, // 3 left-top
	};
}

namespace cactusData {
	const float bias = 1.0f / 16.0f;
	const float scale = cubeData::scale;

	const FaceCoords frontFace{
		0.0f * scale, -1.0f * scale, (0.0f - bias) * scale, // 0 left-bottom
		1.0f * scale, -1.0f * scale, (0.0f - bias) * scale, // 1 rigth-bottom
		1.0f * scale,  0.0f * scale, (0.0f - bias) * scale, // 2 right-top
		0.0f * scale,  0.0f * scale, (0.0f - bias) * scale, // 3 left-top
	};
	const FaceCoords rightFace{
		(1.0f - bias) * scale, -1.0f * scale,  0.0f * scale, // 0 left-bottom
		(1.0f - bias) * scale, -1.0f * scale, -1.0f * scale, // 1 rigth-bottom
		(1.0f - bias) * scale,  0.0f * scale, -1.0f * scale, // 2 right-top
		(1.0f - bias) * scale,  0.0f * scale,  0.0f * scale, // 3 left-top
	};
	const FaceCoords leftFace{
		(0.0f + bias) * scale, -1.0f * scale, (-1.0f + bias) * scale, // 0 left-bottom
		(0.0f + bias) * scale, -1.0f * scale, ( 0.0f - bias) * scale, // 1 rigth-bottom
		(0.0f + bias) * scale,  0.0f * scale, ( 0.0f - bias) * scale, // 2 right-top
		(0.0f + bias) * scale,  0.0f * scale, (-1.0f + bias) * scale, // 3 left-top
	};
	const FaceCoords topFace{
		0.0f * scale, 0.0f * scale,  0.0f * scale, // 0 left-bottom
		1.0f * scale, 0.0f * scale,  0.0f * scale, // 1 rigth-bottom
		1.0f * scale, 0.0f * scale, -1.0f * scale, // 2 right-top
		0.0f * scale, 0.0f * scale, -1.0f * scale, // 3 left-top
	};
	const FaceCoords bottomFace{
		0.0f * scale, -1.0f * scale,  0.0f * scale, // 0 left-bottom
		1.0f * scale, -1.0f * scale,  0.0f * scale, // 1 rigth-bottom
		1.0f * scale, -1.0f * scale, -1.0f * scale, // 2 right-top
		0.0f * scale, -1.0f * scale, -1.0f * scale, // 3 left-top
	};
}