#version 430

layout (triangles, equal_spacing, cw) in;

//in vec4 color_tc[];
//out vec4 color_te;

in vec4 indi_tc[];
out vec4 indi;

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

	// new vertex should get color by mixing 0, 1, 2 colors
	//color_te.x = int((color_tc[0].x + color_tc[1].x + color_tc[2].x)/3);
	//color_te.y = int((color_tc[0].y + color_tc[1].y + color_tc[2].y)/3);
	//color_te.z = int((color_tc[0].z + color_tc[1].z + color_tc[2].z)/3);
	//color_te.w = 1;

	indi = indi_tc[0];

}