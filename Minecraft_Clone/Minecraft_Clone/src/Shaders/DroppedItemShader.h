#pragma once

#include "BasicShader.h"

class DroppedItemShader : public BasicShader {
public:
	DroppedItemShader();
	void loadTime(const float &time);
	void loadLighting(float light);
private:
	void getUniforms() override;
	GLuint m_time;
	GLuint m_lighting;
};