#version 430

layout (triangles, equal_spacing, cw) in;

void main(void)
{ 
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;
	float s = gl_TessCoord.z;
	
	gl_Position = 
	( u * gl_in[0].gl_Position+
	  v * gl_in[1].gl_Position+
	  s * gl_in[2].gl_Position
	);

}