#include "glFuncs.h"
#include "DisplayManager.h"

#ifndef WATERFRAMEBUFFERS_H
#define WATERFRAMEBUFFERS_H


class WaterFrameBuffers
{
public:
	WaterFrameBuffers();
	~WaterFrameBuffers();

	void cleanUp();
	void bindReflectionFrameBuffer();
	void bindRefractionFrameBuffer();
	void unbindCurrentFrameBuffer();
	int getReflectionTexture();
	int getRefractionTexture();
	int getRefractionDepthTexture();


private:
	static const int REFLECTION_HEIGHT;
	static const int REFRACTION_HEIGHT;

	GLuint _reflectionFrameBuffer;
	GLuint _reflectionTexture;
	GLuint _reflectionDepthBuffer;
	GLuint _refractionFrameBuffer;
	GLuint _refractionTexture;
	GLuint _refractionDepthTexture;

	void initialiseReflectionFrameBuffer();
	void initialiseRefractionFrameBuffer();
	void bindFrameBuffer(int p_frameBuffer, int p_width, int p_height);
	int createFrameBuffer();
	int createTextureAttachment(int p_width, int p_height);
	int createDepthTextureAttachment(int p_width, int p_height);
	int createDepthBufferAttachment(int p_width, int p_height);

protected:
	static const int REFLECTION_WIDTH;
	static const int REFRACTION_WIDTH;

};


#endif WATERFRAMEBUFFERS_H