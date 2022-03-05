#version 330

layout(location = 0) in vec3 inVertexPosition;
layout(location = 1) in vec2 inTextureCoord;
layout(location = 2) in float inTorchLight;
layout(location = 3) in float inSunLight;

out vec2 passTextureCoord;
out float passCardinalLight;
out float visibility;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform float globalTime;
uniform int fog;
uniform float fogDensity;
uniform float lighting;

const float gradient = 10.0;
const float MIN_LIGHT_VALUE = 0.1f;
const float	LIGHT_COEF = 0.06f;
//const float HALF_SIZE = 0.15f;

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
	passCardinalLight = max(inTorchLight, inSunLight * lighting);
	passCardinalLight = passCardinalLight * LIGHT_COEF + MIN_LIGHT_VALUE;
	
	if (fog == 1) {
		float distance = length(positionRelativeToCam.xyz);
		visibility = exp(-pow((distance * fogDensity), gradient));
		visibility = clamp(visibility, 0.0, 1.0);
	}
}