#pragma once

#include "Utility.h"
#include "Loader.h"
#include "RawModel.h"
#include "Maths.h"

#ifndef TESS_H
#define TESS_H

class Tess {

public:
	Tess(Loader p_loader, glm::mat4 p_projMatrix);
	~Tess();

	void cleanUp();
	void render(Camera& p_camera);

private:
	const char* _FILE_VS;
	const char* _FILE_TC;
	const char* _FILE_TE;
	const char* _FILE_GS;
	const char* _FILE_FS;

	float _square_x;
	float _square_z;
	float _square_y;
	float _square_scale;

	RawModel _square_rawModel;

	int h_program;
	int h_VS;
	int h_TC;
	int h_TE;
	int h_GS;
	int h_FS;

	int _location_projMatrix;
	int _location_viewMatrix;
	int _location_modelMatrix;

	void enableShader();
	void disableShader();

	void createRawModel(Loader p_loader);

	char* readShaderFile(const char * p_shaderFile);
	void addShaderToProgram(const char* p_file, int &p_shaderHandle);
	void createProgram();

	void loadMatrix(int p_location, glm::mat4 &p_matrix);



};



#endif TESS_H