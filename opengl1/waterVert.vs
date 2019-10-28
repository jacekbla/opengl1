#version 430

in vec2 vPosition;

out vec4 clipSpace;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;


void main(void) 
{
	clipSpace = projectionMatrix * viewMatrix * modelMatrix * vec4(vPosition.x, 0.0, vPosition.y, 1.0);
	gl_Position = clipSpace;
}