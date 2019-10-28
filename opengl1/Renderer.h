#include "Utility.h"
#include "RawModel.h"
#include "TexturedModel.h"
#include "Entity.h"
#include "StaticShader.h"
#include "Maths.h"
#include "DisplayManager.h"
#include <map>

#ifndef RENDERER_H
#define RENDERER_H

class Renderer
{
public:
	Renderer(StaticShader shader);
	~Renderer();

	void prepare();
	void render(std::map<TexturedModel, std::vector<Entity>> p_entities);
	void renderRaw(RawModel model);
	void renderTextured(TexturedModel texturedModel);
	void renderEntity(Entity p_entity, StaticShader p_shader);

	glm::mat4 getProjectionMatrix();

private:
	static const float FOV;
	static const float NEAR_PLANE;
	static const float FAR_PLANE;

	StaticShader _shader;
	glm::mat4 projectionMatrix;

	void createProjectionMatrix();
	void prepareTexturedModel(TexturedModel p_model);
	void unbindTexturedModel();
	void prepareInstance(Entity p_entity);
};

#endif RENDERER_H