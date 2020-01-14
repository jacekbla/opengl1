#version 430

layout(triangles) in;
layout(line_strip, max_vertices = 4) out;

//uniform mat4 projMatrix;
//uniform mat4 viewMatrix;
//uniform mat4 modelMatrix;

void main() {
	
	for (int i = 0; i < gl_in.length(); ++i)
	{
		vec4 position = gl_in[i].gl_Position;

		// position = modelMatrix * position;
		// gl_Position = projMatrix * viewMatrix * position;
		gl_Position = position;

		EmitVertex();
	}
	
	vec4 position = gl_in[0].gl_Position;

	// position = modelMatrix * position;
	// gl_Position = projMatrix * viewMatrix * position;
	gl_Position = position;

    EmitVertex();
	
	EndPrimitive();



}