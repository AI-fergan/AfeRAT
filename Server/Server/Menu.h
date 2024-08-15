#pragma once
#include "pch.h"
#include "Communication.h"

class Menu {
public:
	static int printCameras(Communication* communication);
	static int printRevShells(Communication* communication);
	static int printOptions();
	static void sendShellByID(Communication* communication, unsigned int id);
	static void sendCameraByID(Communication* communication, unsigned int id);
};