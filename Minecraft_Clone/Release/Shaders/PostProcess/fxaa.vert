#version 330

layout(location = 0) in vec3 inVertexPosition;
layout(location = 1) in vec2 inTextureCoord;

out vec2 passTextureCoord;

uniform mat4 projViewMatrix;
uniform mat4 modelMatrix;

void main()
{
	gl_Position = vec4(inVertexPosition.x, inVertexPosition.y, 0.0, 1.0);
    passTextureCoord = inTextureCoord;
}
