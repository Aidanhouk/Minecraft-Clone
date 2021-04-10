#version 330

out vec4 outColour;

in vec2 passTextureCoord;

uniform sampler2D texSampler;
vec4 color;
uniform float lighting;

void main()
{
    color = texture(texSampler, passTextureCoord);
    outColour = vec4(color.rgb * lighting, color.a);
    if (outColour.a == 0) discard;
}
