#include "Utility.h"
#include "WaterShader.h"
#include "Loader.h"
#include "RawModel.h"
#include "WaterTile.h"
#include "Maths.h"
#include "WaterFrameBuffers.h"
#include "DisplayManager.h"
#include "Light.h"

#ifndef WATER_RENDERER_H
#define WATER_RENDERER_H


class WaterRenderer
{
public:
	explicit WaterRenderer(WaterShader p_shader, glm::mat4 p_projMatrix, WaterFrameBuffers p_fbos, WaterTile& p_quad);
	~WaterRenderer();

	void render(Camera &p_camera, Light &p_light);

private:
	static const float _WAVE_SPEED;
	static const float _HEIGHT;

	WaterTile _quad;
	WaterShader _shader;
	WaterFrameBuffers _fbos;
	float _waveTime = 0.0f;

	void prepare(Camera &p_camera, Light &p_light);
	void unbind();
	void updateTime(); 
	void loadLightVariables(Light p_light);
	//void setUpVAO(Loader p_loader);
};


#endif WATER_RENDERER_H