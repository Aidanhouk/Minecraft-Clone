#version 330

out vec4 outColour;

in vec2 passTextureCoord;
in float passCardinalLight;
in float passVisibility;

uniform sampler2D texSampler;
uniform int fog;

void main()
{
	vec4 colour = texture(texSampler, passTextureCoord);
	if (colour.a == 0) discard;

	if (fog == 1) {
		outColour = vec4(colour.rgb * passCardinalLight, colour.a * passVisibility);
		if (outColour.a == 0) discard;
	}
	else {
		outColour = vec4(colour.rgb * passCardinalLight, colour.a);
	}
}