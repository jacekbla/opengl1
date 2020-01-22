#pragma once

#include "Utility.h"
#include "Loader.h"
#include "RawModel.h"
#include "Maths.h"
#include "WaterTile.h"
#include "Light.h"
#include "DisplayManager.h"
#include "WaterFrameBuffers.h"

#ifndef TESS_H
#define TESS_H

class Tess {

public:
	Tess(Loader p_loader, glm::mat4 p_projMatrix, WaterTile& p_quad, WaterFrameBuffers p_fbos);
	~Tess();

	void render(Camera& p_camera, std::vector<Light*> &p_lights, bool seeTessEdges);
	void cleanUp();

private:
	static const char* _DUDV_MAP;
	static const float _WAVE_SPEED;
	static const float _HEIGHT;

	static const int _MAX_LIGHTS;

	const char* _FILE_VS;
	const char* _FILE_TC;
	const char* _FILE_TE;
	const char* _FILE_GS;
	const char* _FILE_FS;

	WaterTile _water;
	WaterFrameBuffers _fbos;

	float _moveFactor = 0.0f;
	float _waveTime = 0.0f;
	int _dudvTexture;

	int h_program;
	int h_VS;
	int h_TC;
	int h_TE;
	int h_GS;
	int h_FS;

	int _location_projMatrix;
	int _location_viewMatrix;
	int _location_modelMatrix;

	int _location_reflectionTexture;
	int _location_refractionTexture;
	int _location_dudvMap;

	int _location_moveFactor;
	int _location_cameraPosition;
	int _location_depthMap;
	int _location_height;
	int _location_waveTime;

	int _location_lightPosition[4];
	int _location_lightColor[4];
	int _location_lightStrenght[4];

	//int _location_lightPosition;
	//int _location_lightColour;

	void beforeRender(Camera p_camera, std::vector<Light*> p_lights);
	void afterRender();

	void enableShader();
	void disableShader();

	char* readShaderFile(const char * p_shaderFile);
	void addShaderToProgram(const char* p_file, int &p_shaderHandle);
	void createProgram();

	void loadMatrix(int p_location, glm::mat4 &p_matrix);
	void loadVec3(int p_location, glm::vec3 &p_value);
	void loadFloat(int p_location, float p_value);
	void loadInt(int p_location, int p_value);



};



#endif TESS_H