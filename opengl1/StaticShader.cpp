#include "StaticShader.h"

const char* StaticShader::VERTEX_FILE = "vert.vs";
const char* StaticShader::FRAGMENT_FILE = "frag.fs";


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
	location_MVP = ShaderProgram::getUniformLocation("MVP");
	location_transformMatrix = ShaderProgram::getUniformLocation("transformMatrix");
	location_projectionMatrix = ShaderProgram::getUniformLocation("projectionMatrix");
	location_viewMatrix = ShaderProgram::getUniformLocation("viewMatrix");
	location_lightPosition = ShaderProgram::getUniformLocation("lightPosition");
	location_lightColor = ShaderProgram::getUniformLocation("lightColor");
	location_shineDamper = ShaderProgram::getUniformLocation("shineDamper");
	location_reflectivity = ShaderProgram::getUniformLocation("reflectivity");
	location_plane = ShaderProgram::getUniformLocation("plane");
}

void StaticShader::loadTransformMatrix(glm::mat4 &p_matrix)
{
	ShaderProgram::loadMatrix(location_transformMatrix, p_matrix);
}

void StaticShader::loadProjectionMatrix(glm::mat4 &p_matrix)
{
	ShaderProgram::loadMatrix(location_projectionMatrix, p_matrix);
}

void StaticShader::loadViewMatrix(Camera &p_camera)
{
	glm::mat4 viewMatrix = Maths::createViewMatrix(p_camera);
	ShaderProgram::loadMatrix(location_viewMatrix, viewMatrix);
}

void StaticShader::loadLight(Light &p_light)
{
	ShaderProgram::loadVector(location_lightPosition, p_light.getPostion());
	ShaderProgram::loadVector(location_lightColor, p_light.getColor());
}

void StaticShader::loadShineVariables(float &p_damper, float &p_reflectivity)
{
	ShaderProgram::loadFloat(location_shineDamper, p_damper);
	ShaderProgram::loadFloat(location_reflectivity, p_reflectivity);
}

void StaticShader::loadClipPlane(glm::fvec4 p_plane)
{
	loadVector(location_plane, p_plane);
}

void StaticShader::loadMVP(glm::mat4 &p_matrix)
{
	ShaderProgram::loadMatrix(location_MVP, p_matrix);
}