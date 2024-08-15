#pragma once
#include "Communication.h"
#include "pch.h"

class Camera {
private:
	Communication* _communication;
	VideoCapture _cap;

public:
	Camera(char* ip, short port);
	~Camera();
	void shotFrame();
	void run();
};