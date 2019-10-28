#include "Utility.h"
#include "DisplayManager.h"
#include "Loader.h"
#include "Renderer.h"
#include "StaticShader.h"
#include "ModelTexture.h"
#include "TexturedModel.h"
#include "Camera.h"
#include "OBJLoader.h"
#include "Light.h"
#include "MasterRenderer.h"

#include "WaterShader.h"
#include "WaterRenderer.h"
#include "WaterTile.h"
#include "WaterFrameBuffers.h"

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 033:
	case 'q':
	case 'Q':
		exit(EXIT_SUCCESS);
		break;
	}
}

RawModel* rawModel;
TexturedModel* texturedModel;
Entity* entity;

RawModel* rawModel2;
TexturedModel* texturedModel2;
Entity* entity2;

Renderer* renderer;
StaticShader* shader;
Camera* camera;
Light* light;
MasterRenderer* masterRenderer;

std::vector<WaterTile>* waterTile;
WaterRenderer* waterRenderer;
WaterShader* waterShader;
WaterFrameBuffers* fbos;

void display(void)
{
	//entity->increasePosition(0.0f, 0.0f, -0.002f);
	//entity->increaseRotation(0.0f, 1.0f, 0.0f);
	camera->move();

	glEnable(GL_CLIP_DISTANCE0);

	fbos->bindReflectionFrameBuffer();
	float distance = 2 * (camera->getPosition().y - waterTile->at(0)._getHeight());
	float originalCameraY = camera->getPosition().y;
	camera->setPosition(glm::vec3(camera->getPosition().x, originalCameraY - distance, camera->getPosition().z));
	camera->invertPitch();
	masterRenderer->processEntity(*entity);
	masterRenderer->processEntity(*entity2);
	masterRenderer->render(*light, *camera, glm::fvec4(0.0f, 1.0f, 0.0f, -waterTile->at(0)._getHeight()));
	camera->setPosition(glm::vec3(camera->getPosition().x, originalCameraY, camera->getPosition().z));
	camera->invertPitch();
	fbos->unbindCurrentFrameBuffer();

	fbos->bindRefractionFrameBuffer();
	masterRenderer->processEntity(*entity);
	masterRenderer->processEntity(*entity2);
	masterRenderer->render(*light, *camera, glm::fvec4(0.0f, -1.0f, 0.0f, waterTile->at(0)._getHeight()));


	glDisable(GL_CLIP_DISTANCE0);
	fbos->unbindCurrentFrameBuffer();


	masterRenderer->processEntity(*entity);
	masterRenderer->processEntity(*entity2);
	masterRenderer->render(*light, *camera, glm::fvec4(0.0f, -1.0f, 0.0f, -1.0f));
	waterRenderer->render(*waterTile, *camera);

	DisplayManager::updateDisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	DisplayManager::createDisplay();

	Loader loader;

	rawModel = &loadOBJ("res/lake1.obj", loader);
	ModelTexture texture(loader.loadTexture("res/green.bmp"));
	texture.setShineDamper(30.0f);
	texture.setReflectivity(0.3f);
	texturedModel = new TexturedModel(*rawModel, texture);

	rawModel2 = &loadOBJ("res/elephun.obj", loader);
	ModelTexture texture2(loader.loadTexture("res/empty.bmp"));
	texture2.setShineDamper(10.0f);
	texture2.setReflectivity(5.0f);
	texturedModel2 = new TexturedModel(*rawModel2, texture2);

	shader = new StaticShader();
	entity = new Entity(*texturedModel, glm::vec3(0.0f, -3.0f, -7.0f), 0.0f, 0.0f, 0.0f, 1.0f);
	entity2 = new Entity(*texturedModel2, glm::vec3(0.0f, -1.0f, -11.0f), 0.0f, 225.0f, 0.0f, 0.3f);
	light = new Light(glm::vec3(5.0f, -10.0f, -12.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	camera = new Camera(2.0f, 0.0f, 0.0f);
	renderer = new Renderer(*shader);
	
	masterRenderer = new MasterRenderer();

	//water
	fbos = new WaterFrameBuffers();
	waterShader = new WaterShader();
	waterRenderer = new WaterRenderer(loader, *waterShader, renderer->getProjectionMatrix(), *fbos);
	waterTile = new std::vector<WaterTile>({ WaterTile(0.0f, -7.0f, -2.5f) });

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);

	glutMainLoop();


	fbos->cleanUp();
	waterShader->cleanUp();
	masterRenderer->cleanUp();
	loader.cleanUp();
	delete camera;
	delete entity;
	delete light;
	delete texturedModel;
	delete shader;
	delete renderer;
	delete masterRenderer;
	delete waterTile;
	delete waterShader;
	delete waterRenderer;
	delete fbos;
	DisplayManager::closeDisplay();

	return 0;
}