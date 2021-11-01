#pragma once

#include "BasicShader.h"

class WaterShader : public BasicShader {
  public:
    WaterShader();

    void loadTime(const float &time);
	void loadLighting(float light);
	void loadDTime(float time);
	void loadFog(bool fog);
	void loadFogDensity(float density);
  private:
    void getUniforms() override;
    GLuint m_time;
	GLuint m_lighting;
	GLuint m_dtime;
	GLboolean m_fog;
	GLfloat m_fogDensity;
};