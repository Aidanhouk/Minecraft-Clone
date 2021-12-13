#version 330

layout(location = 0) in vec3  inVertexPosition;
layout(location = 1) in vec2  inTextureCoord;
layout(location = 2) in float inAmbientOcclusion;
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

const float gradient = 10.0f;
const float MIN_LIGHT_VALUE = 0.1f;
const float	LIGHT_COEF = 0.06f;

vec4 getWorldPos()
{
	vec3 inVert = inVertexPosition.xyz;
	inVert.x += sin((globalTime + inVert.z + inVert.y) * 1.8f) / 15.0f;
	inVert.z -= cos((globalTime + inVert.x + inVert.y) * 1.8f) / 15.0f;
	
	return vec4(inVert, 1);
}

void main()
{
	vec4 positionRelativeToCam = viewMatrix * getWorldPos();
	gl_Position = projMatrix * positionRelativeToCam;
	passTextureCoord = inTextureCoord;
	passCardinalLight = max(inTorchLight, inSunLight * lighting);
	passCardinalLight = (passCardinalLight * LIGHT_COEF + MIN_LIGHT_VALUE) * inAmbientOcclusion;

	if (fog == 1) {
		float distance = length(positionRelativeToCam.xyz);
		passVisibility = exp(-pow((distance * fogDensity), gradient));
		passVisibility = clamp(passVisibility, 0.0, 1.0);
	}
}