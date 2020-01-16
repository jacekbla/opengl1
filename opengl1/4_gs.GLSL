#version 430

layout(triangles) in;
layout(triangle_strip, max_vertices = 4) out;

in vec4 color_te[];
out vec4 color;

in vec4 indi[];

out vec4 clipSpace;
out vec4 clipSpaceGrid;
out vec2 textureCoords;
out vec3 toCameraVector;
out vec3 normal;
out vec3 specular;
out vec3 diffuse;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

uniform vec3 cameraPosition;
uniform float height;
uniform float waveTime;
uniform vec3 lightPosition;
uniform vec3 lightColour;

const float tiling = 0.05;
const float PI = 3.1415926535897932384626433832795;

const float waveLength = 10.0;
const float waveAmplitude = 0.5;

const float specularReflectivity = 0.4;
const float shineDamper = 20.0;
const vec2 lightBias = vec2(0.5, 0.9);

float generateOffset(float x, float z)
{
	float radiansX = (x / waveLength + waveTime) * 2.0 * PI;
	float radiansZ = (z / waveLength + waveTime) * 2.0 * PI;
	return waveAmplitude * 0.5 * (sin(radiansZ) + cos(radiansX));
}

vec3 applyDistortion(vec3 vertex)
{
	float xDistortion = generateOffset(vertex.x, vertex.z);
	float yDistortion = generateOffset(vertex.x, vertex.z);
	float zDistortion = generateOffset(vertex.x, vertex.z);
	return vertex + vec3(xDistortion, yDistortion, zDistortion);
}

vec3 calcNormal(vec3 vertex0, vec3 vertex1, vec3 vertex2)
{
	vec3 tangent = vertex1 - vertex0;
	vec3 bitangent = vertex2 - vertex0;
	return normalize(cross(tangent, bitangent));
}

vec3 calcSpecularLighting(vec3 toCamVector, vec3 toLightVector, vec3 normal)
{
	vec3 reflectedLightDirection = reflect(-toLightVector, normal);
	float specularFactor = dot(reflectedLightDirection, toCamVector);
	specularFactor = max(specularFactor,0.0);
	specularFactor = pow(specularFactor, shineDamper);
	return specularFactor * specularReflectivity * lightColour;
}

vec3 calculateDiffuseLighting(vec3 toLightVector, vec3 normal)
{
	float brightness = max(dot(toLightVector, normal), 0.0);
	return (lightColour * lightBias.x) + (brightness * lightColour * lightBias.y);
}

vec4 getPosition(int i)
{
	vec2 vPosition = vec2(gl_in[i].gl_Position.x , gl_in[i].gl_Position.z);
	vec4 vIndicators = indi[i];

	// vec4 position = gl_in[i].gl_Position;
    // position = modelMatrix * position;
	// return (projMatrix * viewMatrix * position);
	// return position;

	vec3 currentVertex = vec3(vPosition.x, height, vPosition.y);
	vec3 vertex1 = currentVertex + vec3(vIndicators.x, 0.0, vIndicators.y);
	vec3 vertex2 = currentVertex + vec3(vIndicators.z, 0.0, vIndicators.w);

	vec4 worldPosition = modelMatrix * vec4(currentVertex, 1.0);
	clipSpaceGrid = projMatrix * viewMatrix * worldPosition;

	currentVertex = applyDistortion(currentVertex);
	vertex1 = applyDistortion(vertex1);
	vertex2 = applyDistortion(vertex2);

	normal = calcNormal(currentVertex, vertex1, vertex2);

	worldPosition = modelMatrix * vec4(currentVertex, 1.0);
	clipSpace = projMatrix * viewMatrix * worldPosition;
	gl_Position = clipSpace;

	textureCoords = vec2(vPosition.x/2.0 + 0.5, vPosition.y/2.0 + 0.5) * tiling;

	toCameraVector = normalize(cameraPosition - worldPosition.xyz);
	vec3 toLightVector = normalize(lightPosition - worldPosition.xyz);
	specular = calcSpecularLighting(toCameraVector, toLightVector, normal);
	diffuse = calculateDiffuseLighting(toLightVector, normal);

	return gl_Position;
}

vec4 getColor(int i)
{
	return color_te[i];
}

void createVertex(int i)
{
	gl_Position = getPosition(i);
	color = getColor(i);

	EmitVertex();
}

void main() {
	
	for (int i = 0; i < gl_in.length(); ++i)
	{
		createVertex(i);
	}
	createVertex(0);
	EndPrimitive();
}