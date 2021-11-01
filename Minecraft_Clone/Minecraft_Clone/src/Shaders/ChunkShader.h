#pragma once

#include "BasicShader.h"

class ChunkShader : public BasicShader {
  public:
    ChunkShader();

	void loadLighting(float light);
	void loadDTime(float dayTime);
	void loadFog(bool fog);
	void loadFogDensity(float density);
  private:
    void getUniforms() override;
	GLuint m_lighting;
	GLuint m_dtime;
	GLboolean m_fog;
	GLfloat m_fogDensity;
};