#include "WaterShader.h"

const char* WaterShader::_VERTEX_FILE = "waterVert.vs";
const char* WaterShader::_FRAGMENT_FILE = "waterFrag.fs";
const int WaterShader::_MAX_LIGHTS = 4;

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

	for (int i = 0; i < _MAX_LIGHTS; i++) {
		if (i == 0) {
			_location_lightPosition[i] = ShaderProgram::getUniformLocation("lightPosition");
			_location_lightColor[i] = ShaderProgram::getUniformLocation("lightColour");
			_location_lightStrenght[i] = ShaderProgram::getUniformLocation("lightStrenght");
		}
		else {
			_location_lightPosition[i] = _location_lightPosition[i - 1] + 1;
			_location_lightColor[i] = _location_lightColor[i - 1] + 1;
			_location_lightStrenght[i] = _location_lightStrenght[i - 1] + 1;
		}
	}
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

void WaterShader::loadLightPositions(std::vector<Light*> &p_lights)
{
	glm::vec3 zero(0.0, 0.0, 0.0);
	for (int i = 0; i < _MAX_LIGHTS; i++) {
		if (i < p_lights.size()) {
			ShaderProgram::loadVector(_location_lightPosition[i], p_lights[i]->getPostion());

		}
		else {
			ShaderProgram::loadVector(_location_lightPosition[i],zero);
		}
	}
}

void WaterShader::loadLightColours(std::vector<Light*> &p_lights)
{
	glm::vec3 zero(0.0, 0.0, 0.0);
	for (int i = 0; i < _MAX_LIGHTS; i++) {
		if (i < p_lights.size()) {
			ShaderProgram::loadVector(_location_lightColor[i], p_lights[i]->getColor());

		}
		else {
			ShaderProgram::loadVector(_location_lightColor[i],zero);
		}
	}
}

void WaterShader::loadLightStrenghts(std::vector<Light*>& p_lights)
{

	for (int i = 0; i < _MAX_LIGHTS; i++) {
		if (i < p_lights.size()) {
			ShaderProgram::loadFloat(_location_lightStrenght[i], p_lights[i]->getStrenght());

		}
		else {
			ShaderProgram::loadFloat(_location_lightStrenght[i], 0.0);
		}
	}
}


