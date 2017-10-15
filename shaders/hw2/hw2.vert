#version 330

layout(location = 0) in vec4 vertexPosition;

uniform float inputTime;

void main()
{
    vec4 modifiedVertexPosition = vertexPosition - vec4(0,1,0,0)*sin(inputTime)/2 - vec4(0,1,0,0)/2;

    // Insert your code for "Slightly-More Advanced Shaders" here.

    gl_Position = modifiedVertexPosition;
}