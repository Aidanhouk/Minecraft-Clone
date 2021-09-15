#pragma once

#include "../BasicShader.h"

class DelineateShader : public BasicShader {
public:
	DelineateShader();

private:
	void getUniforms() override;
};