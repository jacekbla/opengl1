#include "Utility.h"
#include "WaterShader.h"
#include "Loader.h"
#include "RawModel.h"
#include "WaterTile.h"
#include "Maths.h"
#include "WaterFrameBuffers.h"

#ifndef WATERRENDERER_H
#define WATERRENDERER_H


class WaterRenderer
{
public:
	WaterRenderer(Loader p_loader, WaterShader p_shader, glm::mat4 p_projMatrix, WaterFrameBuffers p_fbos);
	~WaterRenderer();

	void render(std::vector<WaterTile> p_water, Camera &p_camera);

private:
	RawModel _quad;
	WaterShader _shader;
	WaterFrameBuffers _fbos;

	void prepare(Camera &p_camera);
	void unbind();
	void setUpVAO(Loader p_loader);
};


#endif WATERRENDERER_H