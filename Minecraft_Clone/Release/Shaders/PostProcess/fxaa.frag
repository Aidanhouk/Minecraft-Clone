#version 330

out vec4 outColour;
in  vec2 passTextureCoord;

uniform sampler2D texSampler;
uniform vec2 resolution;

#define FXAA_REDUCE_MIN (1.0/128.0)
#define FXAA_REDUCE_MUL (1.0/8.0)
#define FXAA_SPAN_MAX 8.0

vec3 fxaa(vec2 resolution, sampler2D sampler0, vec2 texcoord)
{
	vec2 inverse_resolution = resolution;
    vec3 luma = vec3(0.299, 0.587, 0.114);
    float lumaNW = dot(luma, texture(sampler0, texcoord.xy + (vec2(-1.0, -1.0)) * inverse_resolution).xyz);
    float lumaNE = dot(luma, texture(sampler0, texcoord.xy + (vec2(1.0, -1.0)) * inverse_resolution).xyz);
    float lumaSW = dot(luma, texture(sampler0, texcoord.xy + (vec2(-1.0, 1.0)) * inverse_resolution).xyz);
    float lumaSE = dot(luma, texture(sampler0, texcoord.xy + (vec2(1.0, 1.0)) * inverse_resolution).xyz);
    float lumaM  = dot(luma, texture(sampler0,  texcoord.xy).xyz);

    vec2 dir;
    dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
    dir.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE));

    float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAA_REDUCE_MUL), FXAA_REDUCE_MIN);
    float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);
    dir = min(vec2(FXAA_SPAN_MAX, FXAA_SPAN_MAX), max(vec2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX), dir * rcpDirMin)) * inverse_resolution;

    vec3 rgbA = 0.5 * (
		texture(sampler0, texcoord.xy + dir * (1.0 / 3.0 - 0.5)).xyz +
		texture(sampler0, texcoord.xy + dir * (2.0 / 3.0 - 0.5)).xyz);
    vec3 rgbB = rgbA * 0.5 + 0.25 * (
		texture(sampler0, texcoord.xy + dir * -0.5).xyz +
		texture(sampler0, texcoord.xy + dir * 0.5).xyz);

    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));
    float lumaB = dot(rgbB, luma);

    if ((lumaB < lumaMin) || (lumaB > lumaMax))
        return rgbA;
    else
        return rgbB;
}

void main(){
    vec4 color = texture(texSampler, passTextureCoord);
    color = vec4(fxaa(resolution, texSampler, passTextureCoord), color.w);

    outColour = color;
}