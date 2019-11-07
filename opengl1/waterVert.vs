#version 430

in vec2 vPosition;

out vec4 clipSpace;
out vec2 textureCoords;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

const float tiling = 6.0;

void main(void) 
{
	clipSpace = projectionMatrix * viewMatrix * modelMatrix * vec4(vPosition.x, 0.0, vPosition.y, 1.0);
	gl_Position = clipSpace;
	textureCoords = vec2(vPosition.x/2.0 + 0.5, vPosition.y/2.0 + 0.5) * tiling;
}