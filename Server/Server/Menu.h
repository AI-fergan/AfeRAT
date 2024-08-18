#pragma once
#include "pch.h"
#include "Communication.h"

class Menu {
public:
	static int printCameras(Communication* communication);
	static int printRevShells(Communication* communication);
	static int printScreens(Communication* communication);

	static void sendShellByID(Communication* communication, unsigned int id);
	static void sendCameraByID(Communication* communication, unsigned int id);
	static void sendScreenByID(Communication* communication, unsigned int id);

	static int printOptions();

};