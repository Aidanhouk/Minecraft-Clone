#pragma once

#include "../BasicShader.h"

class SnowShader : public BasicShader
{
public:
	SnowShader();

	void loadTime(float time);
	void loadLighting(float light);
private:
	void getUniforms() override;
	GLuint moveTime;
	GLuint lighting;
};