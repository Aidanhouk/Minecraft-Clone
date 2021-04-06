#pragma once

#include "Shader.h"

class Basic_2D_Shader : public Shader {
public:
	Basic_2D_Shader(const std::string &vertexFile = "2D_Basic",
		const std::string &fragmentFile = "Basic");

protected:
	virtual void getUniforms() override;
};