#include "WaterShader.h"

const char* WaterShader::VERTEX_FILE = "waterVert.vs";
const char* WaterShader::FRAGMENT_FILE = "waterFrag.fs";

WaterShader::~WaterShader()
{
}

void WaterShader::bindAttributes()
{
	ShaderProgram::bindAttribute(0, "vPosition");
}

void WaterShader::getAllUniformLocations()
{
	location_projectionMatrix = ShaderProgram::getUniformLocation("projectionMatrix");
	location_viewMatrix = ShaderProgram::getUniformLocation("viewMatrix");
	location_modelMatrix = ShaderProgram::getUniformLocation("modelMatrix");
	location_reflectionTexture = ShaderProgram::getUniformLocation("reflectionTexture");
	location_refractionTexture = ShaderProgram::getUniformLocation("refractionTexture");
}

void WaterShader::connectTextureUnits()
{
	ShaderProgram::loadInt(location_reflectionTexture, 0);
	ShaderProgram::loadInt(location_refractionTexture, 1);
}

void WaterShader::loadProjectionMatrix(glm::mat4 &p_matrix)
{
	ShaderProgram::loadMatrix(location_projectionMatrix, p_matrix);
}

void WaterShader::loadViewMatrix(Camera &p_camera)
{
	glm::mat4 viewMatrix = Maths::createViewMatrix(p_camera);
	ShaderProgram::loadMatrix(location_viewMatrix, viewMatrix);
}

void WaterShader::loadModelMatrix(glm::mat4 &p_matrix)
{
	ShaderProgram::loadMatrix(location_modelMatrix, p_matrix);
}