#version 330

layout(location = 0) in vec3 inVertexPosition;

uniform mat4 projViewMatrix;
uniform mat4 modelMatrix;

void main()
{
    gl_Position = projViewMatrix * modelMatrix * vec4(inVertexPosition, 1.0);
}
