#pragma once
#include "pch.h"

#pragma comment(lib, "ws2_32.lib")

class Communication {
	char* _ip;
	short _port;
	SOCKET _sock;

public:
	Communication(char* ip, short port);
	~Communication();
	bool connect();
	string getMsg();
	void sendMsg(const string& results);
	void sendFile(const string& filename);
};