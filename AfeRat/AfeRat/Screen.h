#pragma once
#include "Communication.h"
#include "pch.h"

class Screen {
private:
	Communication* _communication;
	HWND _screen;

public:
	Screen(char* ip, short port);
	void shotScreen();
	void run();
};