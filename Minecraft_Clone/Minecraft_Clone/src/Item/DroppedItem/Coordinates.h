#pragma once

#include "GL/glew.h"

#include <array>

const float HALF_SIZE = 0.15f;

namespace cubeFaces {

	const std::array<GLfloat, 12> frontFace{
		-HALF_SIZE, -HALF_SIZE, +HALF_SIZE,
		+HALF_SIZE, -HALF_SIZE, +HALF_SIZE,
		+HALF_SIZE, +HALF_SIZE, +HALF_SIZE,
		-HALF_SIZE, +HALF_SIZE, +HALF_SIZE,
	};
	const std::array<GLfloat, 12> backFace{
		+HALF_SIZE, -HALF_SIZE, -HALF_SIZE,
		-HALF_SIZE, -HALF_SIZE, -HALF_SIZE,
		-HALF_SIZE, +HALF_SIZE, -HALF_SIZE,
		+HALF_SIZE, +HALF_SIZE, -HALF_SIZE,
	};
	const std::array<GLfloat, 12> leftFace{
		-HALF_SIZE, -HALF_SIZE, -HALF_SIZE,
		-HALF_SIZE, -HALF_SIZE, +HALF_SIZE,
		-HALF_SIZE, +HALF_SIZE, +HALF_SIZE,
		-HALF_SIZE, +HALF_SIZE, -HALF_SIZE,
	};
	const std::array<GLfloat, 12> rightFace{
		+HALF_SIZE, -HALF_SIZE, +HALF_SIZE,
		+HALF_SIZE, -HALF_SIZE, -HALF_SIZE,
		+HALF_SIZE, +HALF_SIZE, -HALF_SIZE,
		+HALF_SIZE, +HALF_SIZE, +HALF_SIZE,
	};
	const std::array<GLfloat, 12> topFace{
		-HALF_SIZE, +HALF_SIZE, +HALF_SIZE,
		+HALF_SIZE, +HALF_SIZE, +HALF_SIZE,
		+HALF_SIZE, +HALF_SIZE, -HALF_SIZE,
		-HALF_SIZE, +HALF_SIZE, -HALF_SIZE,
	};
	const std::array<GLfloat, 12> bottomFace{
		-HALF_SIZE, -HALF_SIZE, -HALF_SIZE,
		+HALF_SIZE, -HALF_SIZE, -HALF_SIZE,
		+HALF_SIZE, -HALF_SIZE, +HALF_SIZE,
		-HALF_SIZE, -HALF_SIZE, +HALF_SIZE,
	};
} // namespace cubeFaces

namespace xBlockFaces {

	const std::array<GLfloat, 12> xFace1{
		-HALF_SIZE, -HALF_SIZE, -HALF_SIZE,
		+HALF_SIZE, -HALF_SIZE, +HALF_SIZE,
		+HALF_SIZE, +HALF_SIZE, +HALF_SIZE,
		-HALF_SIZE, +HALF_SIZE, -HALF_SIZE,
	};
	const std::array<GLfloat, 12> xFace2{
		-HALF_SIZE, -HALF_SIZE, +HALF_SIZE,
		+HALF_SIZE, -HALF_SIZE, -HALF_SIZE,
		+HALF_SIZE, +HALF_SIZE, -HALF_SIZE,
		-HALF_SIZE, +HALF_SIZE, +HALF_SIZE,
	};
} // namespace xBlockFaces

namespace cactusFaces {

	const float bias = 1 / 16.0f * 0.3f;

	const std::array<GLfloat, 12> frontFace{
		-HALF_SIZE, -HALF_SIZE, +HALF_SIZE - bias,
		+HALF_SIZE, -HALF_SIZE, +HALF_SIZE - bias,
		+HALF_SIZE, +HALF_SIZE, +HALF_SIZE - bias,
		-HALF_SIZE, +HALF_SIZE, +HALF_SIZE - bias,
	};
	const std::array<GLfloat, 12> backFace{
		+HALF_SIZE, -HALF_SIZE, -HALF_SIZE + bias,
		-HALF_SIZE, -HALF_SIZE, -HALF_SIZE + bias,
		-HALF_SIZE, +HALF_SIZE, -HALF_SIZE + bias,
		+HALF_SIZE, +HALF_SIZE, -HALF_SIZE + bias,
	};
	const std::array<GLfloat, 12> leftFace{
		-HALF_SIZE + bias, -HALF_SIZE, -HALF_SIZE,
		-HALF_SIZE + bias, -HALF_SIZE, +HALF_SIZE,
		-HALF_SIZE + bias, +HALF_SIZE, +HALF_SIZE,
		-HALF_SIZE + bias, +HALF_SIZE, -HALF_SIZE,
	};
	const std::array<GLfloat, 12> rightFace{
		+HALF_SIZE - bias, -HALF_SIZE, +HALF_SIZE,
		+HALF_SIZE - bias, -HALF_SIZE, -HALF_SIZE,
		+HALF_SIZE - bias, +HALF_SIZE, -HALF_SIZE,
		+HALF_SIZE - bias, +HALF_SIZE, +HALF_SIZE,
	};
	const std::array<GLfloat, 12> topFace{
		-HALF_SIZE, +HALF_SIZE, +HALF_SIZE,
		+HALF_SIZE, +HALF_SIZE, +HALF_SIZE,
		+HALF_SIZE, +HALF_SIZE, -HALF_SIZE,
		-HALF_SIZE, +HALF_SIZE, -HALF_SIZE,
	};
	const std::array<GLfloat, 12> bottomFace{
		-HALF_SIZE, -HALF_SIZE, -HALF_SIZE,
		+HALF_SIZE, -HALF_SIZE, -HALF_SIZE,
		+HALF_SIZE, -HALF_SIZE, +HALF_SIZE,
		-HALF_SIZE, -HALF_SIZE, +HALF_SIZE,
	};
} // namespace cactusFaces

