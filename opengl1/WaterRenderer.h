#include "Utility.h"
#include "WaterShader.h"
#include "Loader.h"
#include "RawModel.h"
#include "WaterTile.h"
#include "Maths.h"
#include "WaterFrameBuffers.h"
#include "DisplayManager.h"

#ifndef WATER_RENDERER_H
#define WATER_RENDERER_H


class WaterRenderer
{
public:
	explicit WaterRenderer(Loader p_loader, WaterShader p_shader, glm::mat4 p_projMatrix, WaterFrameBuffers p_fbos, WaterTile& p_quad);
	~WaterRenderer();

	void render(Camera &p_camera);

private:
	static const char* _DUDV_MAP;
	static const float _WAVE_SPEED;
	static const float _HEIGHT;

	WaterTile _quad;
	WaterShader _shader;
	WaterFrameBuffers _fbos;
	float _moveFactor = 0.0f;
	float _waveTime = 0.0f;
	int _dudvTexture;

	void prepare(Camera &p_camera);
	void unbind();
	void updateTime();
	//void setUpVAO(Loader p_loader);
};


#endif WATER_RENDERER_H