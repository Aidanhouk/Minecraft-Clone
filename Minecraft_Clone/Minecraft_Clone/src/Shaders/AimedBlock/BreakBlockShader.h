#pragma once

#include "../BasicShader.h"

class BreakBlockShader : public BasicShader {
public:
	BreakBlockShader();

	void loadLighting(float light);
private:
	void getUniforms() override;
	GLuint lighting;
};