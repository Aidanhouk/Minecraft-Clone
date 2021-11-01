#version 330

out vec4 outColour;

in vec2 passTextureCoord;
in float visibility;

uniform sampler2D texSampler;
uniform float lighting;
uniform int fog;

void main()
{
    vec4 colour = texture(texSampler, passTextureCoord);
    if (colour.a == 0) discard;
	
	if (fog == 1) {
		outColour = vec4(colour.rgb * lighting, colour.a * visibility);
		if (outColour.a == 0) discard;
	}
	else {
		outColour = vec4(colour.rgb * lighting, colour.a);
	}
}
