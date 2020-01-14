#version 430

in vec2 position;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main(void) 
{
	vec4 worldPosition = modelMatrix * vec4(position.x, 0.0, position.y, 1.0);
	gl_Position = projMatrix * viewMatrix * worldPosition;

	// gl_Position = vec4(position.x, 0.0, position.y, 1.0);

}