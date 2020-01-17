#version 430

layout (vertices = 3) out;

//in vec4 color_vertex[];
//out vec4 color_tc[];

in vec4 indi_vs[];
out vec4 indi_tc[];

uniform vec3 cameraPosition;
uniform mat4 modelMatrix;
uniform float height;

vec4 A = modelMatrix * vec4(gl_in[0].gl_Position.x*0.5, height ,gl_in[0].gl_Position.z*0.5, 1.0);
vec4 B = modelMatrix * vec4(gl_in[1].gl_Position.x*0.5, height ,gl_in[1].gl_Position.z*0.5, 1.0);
vec4 C = modelMatrix * vec4(gl_in[2].gl_Position.x*0.5, height ,gl_in[2].gl_Position.z*0.5, 1.0);

vec3 AB = vec3(A+B)/2.0;
vec3 BC = vec3(B+C)/2.0;
vec3 CA = vec3(C+A)/2.0;

float dstAB = distance(AB,cameraPosition);
float dstBC = distance(BC,cameraPosition);
float dstCA = distance(CA,cameraPosition);

float getTessLevel(float dst)
{
	if(dst < 0.5)
	{
		return 5;
	}
	if(dst < 1)
	{
		return 4;
	}
	if(dst < 2)
	{
		return 2;
	}
	else
	{
		return 1;
	}
}


void main(void)
{

    if (gl_InvocationID == 0)
	{
        gl_TessLevelOuter[0] = getTessLevel(dstAB); //mix(1,maxTess,getTessLevel(dstAB));
        gl_TessLevelOuter[1] = getTessLevel(dstAB); //mix(1,maxTess,getTessLevel(dstBC));
        gl_TessLevelOuter[2] = getTessLevel(dstAB); //mix(1,maxTess,getTessLevel(dstCA));

		gl_TessLevelInner[0] = (gl_TessLevelOuter[0]+gl_TessLevelOuter[1]+gl_TessLevelOuter[2])/3;
    }

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	//color_tc[gl_InvocationID] = color_vertex[gl_InvocationID];
	indi_tc[gl_InvocationID] = indi_vs[gl_InvocationID];

}