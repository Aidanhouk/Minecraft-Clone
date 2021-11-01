#pragma once

#include "BasicShader.h"

class PostProcessShader : public BasicShader
{
public:
	PostProcessShader();
	
	void loadContrast(float c);
	void loadGamma(float g);
	void loadBrightness(float b);
	void loadSaturation(float b);
	void loadValue(float b);
	void loadResolution(glm::vec2 resolution);
	void loadUnderwater(bool underwater);
	void loadDarkScreen(float darkScreen);
private:
	void getUniforms() override;
	
	GLfloat m_contrast;
	GLfloat m_gamma;
	GLfloat m_brightness;
	GLfloat m_saturation;
	GLfloat m_value;
	GLuint m_resolution;
	GLuint m_underwater;
	GLuint m_darkScreen;
	GLuint m_inCave;
};