#include "DisplayManager.h"

DisplayManager::DisplayManager(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
}

DisplayManager::DisplayManager()
{
	glutInit(nullptr, nullptr);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
}

DisplayManager::~DisplayManager()
{
}

void DisplayManager::createDisplay()
{
	const char* TITLE = "openGL";

	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow(TITLE);
	glViewport(0, 0, WIDTH, HEIGHT);
}

void DisplayManager::updateDisplay()
{
	glutSwapBuffers();
	glutPostRedisplay();
}

void DisplayManager::closeDisplay()
{
	
}

