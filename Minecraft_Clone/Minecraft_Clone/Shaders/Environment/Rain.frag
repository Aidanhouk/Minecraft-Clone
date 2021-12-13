#version 330

out vec4 outColour;
in  vec2 passTextureCoord;

uniform sampler2D texSampler;
uniform float lighting;
uniform float visibility;

void main()
{
    vec4 colour = texture(texSampler, passTextureCoord);
	
    outColour = vec4(colour.rgb * lighting, colour.a * visibility);
    if (outColour.a == 0) discard;
}
