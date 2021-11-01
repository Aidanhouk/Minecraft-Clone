#pragma once

#include "../BasicShader.h"

class SnowShader : public BasicShader
{
public:
	SnowShader();

	void loadTime(float time);
	void loadLighting(float light);
	void loadVisibility(float visibility);
private:
	void getUniforms() override;
	GLuint moveTime;
	GLuint lighting;
	GLfloat m_visibility;
};