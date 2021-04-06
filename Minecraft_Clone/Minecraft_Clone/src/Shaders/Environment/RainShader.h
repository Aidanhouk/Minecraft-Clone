#pragma once

#include "../BasicShader.h"

class RainShader : public BasicShader
{
public:
	RainShader();

	void loadLighting(float light);
private:
	void getUniforms() override;
	GLuint lighting;
};