#include "ShaderProgram.h"
#include "Camera.h"
#include "Maths.h"

#ifndef WATERSHADER_H
#define WATERSHADER_H


class WaterShader : public ShaderProgram
{
public:
	WaterShader() : ShaderProgram(VERTEX_FILE, FRAGMENT_FILE)
	{
		getAllUniformLocations();
		//bindAttributes();
	};
	~WaterShader();

	virtual void bindAttributes() override;
	virtual void getAllUniformLocations() override;

	void connectTextureUnits();

	void loadProjectionMatrix(glm::mat4 &p_matrix);
	void loadViewMatrix(Camera &p_camera);
	void loadModelMatrix(glm::mat4 &p_matrix);

private:
	static const char* VERTEX_FILE;
	static const char* FRAGMENT_FILE;

	int location_modelMatrix;
	int location_viewMatrix;
	int location_projectionMatrix;
	int location_reflectionTexture;
	int location_refractionTexture;
};


#endif WATERSHADER_H