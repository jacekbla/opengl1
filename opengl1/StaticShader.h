#include "ShaderProgram.h"
#include "Camera.h"
#include "Maths.h"
#include "Light.h"

#ifndef STATICSHADER_H
#define STATICSHADER_H

class StaticShader : public ShaderProgram
{
public:
	StaticShader() : ShaderProgram(VERTEX_FILE, FRAGMENT_FILE) 
	{
		getAllUniformLocations();
		//bindAttributes();
	};
	~StaticShader();

	virtual void bindAttributes() override;
	virtual void getAllUniformLocations() override;

	void loadTransformMatrix(glm::mat4 &p_matrix);
	void loadProjectionMatrix(glm::mat4 &p_matrix);
	void loadViewMatrix(Camera &p_camera);
	void loadLight(Light &p_light);
	void loadShineVariables(float &p_damper, float &p_reflectivity);
	void loadClipPlane(glm::fvec4 plane);

	void loadMVP(glm::mat4 &p_matrix);

private:
	static const char* VERTEX_FILE;
	static const char* FRAGMENT_FILE;
	int location_MVP;
	int location_transformMatrix;
	int location_projectionMatrix;
	int location_viewMatrix;
	int location_lightPosition;
	int location_lightColor;
	int location_shineDamper;
	int location_reflectivity;
	int location_plane;
};

#endif STATICSHADER_H