#version 430 

//in vec3 vColorFrag;
in vec2 vTexCoordsFrag;
in vec3 vSurfaceNormal;
in vec3 vToLightVector[4];
in vec3 vToCameraVector;

out vec4 fColor;

uniform sampler2D textureSampler;
uniform vec3 lightColor[4];
uniform float lightStrenght[4];
uniform float shineDamper;
uniform float reflectivity;

void main()
{
	vec3 unitNormal = normalize(vSurfaceNormal);
	vec3 unitToCameraVector = normalize(vToCameraVector);

	vec3 totalDiffuse = vec3(0.0,0.0,0.0);
	vec3 totalSpecular = vec3(0.0,0.0,0.0);

	
	for(int i=0; i<4; i++){
		vec3 unitToLightVector = normalize(vToLightVector[i]);
		float nDotl = dot(unitNormal, unitToLightVector);
		float brightness = max(nDotl, 0.1);
		vec3 lightDirection = -unitToLightVector;
		vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);
		float specularFactor = dot(reflectedLightDirection, unitToCameraVector);
		specularFactor = max(specularFactor, 0.0);
		float dampedFactor = pow(specularFactor, shineDamper);
		float attenuation =2.0f/(pow ( distance(vToLightVector[i], vec3(0.0,0.0,0.0)),1.1f)*(1-lightStrenght[i]));
		totalDiffuse = totalDiffuse + (brightness * lightColor[i]*attenuation);
		totalSpecular = totalSpecular + (dampedFactor * reflectivity * lightColor[i]*attenuation);
	}
	totalDiffuse=max(totalDiffuse, 0.2);

	
    //float attenuation=1.0f;
	//fColor = vec4(vColorFrag, 1.0);
	fColor = vec4(totalDiffuse, 1.0) * texture(textureSampler, vTexCoordsFrag) + vec4(totalSpecular, 1.0);
}
