#include "ShaderProgram.h"
#include "Camera.h"
#include "Maths.h"
#include "Light.h"

#ifndef WATER_SHADER_H
#define WATER_SHADER_H


class WaterShader : public ShaderProgram
{
public:
	explicit WaterShader() : ShaderProgram(_VERTEX_FILE, _FRAGMENT_FILE)
	{
		getAllUniformLocations();
		bindAttributes();
	};
	~WaterShader();

	virtual void bindAttributes() override;
	virtual void getAllUniformLocations() override;

	void connectTextureUnits();

	void loadProjectionMatrix(glm::mat4 &p_matrix);
	void loadViewMatrix(Camera &p_camera);
	void loadModelMatrix(glm::mat4 &p_matrix);
	void loadHeight(float p_height);
	void loadWaveTime(float p_waveTime);
	void loadLightPositions(std::vector<Light*> &p_lights);
	void loadLightColours(std::vector<Light*> &p_lights);
	void loadLightStrenghts(std::vector<Light*>& p_lights);


private:
	static const char* _VERTEX_FILE;
	static const char* _FRAGMENT_FILE;
	static const int _MAX_LIGHTS;

	int _location_modelMatrix;
	int _location_viewMatrix;
	int _location_projectionMatrix;
	int _location_reflectionTexture;
	int _location_refractionTexture;
	int _location_cameraPosition;
	int _location_depthMap;
	int _location_height;
	int _location_waveTime;
	int _location_lightPosition[4];
	int _location_lightColor[4];
	int _location_lightStrenght[4];
};


#endif WATER_SHADER_H