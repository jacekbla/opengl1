#include "StaticShader.h"

const char* StaticShader::_VERTEX_FILE = "vert.vxs";
const char* StaticShader::_FRAGMENT_FILE = "frag.fs";

const int StaticShader::_MAX_LIGHTS = 4;

StaticShader::~StaticShader()
{
}

void StaticShader::bindAttributes()
{
	ShaderProgram::bindAttribute(0, "vPosition");
	ShaderProgram::bindAttribute(1, "vColor");
	ShaderProgram::bindAttribute(2, "vTexCoords");
	ShaderProgram::bindAttribute(3, "vNormal");
}

void StaticShader::getAllUniformLocations()
{
	_location_MVP = ShaderProgram::getUniformLocation("MVP");
	_location_transformMatrix = ShaderProgram::getUniformLocation("transformMatrix");
	_location_projectionMatrix = ShaderProgram::getUniformLocation("projectionMatrix");
	_location_viewMatrix = ShaderProgram::getUniformLocation("viewMatrix");
	_location_shineDamper = ShaderProgram::getUniformLocation("shineDamper");
	_location_reflectivity = ShaderProgram::getUniformLocation("reflectivity");
	_location_plane = ShaderProgram::getUniformLocation("plane");

	//_location_lightPosition = new int[4];
	//_location_lightColor =  int[4];

	for (int i = 0; i < _MAX_LIGHTS; i++) {
		if (i == 0) {
			_location_lightPosition[i] = ShaderProgram::getUniformLocation("lightPosition");
			_location_lightColor[i] = ShaderProgram::getUniformLocation("lightColor");
			_location_lightStrenght[i] = ShaderProgram::getUniformLocation("lightStrenght");
		}
		else {
			_location_lightPosition[i] = _location_lightPosition[i - 1] + 1;
			_location_lightColor[i] = _location_lightColor[i - 1] + 1;
			_location_lightStrenght[i] = _location_lightStrenght[i - 1] + 1;
		}
	}
}

void StaticShader::loadTransformMatrix(glm::mat4 &p_matrix)
{
	ShaderProgram::loadMatrix(_location_transformMatrix, p_matrix);
}

void StaticShader::loadProjectionMatrix(glm::mat4 &p_matrix)
{
	ShaderProgram::loadMatrix(_location_projectionMatrix, p_matrix);
}

void StaticShader::loadViewMatrix(Camera &p_camera)
{
	glm::mat4 viewMatrix = Maths::createViewMatrix(p_camera);
	ShaderProgram::loadMatrix(_location_viewMatrix, viewMatrix);
}

void StaticShader::loadLights(std::vector<Light*> &p_light)
{
	//glm::vec3 zero(0.0, 0.0, 0.0);
	for (int i = 0; i < _MAX_LIGHTS; i++) {
		if (i<p_light.size()) {
			ShaderProgram::loadVector(_location_lightPosition[i], p_light[i]->getPostion());
			ShaderProgram::loadVector(_location_lightColor[i], p_light[i]->getColor());
			ShaderProgram::loadFloat(_location_lightStrenght[i], p_light[i]->getStrenght());
		}
	}
}

void StaticShader::loadShineVariables(float p_damper, float p_reflectivity)
{
	ShaderProgram::loadFloat(_location_shineDamper, p_damper);
	ShaderProgram::loadFloat(_location_reflectivity, p_reflectivity);
}

void StaticShader::loadClipPlane(glm::fvec4 p_plane)
{
	loadVector(_location_plane, p_plane);
}

void StaticShader::loadMVP(glm::mat4 &p_matrix)
{
	ShaderProgram::loadMatrix(_location_MVP, p_matrix);
}