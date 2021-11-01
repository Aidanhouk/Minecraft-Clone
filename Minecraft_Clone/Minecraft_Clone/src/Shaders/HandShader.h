#pragma once

#include "BasicShader.h"

class HandShader : public BasicShader {
public:
	HandShader();

	void loadLighting(float light);
	void loadTime(float time);
private:
	void getUniforms() override;
	GLuint m_lighting;
	GLuint m_time;
};