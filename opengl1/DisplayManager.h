#include "Utility.h"

#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

class DisplayManager
{
public:
	explicit DisplayManager();
	DisplayManager(int p_argc, char **p_argv);
	~DisplayManager();

	static void createDisplay();
	static void updateDisplay();
	static void closeDisplay();

	static unsigned int getWIDTH() { return _WIDTH; };
	static unsigned int getHEIGHT() { return _HEIGHT; };

private:
	const static char* _TITLE;
	const static unsigned int _WIDTH = 800;
	const static unsigned int _HEIGHT = 600;
};

#endif DISPLAY_MANAGER_H