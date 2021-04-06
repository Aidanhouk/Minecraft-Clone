#version 330

layout(location = 0) in vec3 inVertexPosition;
layout(location = 1) in vec2 inTextureCoord;
layout(location = 2) in vec2 inDirections;

out vec2 passTextureCoord;

uniform mat4 projViewMatrix;
uniform mat4 modelMatrix;
uniform float time;

void main()
{
    vec4 worldPos = modelMatrix * vec4(inVertexPosition, 1.0);
    gl_Position = projViewMatrix * worldPos;

    passTextureCoord = vec2(
	inTextureCoord.x + inDirections.x * time,
	inTextureCoord.y + inDirections.y * time);
}
