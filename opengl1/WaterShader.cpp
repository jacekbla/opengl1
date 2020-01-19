#include "WaterShader.h"

const char* WaterShader::_VERTEX_FILE = "waterVert.vs";
const char* WaterShader::_FRAGMENT_FILE = "waterFrag.fs";

WaterShader::~WaterShader()
{
}

void WaterShader::bindAttributes()
{
	ShaderProgram::bindAttribute(0, "vPosition");
	ShaderProgram::bindAttribute(1, "vIndicators");
}

void WaterShader::getAllUniformLocations()
{
	_location_projectionMatrix = ShaderProgram::getUniformLocation("projectionMatrix");
	_location_viewMatrix = ShaderProgram::getUniformLocation("viewMatrix");
	_location_modelMatrix = ShaderProgram::getUniformLocation("modelMatrix");
	_location_reflectionTexture = ShaderProgram::getUniformLocation("reflectionTexture");
	_location_refractionTexture = ShaderProgram::getUniformLocation("refractionTexture");
	_location_cameraPosition = ShaderProgram::getUniformLocation("cameraPosition");
	_location_depthMap = ShaderProgram::getUniformLocation("depthMap");
	_location_height = ShaderProgram::getUniformLocation("height");
	_location_waveTime = ShaderProgram::getUniformLocation("waveTime");
	_location_lightPosition = ShaderProgram::getUniformLocation("lightPosition");
	_location_lightColour = ShaderProgram::getUniformLocation("lightColour");
}

void WaterShader::connectTextureUnits()
{
	ShaderProgram::loadInt(_location_reflectionTexture, 0);
	ShaderProgram::loadInt(_location_refractionTexture, 1);
	ShaderProgram::loadInt(_location_depthMap, 3);
}

void WaterShader::loadProjectionMatrix(glm::mat4 &p_matrix)
{
	ShaderProgram::loadMatrix(_location_projectionMatrix, p_matrix);
}

void WaterShader::loadViewMatrix(Camera &p_camera)
{
	glm::mat4 viewMatrix = Maths::createViewMatrix(p_camera);
	ShaderProgram::loadMatrix(_location_viewMatrix, viewMatrix);
	glm::vec3 pos = p_camera.getPosition();
	ShaderProgram::loadVector(_location_cameraPosition, pos);
}

void WaterShader::loadModelMatrix(glm::mat4 &p_matrix)
{
	ShaderProgram::loadMatrix(_location_modelMatrix, p_matrix);
}

void WaterShader::loadHeight(float p_height)
{
	ShaderProgram::loadFloat(_location_height, p_height);
}

void WaterShader::loadWaveTime(float p_waveTime)
{
	ShaderProgram::loadFloat(_location_waveTime, p_waveTime);
}

void WaterShader::loadLightPosition(glm::fvec3 p_lightPosition)
{
	ShaderProgram::loadVector(_location_lightPosition, p_lightPosition);
}

void WaterShader::loadLightColour(glm::fvec3 p_lightColout)
{
	ShaderProgram::loadVector(_location_lightColour, p_lightColout);
}


