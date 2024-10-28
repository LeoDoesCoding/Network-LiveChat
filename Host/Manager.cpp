//Maintains connection to client
#include <iostream>
#include "Manager.h"

using namespace std;

void Manager::start(int choice) {
	Connection connection;
	switch (choice) {
	case 1:
		connection.start(displayMessage); //Set callback, start Connection.
		break;
	case 2:
		break;
	}
}

void Manager::displayMessage(string message) {
	cout << message << endl;
}