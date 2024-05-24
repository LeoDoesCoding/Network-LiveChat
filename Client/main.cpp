#include <iostream>
#include <thread>
#include "Connection.h"
using namespace std;

int main() {
	Connection connection;
	string inpStr;
	char* input;

	wstring IP;
	int port;

	cout << "Please enter the IP to connect to: " << endl;
	wcin >> IP;
	

	//If connection is successful, initiate the listening loop and allow messages to be sent.
	if (connection.setup(IP)) {
		cout << "You are successfully connected to the server." << endl;
		connection.thread_obj = thread(&Connection::recieveMessage, &connection);

		//Clear input cache, start listening thread.
		cin.ignore();

		cout << "Enter a message: " << endl;		

		//Until "end" is entered, send each message to Connection.
		while (true) {
			cin >> inpStr;
			//getline(cin, inpStr);
			input = new char[inpStr.length()];
			memcpy(input, inpStr.c_str(), inpStr.length());

			connection.sendMessage(input, inpStr.length());
			if (strcmp(input, "end") == 0) {
				break;
			}
			cin.clear();
			delete[] input;
		}
	}

	connection.thread_obj.join();
	return 0;
}