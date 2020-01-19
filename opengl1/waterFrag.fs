#version 430

in vec4 clipSpace;
in vec4 clipSpaceGrid;
in vec3 toCameraVector;
in vec3 toLightVector;
in vec3 surfaceNormal;
in vec3 vertexNormal;
in vec3 specular;
in vec3 diffuse;

out vec4 out_Color;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D depthMap;

const float waveStrength = 0.01;
const float waterBlendDepth = 0.25;
const float fresnelReflective = 0.6;
const float murkyDepth = 14.0;
const float minBlueness = 0.4;
const float maxBlueness = 0.8;
const vec4 waterColour = vec4(0.0, 0.3, 0.5, 1.0);

vec4 applyMurkiness(vec4 refractColour, float waterDepth)
{
	float murkyFactor = clamp(waterDepth / murkyDepth, 0.0, 1.0);
	float murkiness = minBlueness + murkyFactor * (maxBlueness - minBlueness);
	return mix(refractColour, waterColour, murkiness);
}

float calculateFresnel()
{
	vec3 viewVector = normalize(toCameraVector);
	float refractiveFactor = dot(viewVector, normalize(vertexNormal));
	refractiveFactor = pow(refractiveFactor, fresnelReflective);
	return clamp(refractiveFactor, 0.0, 1.0);
}

void main(void) 
{
	vec2 ndc = (clipSpace.xy/clipSpace.w)/2.0 + 0.5;
	vec2 refractTexCoordsReal = vec2(ndc.x, ndc.y);
	vec2 reflectTexCoordsReal = vec2(ndc.x, -ndc.y);

    vec2 ndcGrid = (clipSpaceGrid.xy/clipSpaceGrid.w)/2.0 + 0.5;
	vec2 refractTexCoords = vec2(ndcGrid.x, ndcGrid.y);
	vec2 reflectTexCoords = vec2(ndcGrid.x, -ndcGrid.y);

	float near = 0.1;
	float far = 1000.0;
	float depth = texture(depthMap, refractTexCoordsReal).r;
	float floorDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));

	depth = gl_FragCoord.z;
	float waterDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));
	float waterDepth = floorDistance - waterDistance;
	
	refractTexCoords = clamp(refractTexCoords, 0.001, 0.999);
	reflectTexCoords.x = clamp(reflectTexCoords.x, 0.001, 0.999);
	reflectTexCoords.y = clamp(reflectTexCoords.y, -0.999, -0.001);

	vec4 reflectColor = texture(reflectionTexture, reflectTexCoords);
	vec4 refractColor = texture(refractionTexture, refractTexCoords);
    refractColor = applyMurkiness(refractColor, waterDepth);

	out_Color = mix(reflectColor, refractColor, calculateFresnel());
	//out_Color = mix(out_Color, vec4(0.0, 0.3, 0.5, 1.0), 0.2);
    out_Color = out_Color * vec4(diffuse, 1.0) + vec4(specular, 0.0);
	out_Color.a = clamp(waterDepth/waterBlendDepth, 0.0, 1.0);
}