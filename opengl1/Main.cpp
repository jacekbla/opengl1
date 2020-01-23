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
#include "WaterGenerator.h"
#include "Tess.h"

bool useTesselation = false;
bool onlyEdges = false;

void setAngle(float angleDeg);
bool sunRotation=true;

void keyboard(unsigned char p_key, int p_x, int p_y)
{
	switch (p_key)
	{
	case 033:
	case 'q':
	case 'Q':
		exit(EXIT_SUCCESS);
		break;
	case '1':
		onlyEdges = true;
		break;
	case '2':
		onlyEdges = false;
		break;

	case 'j':
		useTesselation = false;
		break;
	case 'k':
		useTesselation = true;
		break;
	case '+':
		setAngle(90);
		break;
	case '-':
		setAngle(-90);
		break;
	case '0':
		sunRotation = !sunRotation;
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

RawModel* rawModel_sun;
TexturedModel* texturedModel_sun;
Entity* sun;

RawModel* rawModel_lamp;
TexturedModel* texturedModel_lamp;
Entity* lamp;

Renderer* renderer;
StaticShader* shader;
Camera* camera;
Light* light;
std::vector<Light*>* lights;
MasterRenderer* masterRenderer;

WaterTile* waterTile;
WaterRenderer* waterRenderer;
WaterShader* waterShader;
WaterFrameBuffers* fbos;


Tess* tess;

float sunAngle = 0;
float angleDiff = 0.02f;
const float PI = 3.141592653589793238462643383279502884197169f;
float changedDiff=false;

void setAngle(float angleDeg) {
	sunAngle += angleDeg * (PI / 180.0);
	angleDiff += angleDeg;
	changedDiff = true;
}


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

void changeLight(Light* sunLight,Entity* sunEntity, glm::vec3 center) 
{

	if (sunRotation || changedDiff) {

		if(sunAngle > PI* 2)
			sunAngle-= PI*2;

		sunLight->setPostion(rotatePoint(sunLight->getPostion(), center, angleDiff));
		sunEntity->setPosition(sunLight->getPostion());

		if (changedDiff) {
			angleDiff = 0.02f;
			changedDiff = false;
		}
		else {
			sunAngle += angleDiff * (PI / 180.0);
		}
		
	}

	glm::fvec3 lightRed(0.62, 0.38, 0);
	glm::fvec3 lightBlue(0.45f, 0.65f, 0.95f);
	glm::fvec3 lightBlack(1.0f, 1.0f, 1.0f);

	if (sunAngle > PI/2 && sunAngle < (3*PI)/2) {
		lightBlue = glm::vec3(0.0f, 0.0f, 0.0f);
		lightBlack = glm::vec3(0.2f, 0.2f, 0.2f);
	}
	float sin2 = pow(glm::sin(sunAngle), 2);
	float cos2 = pow(glm::cos(sunAngle), 2);

	
	lightRed = glm::fvec3(lightRed.x * sin2, lightRed.y * sin2, lightRed.z * sin2);
	lightBlack = glm::fvec3(lightBlack.x * cos2, lightBlack.y * cos2, lightBlack.z * cos2);
	lightBlue = glm::fvec3(lightBlue.x * cos2, lightBlue.y * cos2, lightBlue.z * cos2);

	glm::fvec3 lightColor = glm::fvec3(lightRed.x + lightBlack.x, lightRed.y + lightBlack.y, lightRed.z + lightBlack.z);
	glm::fvec3 skyColor = glm::fvec3(lightRed.x + lightBlue.x, lightRed.y + lightBlue.y, lightRed.z + lightBlue.z);

	sunLight->setColor(lightColor);
	sunLight->setSkyColor(skyColor);
}

void display(void)
{

	camera->move();

	glm::vec3 center(0.0f, 0.0f, -8.0f);
	changeLight(light, sun, center);


	glEnable(GL_CLIP_DISTANCE0);

	//render reflection texture to fbo

	fbos->bindReflectionFrameBuffer();
	float distance = 2 * (camera->getPosition().y - waterTile->getHeight());
	float originalCameraY = camera->getPosition().y;
	camera->setPosition(glm::vec3(camera->getPosition().x, originalCameraY - distance, camera->getPosition().z));
	camera->invertPitch();
	masterRenderer->processEntity(*terrain);
	masterRenderer->processEntity(*tree);
	masterRenderer->processEntity(*tree2);
	masterRenderer->processEntity(*elephant);
	masterRenderer->processEntity(*sun);
	masterRenderer->processEntity(*lamp);
	masterRenderer->render(*lights, *camera, glm::fvec4(0.0f, 1.0f, 0.0f, -waterTile->getHeight() + 0.5f));
	camera->setPosition(glm::vec3(camera->getPosition().x, originalCameraY, camera->getPosition().z));
	camera->invertPitch();
	fbos->unbindCurrentFrameBuffer();


	//render refraction texture to fbo
	fbos->bindRefractionFrameBuffer();
	masterRenderer->processEntity(*terrain);
	masterRenderer->processEntity(*tree);
	masterRenderer->processEntity(*tree2);
	masterRenderer->processEntity(*elephant);
	masterRenderer->processEntity(*sun);
	masterRenderer->processEntity(*lamp);
	masterRenderer->render(*lights, *camera, glm::fvec4(0.0f, -1.0f, 0.0f, waterTile->getHeight() + 0.5f));



	//render to screen
	glDisable(GL_CLIP_DISTANCE0);
	fbos->unbindCurrentFrameBuffer();
	masterRenderer->processEntity(*terrain);
	masterRenderer->processEntity(*tree);
	masterRenderer->processEntity(*tree2);
	masterRenderer->processEntity(*elephant);
	masterRenderer->processEntity(*sun);
	masterRenderer->processEntity(*lamp);

	masterRenderer->render(*lights, *camera, glm::fvec4(0.0f, -1.0f, 0.0f, -1.0f));

	//masterRenderer->processEntity(*elephant);
	//masterRenderer->render(*lights, *camera, glm::fvec4(0.0f, -1.0f, 0.0f, 100.0f));
	//waterRenderer->render(*camera, *lights);

	// water rendering //

	if (onlyEdges) // press [1]
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else // press [2]
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (useTesselation) // press [k] (krzysztof)
	{
		tess->render(*camera, *lights, false);
	}
	else // press [j] (jacek)
	{
		waterRenderer->render(*camera, *lights);
	}

	DisplayManager::updateDisplay();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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

	rawModel_sun = &loadOBJ("res/sun.obj", loader);
	ModelTexture texture3(loader.loadTexture("res/white.bmp"));
	texture3.setShineDamper(30.0f);
	texture3.setReflectivity(0.4f);
	texturedModel_sun = new TexturedModel(*rawModel_sun, texture3);

	rawModel_lamp = &loadOBJ("res/lamp.obj", loader);
	//ModelTexture texture2(loader.loadTexture("res/empty.bmp"));
	texture2.setShineDamper(30.0f);
	texture2.setReflectivity(0.5f);
	texturedModel_lamp = new TexturedModel(*rawModel_lamp, texture2);

	shader = new StaticShader();
	terrain = new Entity(*texturedModel_terrain, glm::vec3(0.0f, -3.0f, -7.0f), 0.0f, 0.0f, 0.0f, 1.0f);
	tree = new Entity(*texturedModel_tree, glm::vec3(-4.0f, -1.5f, -12.0f), 0.0f, 40.0f, 0.0f, 1.0f);
	tree2 = new Entity(*texturedModel_tree2, glm::vec3(1.4f, -1.9f, -13.0f), 0.0f, 150.0f, 0.0f, 1.3f);
	elephant = new Entity(*texturedModel_elephant, glm::vec3(0.0f, -1.0f, -11.0f), 5.0f, 225.0f, 0.0f, 0.3f);
	light = new Light(glm::vec3(0.0f, 10.0f, -8.0f), glm::vec3(0.9f, 0.9f, 0.9f), glm::vec3(0.9f, 0.9f, 0.9f), 0.85f);
	sun = new Entity(*texturedModel_sun, glm::vec3(-4.0f, -1.5f, -12.0f), 0.0f, 0.0f, 0.0f, 0.5f);
	lamp = new Entity(*texturedModel_lamp, glm::vec3(-4.0f, -1.5f, -9.0f), 0.0f, 0.0f, 0.0f, 0.5f);
	camera = new Camera();

	renderer = new Renderer(*shader);
	
	lights = new std::vector<Light*>();
	lights->push_back(light);

	Light* light2 = new Light(glm::vec3(-4.0f, 1.6f, -9.0f), glm::vec3(0.9f, 0.9f, 0.9f), glm::vec3(0.0f, 0.0f, 0.0f), 0.03f);
	lights->push_back(light2);

	masterRenderer = new MasterRenderer();

	//water
	waterTile = &WaterGenerator::generate(200, -5.0f, -2.5f, -12.0f, loader);

	fbos = new WaterFrameBuffers();
	waterShader = new WaterShader();
	waterRenderer = new WaterRenderer(*waterShader, renderer->getProjectionMatrix(), *fbos, *waterTile);

	tess = new Tess(loader, renderer->getProjectionMatrix(), *waterTile, *fbos);

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);

	glutMainLoop();


	fbos->cleanUp();
	waterShader->cleanUp();
	masterRenderer->cleanUp();
	loader.cleanUp();

	tess->cleanUp();

	for (int i = 0; i < lights->size(); i++) {
		delete& lights[i];
	}

	delete camera;
	delete light;
	delete light2;
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

	delete tess;

	DisplayManager::closeDisplay();

	return 0;
}