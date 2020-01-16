#version 430

in vec2 position;
in vec4 indicators;

out vec4 color_vertex;

out vec4 indi_vs;

//uniform mat4 projMatrix;
//uniform mat4 viewMatrix;
//uniform mat4 modelMatrix;

void main(void) 
{
	//vec4 worldPosition = modelMatrix * vec4(position.x, 0.0, position.y, 1.0);
	//gl_Position = projMatrix * viewMatrix * worldPosition;

	gl_Position = vec4(position.x, 0.0, position.y, 1.0);

	color_vertex = vec4(255, 0, 0, 1);
	indi_vs = indicators;

}