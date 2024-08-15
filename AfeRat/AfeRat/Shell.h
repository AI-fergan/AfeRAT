#pragma once
#include "pch.h"
#include "Communication.h"

class Shell {
private:
	Communication* _communication;

public:
	Shell(char* ip, short port);
	string execCommand(const string command);

	void run();
};
