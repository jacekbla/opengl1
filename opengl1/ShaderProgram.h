#include "Utility.h"

#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

class ShaderProgram
{
public:
	ShaderProgram(const char* vertexFile, const char* fragmentFile);
	~ShaderProgram();

	virtual void start();
	virtual void stop();
	virtual void cleanUp();

private:
	int programHandle;
	int vertexShaderHandle;
	int fragmentShaderHandle;

	char* readShader(const char * aShaderFile);
	void setShaders(const char* p_vertex, const char* p_pixel, int &programHandle, int &vertexShaderHandle, int &fragmentShaderHandle);
	void setShader(const char* p_file, int &programHandle, int &shaderHandle);

protected:
	virtual void bindAttributes();
	virtual void bindAttribute(int attribute, const char* variableName);
	virtual void getAllUniformLocations();
	int getUniformLocation(const char* p_uniformName);
	void loadInt(int p_location, int p_value);
	void loadFloat(int p_location, float &p_value);
	void loadVector(int p_location, glm::vec3 &p_value);
	void loadVector(int p_location, glm::vec4 &p_value);
	void loadBoolean(int p_location, bool &value);
	void loadMatrix(int p_location, glm::mat4 &p_matrix);
};

#endif SHADERPROGRAM_H