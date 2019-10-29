#include "WaterShader.h"

const char* WaterShader::_VERTEX_FILE = "waterVert.vs";
const char* WaterShader::_FRAGMENT_FILE = "waterFrag.fs";

WaterShader::~WaterShader()
{
}

void WaterShader::bindAttributes()
{
	ShaderProgram::bindAttribute(0, "vPosition");
}

void WaterShader::getAllUniformLocations()
{
	_location_projectionMatrix = ShaderProgram::getUniformLocation("projectionMatrix");
	_location_viewMatrix = ShaderProgram::getUniformLocation("viewMatrix");
	_location_modelMatrix = ShaderProgram::getUniformLocation("modelMatrix");
	_location_reflectionTexture = ShaderProgram::getUniformLocation("reflectionTexture");
	_location_refractionTexture = ShaderProgram::getUniformLocation("refractionTexture");
}

void WaterShader::connectTextureUnits()
{
	ShaderProgram::loadInt(_location_reflectionTexture, 0);
	ShaderProgram::loadInt(_location_refractionTexture, 1);
}

void WaterShader::loadProjectionMatrix(glm::mat4 &p_matrix)
{
	ShaderProgram::loadMatrix(_location_projectionMatrix, p_matrix);
}

void WaterShader::loadViewMatrix(Camera &p_camera)
{
	glm::mat4 viewMatrix = Maths::createViewMatrix(p_camera);
	ShaderProgram::loadMatrix(_location_viewMatrix, viewMatrix);
}

void WaterShader::loadModelMatrix(glm::mat4 &p_matrix)
{
	ShaderProgram::loadMatrix(_location_modelMatrix, p_matrix);
}