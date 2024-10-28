#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <atlbase.h>
#include <thread>

#include "Manager.h"

using namespace std;

int main() {
	/*int choice;

	cout << "Enter the number for the choice you want: \n1. Create server\n2. Join room" << endl;
	cin >> choice;

	while(cin.fail() || (choice != 1 && choice != 2)) {
		cout << "Invalid choice, please enter a corresponding number to the options provided:  " << endl;
		cin >> choice;
	}*/

	Manager::start(1);

	system("pause");
	return 0;
}