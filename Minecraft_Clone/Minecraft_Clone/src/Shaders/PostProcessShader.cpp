#include "PostProcessShader.h"

PostProcessShader::PostProcessShader()
	: BasicShader ("PostProcess/PostProcess", "PostProcess/PostProcess")
{
    getUniforms();
}

void PostProcessShader::loadContrast(float c)
{
    loadFloat(m_contrast, c);
}

void PostProcessShader::loadGamma(float g)
{
    loadFloat(m_gamma, g);
}

void PostProcessShader::loadBrightness(float b)
{
	loadFloat(m_brightness, b);
}

void PostProcessShader::loadSaturation(float b)
{
	loadFloat(m_saturation, b);
}

void PostProcessShader::loadValue(float b)
{
	loadFloat(m_value, b);
}

void PostProcessShader::loadResolution(glm::vec2 resolution)
{
	loadVector2(m_resolution, resolution);
}

void PostProcessShader::loadUnderwater(bool underwater)
{
	if (underwater)
		loadFloat(m_underwater, 0.4f);
	else
		loadFloat(m_underwater, 1.0f);
}

void PostProcessShader::loadDarkScreen(float darkScreen)
{
	loadFloat(m_darkScreen, darkScreen);
}

void PostProcessShader::getUniforms()
{
	BasicShader::getUniforms();
	m_contrast = glGetUniformLocation(m_id, "contrast");
	m_gamma = glGetUniformLocation(m_id, "gamma");
	m_brightness = glGetUniformLocation(m_id, "brightness");
	m_saturation = glGetUniformLocation(m_id, "saturation");
	m_value = glGetUniformLocation(m_id, "value");

	m_resolution = glGetUniformLocation(m_id, "resolution");
	m_underwater = glGetUniformLocation(m_id, "underwater");
	m_darkScreen = glGetUniformLocation(m_id, "darkScreen");
}
