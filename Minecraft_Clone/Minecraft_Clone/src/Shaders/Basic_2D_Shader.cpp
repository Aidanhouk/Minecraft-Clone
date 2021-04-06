#include "Basic_2D_Shader.h"

Basic_2D_Shader::Basic_2D_Shader(const std::string &vertexFile,
	const std::string &fragmentFile)
	: Shader(vertexFile, fragmentFile)
{
	getUniforms();
}

void Basic_2D_Shader::getUniforms()
{
	useProgram();
}