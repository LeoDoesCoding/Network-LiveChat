#include "../Headers/Manager.h"


//Host
void Manager::start() {
	Host connection;
	connection.start(displayMessage);
}

//Client
void Manager::start(const wstring IP) {
	Client connection;
	connection.start(displayMessage, IP);
}

void Manager::displayMessage(string message) {
	cout << message << endl;
}