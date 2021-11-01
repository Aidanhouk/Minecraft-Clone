#version 330

layout(location = 0) in vec3  inVertexPosition;
layout(location = 1) in vec2  inTextureCoord;
layout(location = 2) in float inCardinalLight;

out vec2 passTextureCoord;
out float passCardinalLight;

uniform mat4 projMatrix;
uniform mat4 modelMatrix;
uniform float globalTime;

vec4 getWorldPos()
{
	vec3 inVert = inVertexPosition.xyz;
	inVert.x += sin(globalTime) / 30.0f;
	inVert.y -= abs(cos(globalTime) / 10.0f);
	
	return vec4(inVert, 1.0);
}

void main()
{
	gl_Position = projMatrix * modelMatrix * getWorldPos();
	//vec4 position = projMatrix * modelMatrix * getWorldPos();
	//position /= position.w;
	//position.z = 0.0f;
	//gl_Position = position;

	passTextureCoord = inTextureCoord;
	passCardinalLight = inCardinalLight;
}