namespace defaultItemFaces {

	const float ENLARGEMENT_COEF = 1.7f;
	const float PIXEL_SIZE = ENLARGEMENT_COEF * 2 * (HALF_SIZE / 16.0f);

	const std::array<GLfloat, 12> mainFace1{
		-HALF_SIZE * ENLARGEMENT_COEF, -HALF_SIZE * ENLARGEMENT_COEF, PIXEL_SIZE / 2.0f, // 0
		+HALF_SIZE * ENLARGEMENT_COEF, -HALF_SIZE * ENLARGEMENT_COEF, PIXEL_SIZE / 2.0f, // 1
		+HALF_SIZE * ENLARGEMENT_COEF, +HALF_SIZE * ENLARGEMENT_COEF, PIXEL_SIZE / 2.0f, // 2
		-HALF_SIZE * ENLARGEMENT_COEF, +HALF_SIZE * ENLARGEMENT_COEF, PIXEL_SIZE / 2.0f, // 3
	};
	const std::array<GLfloat, 12> mainFace2{
		-HALF_SIZE * ENLARGEMENT_COEF, -HALF_SIZE * ENLARGEMENT_COEF, -PIXEL_SIZE / 2.0f, // 0
		+HALF_SIZE * ENLARGEMENT_COEF, -HALF_SIZE * ENLARGEMENT_COEF, -PIXEL_SIZE / 2.0f, // 1
		+HALF_SIZE * ENLARGEMENT_COEF, +HALF_SIZE * ENLARGEMENT_COEF, -PIXEL_SIZE / 2.0f, // 2
		-HALF_SIZE * ENLARGEMENT_COEF, +HALF_SIZE * ENLARGEMENT_COEF, -PIXEL_SIZE / 2.0f, // 3
	};

	const std::array<GLfloat, 12> sideFaceLeft{
		0.0f, 0.0f,			0.0f,
		0.0f, 0.0f,			PIXEL_SIZE,
		0.0f, PIXEL_SIZE,	PIXEL_SIZE,
		0.0f, PIXEL_SIZE,	0.0f,
	};
	const std::array<GLfloat, 12> sideFaceRight{
		PIXEL_SIZE,	0.0f,		PIXEL_SIZE,
		PIXEL_SIZE,	0.0f,		0.0f,
		PIXEL_SIZE,	PIXEL_SIZE,	0.0f,
		PIXEL_SIZE,	PIXEL_SIZE,	PIXEL_SIZE,
	};
	const std::array<GLfloat, 12> sideFaceTop{
		0.0f,		PIXEL_SIZE,	PIXEL_SIZE,
		PIXEL_SIZE,	PIXEL_SIZE,	PIXEL_SIZE,
		PIXEL_SIZE,	PIXEL_SIZE,	0.0f,
		0.0f,		PIXEL_SIZE,	0.0f,
	};
	const std::array<GLfloat, 12> sideFaceBottom{
		0.0f,		0.0f, 0.0f,
		PIXEL_SIZE,	0.0f, 0.0f,
		PIXEL_SIZE,	0.0f, PIXEL_SIZE,
		0.0f,		0.0f, PIXEL_SIZE,
	};
} // namespace xBlockFaces

namespace {

	const std::array<GLfloat, 12> shadow{
		-HALF_SIZE, -1.7 * HALF_SIZE, -HALF_SIZE,
		+HALF_SIZE, -1.7 * HALF_SIZE, -HALF_SIZE,
		+HALF_SIZE, -1.7 * HALF_SIZE, +HALF_SIZE,
		-HALF_SIZE, -1.7 * HALF_SIZE, +HALF_SIZE,
	};
} // namespace
