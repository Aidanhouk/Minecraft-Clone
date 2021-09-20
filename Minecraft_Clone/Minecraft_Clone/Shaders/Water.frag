#version 330

out vec4 outColour;

in  vec2 passTextureCoord;
in float passCardinalLight;
in float visibility;

uniform sampler2D texSampler;
uniform float lighting;
//uniform vec3 skyColour;
vec3 skyColour;

void main()
{
	vec4 colour = texture(texSampler, passTextureCoord);
	//if (colour.a == 0) discard;
	
	//skyColour = vec3(0.5, 0.5, 0.5) * lighting;
	//outColour = vec4(colour.rgb * lighting * passCardinalLight, colour.a);
	//outColour = mix(vec4(skyColour, 1.0), outColour, visibility);

	//outColour = vec4(colour.rgb * lighting * passCardinalLight, colour.a * visibility);
	
	outColour = vec4(colour.rgb * lighting * passCardinalLight, colour.a);
	outColour.xy *= 1.1f;
	outColour.xyz += lighting * vec3(0.04, 0.03, 0.02);
}