#pragma once

#include "BasicShader.h"

class FloraShader : public BasicShader {
  public:
    FloraShader();

    void loadTime(float time);
	void loadLight(float lighting);
	void loadDTime(float time);
	void loadFog(bool fog);
	void loadFogDensity(float density);
  private:
    void getUniforms() override;
    GLuint m_time;
	GLuint m_light;
	GLuint m_dtime;
	GLboolean m_fog;
	GLfloat m_fogDensity;
};