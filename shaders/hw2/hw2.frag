#version 330

out vec4 fragColor;

uniform float inputTime;

void main() 
{
    vec4 finalColor = vec4(1) + vec4(1,0,0,0)*sin(inputTime) + vec4(0,1,0,0)*cos(inputTime) - vec4(0,0,1,0)*cos(inputTime);

    // Insert your code for "Slightly-More Advanced Shaders" here.

    fragColor = finalColor ;
}
