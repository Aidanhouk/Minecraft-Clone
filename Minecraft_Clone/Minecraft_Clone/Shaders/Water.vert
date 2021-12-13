#version 330

layout(location = 0) in vec3  inVertexPosition;
layout(location = 1) in vec2  inTextureCoord;
//layout(location = 2) in float inAmbientOcclusion;
layout(location = 3) in float inTorchLight;
layout(location = 4) in float inSunLight;

out vec2 passTextureCoord;
out float passCardinalLight;
out float passVisibility;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform float dayTime;
uniform float globalTime;
uniform int fog;
uniform float fogDensity;
uniform float lighting;

const float PI = 3.1415927;
#define Water_WaveSpeed1 0.25	
#define Water_WaveSpeed2 0.10

const float gradient = 10.0;
const float MIN_LIGHT_VALUE = 0.1f;
const float	LIGHT_COEF = 0.06f;

vec4 getWorldPos()
{
	vec3 inVert = inVertexPosition.xyz;
	
	float fy = fract(inVert.y + 0.001);
	float wave = 0.05 * sin(2 * PI * (globalTime*2 * Water_WaveSpeed1 - inVert.x /  7.0 - inVert.z / 13.0))
				   + 0.05 * sin(2 * PI * (globalTime * Water_WaveSpeed2 - inVert.x / 11.0 - inVert.z /  5.0));
		
	inVert.y += sin((globalTime + inVert.x) * 2.3) / 64.8f;
	inVert.y += cos((globalTime + inVert.z) * 2.3) / 64.1f;
	float displacement = clamp(wave, -fy, 1.0-fy);
	float Dynamic_wavingWater = 1.0f;
	
	inVert.y += displacement * 1.0 * Dynamic_wavingWater;
	inVert.y += displacement * 1.8 * 0.25;
	inVert.y -= 0.15f;
	return vec4(inVert, 1);
}

void main()
{
	vec4 positionRelativeToCam = viewMatrix * getWorldPos();
	gl_Position = projMatrix * positionRelativeToCam;
	passTextureCoord = inTextureCoord;
	passCardinalLight = max(inTorchLight, inSunLight * lighting);
	passCardinalLight = (passCardinalLight * LIGHT_COEF + MIN_LIGHT_VALUE);
	//passCardinalLight *= inAmbientOcclusion;
	
	if (fog == 1) {
		float distance = length(positionRelativeToCam.xyz);
		passVisibility = exp(-pow((distance * fogDensity), gradient));
		passVisibility = clamp(passVisibility, 0.0, 1.0);
	}
}