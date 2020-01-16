#include "WaterRenderer.h"


const char* WaterRenderer::_DUDV_MAP = "res/dudv/dudv_water_map4.bmp";
const float WaterRenderer::_WAVE_SPEED = 0.01f;
const float WaterRenderer::_HEIGHT = 0.02f;

WaterRenderer::WaterRenderer(Loader p_loader, WaterShader p_shader, glm::mat4 p_projMatrix, WaterFrameBuffers p_fbos, WaterTile& p_quad)
{
	_quad = p_quad;
	_shader = p_shader;
	_fbos = p_fbos;
	_dudvTexture = p_loader.loadTexture(_DUDV_MAP);
	_shader.start();
	_shader.connectTextureUnits();
	_shader.loadProjectionMatrix(p_projMatrix);
	_shader.stop();
	//setUpVAO(p_loader);
	_moveFactor = 0.0f;
}

WaterRenderer::~WaterRenderer()
{
}

void WaterRenderer::render(Camera & p_camera, Light &p_light)
{
	prepare(p_camera, p_light);

	glm::mat4 modelMatrix = Maths::createTransformMatrix(glm::fvec3(_quad.getX(), _quad.getHeight(), _quad.getZ()), 0.0f, 0.0f, 0.0f, WaterTile::TILE_SIZE);
	_shader.loadModelMatrix(modelMatrix);

	glDrawArrays(GL_TRIANGLES, 0, _quad.getVertexCount());

	unbind();
}

void WaterRenderer::prepare(Camera & p_camera, Light &p_light)
{
	_shader.start();
	updateTime();
	_shader.loadViewMatrix(p_camera);
	_moveFactor += _WAVE_SPEED * DisplayManager::getFrameTimeSeconds().count();
	_moveFactor = _moveFactor - floor(_moveFactor);
	_shader.loadMoveFactor(_moveFactor);
	_shader.loadHeight(_HEIGHT);
	loadLightVariables(p_light);
	glFuncs::ref().glBindVertexArray(_quad.getVaoID());
	glFuncs::ref().glEnableVertexAttribArray(0);
	glFuncs::ref().glEnableVertexAttribArray(1);
	glFuncs::ref().glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _fbos.getReflectionTexture());
	glFuncs::ref().glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _fbos.getRefractionTexture());
	glFuncs::ref().glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, _dudvTexture);
	glFuncs::ref().glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, _fbos.getRefractionDepthTexture());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void WaterRenderer::unbind()
{
	glDisable(GL_BLEND);
	glFuncs::ref().glDisableVertexAttribArray(0);
	glFuncs::ref().glDisableVertexAttribArray(1);
	glFuncs::ref().glBindVertexArray(0);
	_shader.stop();
}

void WaterRenderer::updateTime()
{
	_waveTime += _WAVE_SPEED;
	_shader.loadWaveTime(_waveTime);
}

void WaterRenderer::loadLightVariables(Light p_light)
{
	_shader.loadLightPosition(p_light.getPostion());
	_shader.loadLightColour(p_light.getColor());
}

//void WaterRenderer::setUpVAO(Loader p_loader)
//{
//	std::vector<float> vertices = { -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f };
//	_quad = p_loader.loadToVAO(vertices, 2);
//}
