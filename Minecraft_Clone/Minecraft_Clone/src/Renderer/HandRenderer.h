#pragma once

#include "SFML/System.hpp"

#include "Shaders/HandShader.h"
#include "Model.h"

class HandRenderer
{
public:
	void render();

	void setHandModel(Model& model) { m_model = &model; }
private:
	HandShader m_shader;
	Model * m_model;
};