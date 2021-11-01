#version 330

layout(location = 0) in vec3 inVertexPosition;
layout(location = 1) in vec2 inTextureCoord;

out vec2 passTextureCoord;
out float visibility;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform float globalTime;
uniform int fog;
uniform float fogDensity;

const float gradient = 10.0;

const float HALF_SIZE = 0.15f;

vec4 getWorldPos()
{
	vec3 inVert = inVertexPosition.xyz;
	if (inTextureCoord.y < 0.9)
		inVert.y += sin(globalTime / 6.0f) / 15.0f;
	//inVert.x += cos(globalTime / 10.0f) * HALF_SIZE / 3.0f;
	//inVert.z += sin(globalTime / 10.0f) * HALF_SIZE / 3.0f;
	
	return vec4(inVert, 1.0);
}

void main()
{
	vec4 positionRelativeToCam = viewMatrix * getWorldPos();
	gl_Position = projMatrix * positionRelativeToCam;
	passTextureCoord = inTextureCoord;
	
	if (fog == 1) {
		float distance = length(positionRelativeToCam.xyz);
		visibility = exp(-pow((distance * fogDensity), gradient));
		visibility = clamp(visibility, 0.0, 1.0);
	}
}