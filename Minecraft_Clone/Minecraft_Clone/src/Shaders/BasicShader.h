#pragma once

#include "Shader.h"

class BasicShader : public Shader {
  public:
    BasicShader(const std::string &vertexFile = "Basic",
                const std::string &fragmentFile = "Basic");

    void loadProjectionViewMatrix(const glm::mat4 &pvMatrix);
	void loadProjectionMatrix(const glm::mat4& matrix);
	void loadViewMatrix(const glm::mat4& matrix);
    void loadModelMatrix(const glm::mat4 &matrix);

  protected:
    virtual void getUniforms() override;

  private:
    GLuint m_locationProjectionViewMatrix;
	GLuint m_locationProjectionMatrix;
	GLuint m_locationViewMatrix;
    GLuint m_locationModelMatrix;
};