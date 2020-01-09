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

void keyboard(unsigned char p_key, int p_x, int p_y)
{
	switch (p_key)
	{
	case 033:
	case 'q':
	case 'Q':
		exit(EXIT_SUCCESS);
		break;
	}
}

RawModel* rawModel_terrain;
TexturedModel* texturedModel_terrain;
Entity* terrain;

RawModel* rawModel_tree;
TexturedModel* texturedModel_tree;
Entity* tree;

RawModel* rawModel_tree2;
TexturedModel* texturedModel_tree2;
Entity* tree2;

RawModel* rawModel_elephant;
TexturedModel* texturedModel_elephant;
Entity* elephant;

Renderer* renderer;
StaticShader* shader;
Camera* camera;
Light* light;
MasterRenderer* masterRenderer;

std::vector<WaterTile>* waterTile;
WaterRenderer* waterRenderer;
WaterShader* waterShader;
WaterFrameBuffers* fbos;

float sunAngle = 0;
float angleDiff = 0.02f;
const float PI = 3.141592653589793238462643383279502884197169f;

glm::fvec3 rotatePoint(glm::fvec3 point, glm::fvec3  center, float angleDeg) {

	float angle = angleDeg * (PI/ 180.0); // deg->rad

	glm::vec3 axis = glm::vec3(0, 0, 1); // any perpendicular vector to `p1-p0` if `p1-p0` is parallel to (0,0,1) then use `(0,1,0)` instead
	// construct transform matrix
	glm::mat4 m = glm::identity<glm::mat4>(); // unit matrix
	m = glm::translate(m, +center);
	m = glm::rotate(m, angle, axis);
	m = glm::translate(m, -center); // here m should be your rotation matrix
	// use transform matrix
	glm::vec3 rotatedPoint = glm::vec4(point, 1.0) * m;
	return rotatedPoint;
}

