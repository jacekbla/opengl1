#include "DisplayManager.h"

const char* DisplayManager::_TITLE = "openGL";

DisplayManager::DisplayManager(int p_argc, char **p_argv)
{
	glutInit(&p_argc, p_argv);
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
	glutInitWindowSize(_WIDTH, _HEIGHT);
	glutCreateWindow(_TITLE);
	glViewport(0, 0, _WIDTH, _HEIGHT);
}

void DisplayManager::updateDisplay()
{
	glutSwapBuffers();
	glutPostRedisplay();
}

void DisplayManager::closeDisplay()
{
	
}

