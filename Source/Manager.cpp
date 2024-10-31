#include "../Headers/Manager.h"


//Host
void Manager::startHost() {
	Host connection;
	connection.start(displayMessage);
}

//Client
void Manager::startClient() {
	wstring IP;
	Client connection;
	while(true) {
		cout << "Please enter the IP to connect to: "; //Self note: Read IP, ENTER IP NOT PORT YOU ABSOLUTE WALNUT
		wcin >> IP;
		if(connection.start(displayMessage, IP)) { break; }
		cout << "Cannot make connection." << endl;
	}	
}

void Manager::displayMessage(string message) {
	cout << message << endl;
}