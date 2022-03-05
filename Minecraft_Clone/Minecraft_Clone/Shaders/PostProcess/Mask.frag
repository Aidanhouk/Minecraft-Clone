#version 330

out vec4 outColour;
in  vec2 passTextureCoord;

uniform sampler2D texSampler;
//uniform float requestedLuminance;

const float requestedLuminance = 1.0f;

void main()
{
    vec4 color = texture(texSampler, passTextureCoord);

    float brightness = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));
    if (brightness >= requestedLuminance)
        outColour = vec4(color.rgb, 1.0);
    else
        outColour = vec4(0.0, 0.0, 0.0, 1.0);

    //float relativeLuminance = sqrt(0.299 * color.r * color.r + 0.587 * color.g * color.g + 0.114 * color.b * color.b);
    //if (relativeLuminance > requestedLuminance){
    //    outColour = color;
    //}
	//else {
    //    outColour = vec4(0.0);
    //}
}
