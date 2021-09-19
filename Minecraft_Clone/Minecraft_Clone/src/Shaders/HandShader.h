#pragma once

#include "BasicShader.h"

class HandShader : public BasicShader {
public:
	HandShader();

	void loadLighting(float light);
private:
	void getUniforms() override;
	GLuint lighting;
};