#pragma once

#include "BasicShader.h"

class DroppedItemShader : public BasicShader {
public:
	DroppedItemShader();
	void loadTime(const float &time);
	void loadLighting(float light);
	void loadFog(bool fog);
	void loadFogDensity(float density);
private:
	void getUniforms() override;
	GLuint m_time;
	GLuint m_lighting;
	GLboolean m_fog;
	GLfloat m_fogDensity;
};