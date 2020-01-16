#version 430

layout (vertices = 3) out;

in vec4 color_vertex[];
out vec4 color_tc[];

in vec4 indi_vs[];
out vec4 indi_tc[];

void main(void)
{
    if (gl_InvocationID == 0)
	{
        gl_TessLevelInner[0] = 3;//3;

        gl_TessLevelOuter[0] = 3;//3;
        gl_TessLevelOuter[1] = 3;//3;
        gl_TessLevelOuter[2] = 3;//3;
    }

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	color_tc[gl_InvocationID] = color_vertex[gl_InvocationID];
	indi_tc[gl_InvocationID] = indi_vs[gl_InvocationID];

}