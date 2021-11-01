#pragma once

#include "../BasicShader.h"

class ProcSkyShader : public BasicShader{
public:
    ProcSkyShader();
    void loadTime(float ticks);
	void loadLight(float light);
    void loadViewMatrix         (glm::mat4 viewMatrix);
    void loadProjectionMatrix   (const glm::mat4& proj);

private:
    void getUniforms() override;
    GLuint m_day;
    GLuint m_locationProjection;
    GLuint m_locationView;
	GLfloat m_light;
};