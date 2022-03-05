#version 330

out vec4 outColour;
in  vec2 passTextureCoord;

uniform sampler2D texSampler;
uniform sampler2D depth;

uniform mat4 currProjViewMatrix;
uniform mat4 prevProjViewMatrix;
uniform vec2 resolution;

#define SAMPLES 8

const float nearClip = 0.1;
const float farClip = 2000.0;

void main()
{
	vec2 uv = passTextureCoord;

	float zOverw = texture(depth, uv).r / 1.0;
	//float z_n = 2.0 * zOverw - 1.0;
    //zOverw = 2.0 * nearClip * farClip / (farClip + nearClip - z_n * (farClip - nearClip));
	//zOverw = (2 * nearClip) / (farClip + nearClip - zOverw * (farClip - nearClip));
	//zOverw *= farClip;

	vec4 h = vec4(uv.x*2 - 1, (1-uv.y)*2 - 1, zOverw, 1);
	vec4 d = inverse(currProjViewMatrix) * h;
	vec4 worldPos = d / d.w;
	
	vec4 currentPos = h;
	vec4 previousPos = prevProjViewMatrix * worldPos;
	
	previousPos /= previousPos.w;
	
	float maxVelocity = 0.05;
	vec2 pixel = 1.0 / resolution;
	vec2 velocity = (currentPos.xy - previousPos.xy) * 2;
	//velocity = clamp(velocity, vec2(-maxVelocity), vec2(maxVelocity));
	vec2 sampleStep = velocity / SAMPLES;
	
	vec3 color = texture(texSampler, uv).rgb;
	color *= 0.001;
	
	uv += velocity;
	
	for (int i = 1; i <= SAMPLES; ++i) {
		vec2 coord = passTextureCoord - sampleStep * i;
		color += texture(texSampler, clamp(coord, pixel, 1.0 - pixel)).rgb;
	}
	
	//outColour = vec4(color /= max(SAMPLES + 1.0, 1.0), 1.0);
	//outColour = vec4(zOverw);
	outColour = clamp(worldPos, 0, 1);
}