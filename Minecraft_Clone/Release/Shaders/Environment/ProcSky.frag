#version 330

in vec3 passColor;
out vec4 outColour;

void main(){
    vec4 colour = vec4(passColor, 1.0f);
    
    outColour = colour;
    if (outColour.a == 0) discard;
}