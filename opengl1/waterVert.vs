#version 430

layout(location = 0) in vec2 vPosition;
layout(location = 1) in vec4 vIndicators;

out vec4 clipSpace;
out vec4 clipSpaceGrid;
out vec3 toCameraVector;
out vec3 toLightVector;
out vec3 surfaceNormal;
out vec3 vertexNormal;
out vec3 specular;
out vec3 diffuse;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform vec3 cameraPosition;
uniform float height;
uniform float waveTime;
uniform vec3 lightPosition;
uniform vec3 lightColour;

const float tiling = 0.5;
const float PI = 3.1415926535897932384626433832795;

const float waveLength = 15.0;
const float waveAmplitude = 0.3;

const float specularReflectivity = 0.4;
const float shineDamper = 20.0;
const vec2 lightBias = vec2(0.5, 0.8);

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

vec3 calcTriangleNormal(vec3 vertex0, vec3 vertex1, vec3 vertex2)
{
	vec3 tangent = vertex1 - vertex0;
	vec3 bitangent = vertex2 - vertex0;
	return normalize(cross(tangent, bitangent));
}

vec3 calcVertexNormalT(vec3 normal0, vec3 normal1, vec3 normal2, vec3 normal3, vec3 normal4, vec3 normal5)
{
	return (normal0 + normal1 + normal2 + normal3 + normal4 + normal5) / 6.0;
}

vec3 calcVertexNormal(vec3 vertex0, vec3 vertex1, vec3 vertex2, vec3 vertex3, vec3 vertex4, vec3 vertex5, vec3 vertex6)
{
	vec3 normalTriangle403 = calcTriangleNormal(vertex4, vertex0, vertex3);
	vec3 normalTriangle450 = calcTriangleNormal(vertex4, vertex5, vertex0);
	vec3 normalTriangle560 = calcTriangleNormal(vertex5, vertex6, vertex0);
	vec3 normalTriangle302 = calcTriangleNormal(vertex3, vertex0, vertex2);
	vec3 normalTriangle012 = calcTriangleNormal(vertex0, vertex1, vertex2);
	vec3 normalTriangle061 = calcTriangleNormal(vertex0, vertex6, vertex1);
	
	vec3 noramlVertex0 = calcVertexNormalT(normalTriangle403, normalTriangle450, normalTriangle560, normalTriangle302, normalTriangle012, normalTriangle061);

	return normalize(noramlVertex0);
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

void main(void) 
{
	vec3 currentVertex = vec3(vPosition.x * tiling, height, vPosition.y * tiling);
	vec3 vertex1 = currentVertex + vec3(-1.0, 0.0, 1.0);
	vec3 vertex2 = currentVertex + vec3(0.0, 0.0, 1.0);

	vec3 vertex3 = currentVertex + vec3(1.0, 0.0, 0.0);
	vec3 vertex4 = currentVertex + vec3(1.0, 0.0, -1.0);
	vec3 vertex5 = currentVertex + vec3(0.0, 0.0, -1.0);
	vec3 vertex6 = currentVertex + vec3(-1.0, 0.0, 0.0);

	vec3 vertex1_surface = currentVertex + vec3(vIndicators.x, 0.0, vIndicators.y);
	vec3 vertex2_surface = currentVertex + vec3(vIndicators.z, 0.0, vIndicators.w);

	//vec3 vertex3 = currentVertex + vec3(-vIndicators.x, 0.0, 0.0);
	//vec3 vertex4 = currentVertex + vec3(-vIndicators.x, 0.0, -vIndicators.y);
	//vec3 vertex5 = currentVertex + vec3(-vIndicators.z, 0.0, -vIndicators.w);
	//vec3 vertex6 = currentVertex + vec3(vIndicators.x, 0.0, 0.0);

	vec4 worldPosition = modelMatrix * vec4(currentVertex, 1.0);
	clipSpaceGrid = projectionMatrix * viewMatrix * worldPosition;

	currentVertex = applyDistortion(currentVertex);
	vertex1 = applyDistortion(vertex1);
	vertex2 = applyDistortion(vertex2);

	vertex3 = applyDistortion(vertex3);
	vertex4 = applyDistortion(vertex4);
	vertex5 = applyDistortion(vertex5);
	vertex6 = applyDistortion(vertex6);
	
	vertex1_surface = applyDistortion(vertex1_surface);
	vertex2_surface = applyDistortion(vertex2_surface);

	vertexNormal = calcVertexNormal(currentVertex, vertex1, vertex2, vertex3, vertex4, vertex5, vertex6);
	surfaceNormal = calcTriangleNormal(currentVertex, vertex1_surface, vertex2_surface);

	worldPosition = modelMatrix * vec4(currentVertex, 1.0);
	clipSpace = projectionMatrix * viewMatrix * worldPosition;
	gl_Position = clipSpace;

	toCameraVector = normalize(cameraPosition - worldPosition.xyz);
	toLightVector = normalize(lightPosition - worldPosition.xyz);
	specular = calcSpecularLighting(toCameraVector, toLightVector, vertexNormal);
	diffuse = calculateDiffuseLighting(toLightVector, vertexNormal);
}