void changeLight() {
	glm::fvec3 lightRed(0.62, 0.38, 0);
	glm::fvec3 lightBlue(0.45f, 0.65f, 0.95f);
	glm::fvec3 lightWhite(1.0f, 1.0f, 1.0f);

	float angle = angleDiff * (PI / 180.0);
	sunAngle += angle;

	if (sunAngle > 1.5707963268 && sunAngle < 4.7123889804) {
		lightBlue = glm::vec3(0.0f, 0.0f, 0.0f);
		lightWhite = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	
	lightRed = glm::fvec3(lightRed.x * glm::sin(sunAngle)* glm::sin(sunAngle), lightRed.y * glm::sin(sunAngle)* glm::sin(sunAngle), lightRed.z * glm::sin(sunAngle)* glm::sin(sunAngle));
	lightWhite = glm::fvec3(lightWhite.x * glm::cos(sunAngle)* glm::cos(sunAngle), lightWhite.y * glm::cos(sunAngle)* glm::cos(sunAngle), lightWhite.z * glm::cos(sunAngle)* glm::cos(sunAngle));
	lightBlue = glm::fvec3(lightBlue.x * glm::cos(sunAngle) * glm::cos(sunAngle), lightBlue.y * glm::cos(sunAngle) * glm::cos(sunAngle), lightBlue.z * glm::cos(sunAngle) * glm::cos(sunAngle));

	glm::fvec3 lightColor = glm::fvec3(lightRed.x + lightWhite.x, lightRed.y + lightWhite.y, lightRed.z + lightWhite.z);
	glm::fvec3 skyColor = glm::fvec3(lightRed.x + lightBlue.x, lightRed.y + lightBlue.y, lightRed.z + lightBlue.z);

	if (sunAngle > 6.2831853072) {
		sunAngle -= 6.2831853072;
	}

	light->setColor(lightColor);
	light->setSkyColor(skyColor);
}


void display(void)
{
	//entity->increasePosition(0.0f, 0.0f, -0.002f);
	//entity->increaseRotation(0.0f, 1.0f, 0.0f);
	camera->move();

	glm::vec3 center(0.0f, 0.0f, -8.0f);
	light->setPostion(rotatePoint(light->getPostion(), center, angleDiff));
	//elephant->setPosition(rotatePoint(light->getPostion(), center, angleDiff));
	changeLight();

	glEnable(GL_CLIP_DISTANCE0);

	fbos->bindReflectionFrameBuffer();
	float distance = 2 * (camera->getPosition().y - waterTile->at(0).getHeight());
	float originalCameraY = camera->getPosition().y;
	camera->setPosition(glm::vec3(camera->getPosition().x, originalCameraY - distance, camera->getPosition().z));
	camera->invertPitch();
	masterRenderer->processEntity(*terrain);
	masterRenderer->processEntity(*tree);
	masterRenderer->processEntity(*tree2);
	masterRenderer->processEntity(*elephant);
	masterRenderer->render(*light, *camera, glm::fvec4(0.0f, 1.0f, 0.0f, -waterTile->at(0).getHeight()));
	camera->setPosition(glm::vec3(camera->getPosition().x, originalCameraY, camera->getPosition().z));
	camera->invertPitch();
	fbos->unbindCurrentFrameBuffer();

	fbos->bindRefractionFrameBuffer();
	masterRenderer->processEntity(*terrain);
	masterRenderer->processEntity(*tree);
	masterRenderer->processEntity(*tree2);
	masterRenderer->processEntity(*elephant);
	masterRenderer->render(*light, *camera, glm::fvec4(0.0f, -1.0f, 0.0f, waterTile->at(0).getHeight()));


	glDisable(GL_CLIP_DISTANCE0);
	fbos->unbindCurrentFrameBuffer();


	masterRenderer->processEntity(*terrain);
	masterRenderer->processEntity(*tree);
	masterRenderer->processEntity(*tree2);
	masterRenderer->processEntity(*elephant);
	masterRenderer->render(*light, *camera, glm::fvec4(0.0f, -1.0f, 0.0f, 100.0f));
	waterRenderer->render(*waterTile, *camera);

	DisplayManager::updateDisplay();
}



int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	DisplayManager::createDisplay();

	Loader loader;

	rawModel_terrain = &loadOBJ("res/lake1.obj", loader);
	ModelTexture texture(loader.loadTexture("res/green.bmp"));
	texture.setShineDamper(30.0f);
	texture.setReflectivity(0.4f);
	texturedModel_terrain = new TexturedModel(*rawModel_terrain, texture);

	rawModel_tree = &loadOBJ("res/tree_noleaves.obj", loader);
	ModelTexture texture_tree(loader.loadTexture("res/brown.bmp"));
	texture_tree.setShineDamper(20.0f);
	texture_tree.setReflectivity(0.3f);
	texturedModel_tree = new TexturedModel(*rawModel_tree, texture_tree);

	rawModel_tree2 = &loadOBJ("res/tree_noleaves.obj", loader);
	ModelTexture texture_tree2(loader.loadTexture("res/brown.bmp"));
	texture_tree2.setShineDamper(20.0f);
	texture_tree2.setReflectivity(0.3f);
	texturedModel_tree2 = new TexturedModel(*rawModel_tree2, texture_tree2);

	rawModel_elephant = &loadOBJ("res/elephun.obj", loader);
	ModelTexture texture2(loader.loadTexture("res/empty.bmp"));
	texture2.setShineDamper(10.0f);
	texture2.setReflectivity(0.0f);
	texturedModel_elephant = new TexturedModel(*rawModel_elephant, texture2);

	shader = new StaticShader();
	terrain = new Entity(*texturedModel_terrain, glm::vec3(0.0f, -3.0f, -7.0f), 0.0f, 0.0f, 0.0f, 1.0f);
	tree = new Entity(*texturedModel_tree, glm::vec3(-4.0f, -1.5f, -12.0f), 0.0f, 40.0f, 0.0f, 1.0f);
	tree2 = new Entity(*texturedModel_tree2, glm::vec3(1.4f, -1.9f, -13.0f), 0.0f, 150.0f, 0.0f, 1.3f);
	elephant = new Entity(*texturedModel_elephant, glm::vec3(0.0f, -1.0f, -12.0f), 5.0f, 225.0f, 0.0f, 0.3f);
	light = new Light(glm::vec3(0.0f, 10.0f, -8.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.4f, 0.6f, 0.9f));
	camera = new Camera(2.0f, 0.0f, 0.0f);
	renderer = new Renderer(*shader);
	masterRenderer = new MasterRenderer();
	//center
	//(0.0f, 0.0f, -7.0f)
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
	delete light;
	delete terrain;
	delete texturedModel_terrain;
	delete elephant;
	delete texturedModel_elephant;
	delete tree;
	delete tree2;
	delete texturedModel_tree;
	delete texturedModel_tree2;
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