#include "Utility.h"

#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

class DisplayManager
{
public:

	DisplayManager();
	DisplayManager(int argc, char **argv);
	~DisplayManager();

	static void createDisplay();
	static void updateDisplay();
	static void closeDisplay();

	static unsigned int getWIDTH() { return WIDTH; };
	static unsigned int getHEIGHT() { return HEIGHT; };

private:
	const static unsigned int WIDTH = 800;
	const static unsigned int HEIGHT = 600;
	const static unsigned int FPS_CAP;
};

#endif DISPLAY_MANAGER_H