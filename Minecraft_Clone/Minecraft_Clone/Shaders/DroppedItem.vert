#version 330

layout(location = 0) in vec3 inVertexPosition;
layout(location = 1) in vec2 inTextureCoord;

out vec2 passTextureCoord;

uniform mat4 projViewMatrix;
//uniform mat4 modelMatrix;
uniform float globalTime;

vec4 getWorldPos()
{
	vec3 inVert = inVertexPosition.xyz;
	if (inTextureCoord.y < 0.9)
		inVert.y += sin(globalTime / 8.0f) / 20.0f;
	
	return vec4(inVert, 1.0);
}

void main()
{
	gl_Position = projViewMatrix * getWorldPos();
	//gl_Position = projViewMatrix * modelMatrix * getWorldPos();
	
	passTextureCoord = inTextureCoord;
}