#include "../Headers/Manager.h"

using namespace std;

int main() {
	int choice;

	cout << "Enter the number for the choice you want: \n1. Create server\n2. Join room" << endl;
	cin >> choice;

	while(cin.fail() || (choice != 1 && choice != 2)) {
		cout << "Invalid choice, please enter a corresponding number to the options provided:  " << endl;
		cin >> choice;
	}

	switch(choice) {
	case 1:
		Manager::startHost();
		break;
	case 2:
		Manager::startClient();
		break;
	}

	return 0;
}