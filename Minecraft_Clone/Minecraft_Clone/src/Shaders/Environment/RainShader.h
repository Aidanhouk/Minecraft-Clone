#pragma once

#include "../BasicShader.h"

class RainShader : public BasicShader
{
public:
	RainShader();

	void loadLighting(float light);
	void loadVisibility(float visibility);
private:
	void getUniforms() override;
	GLuint lighting;
	GLfloat m_visibility;
};