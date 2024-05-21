#include <iostream>
#include <thread>
#include "Connection.h"
using namespace std;

int main() {
	Connection connection;
	char input[201];

	wstring IP;
	int port;

	cout << "Please enter the IP to connect to: " << endl;
	wcin >> IP;
	

	//If connection is successful, initiate the listening loop and allow messages to be sent.
	if (connection.setup(IP)) {
		cout << "Connection was a success!" << endl;
		connection.thread_obj = thread(&Connection::recieveMessage, &connection);

		//Clear input cache, start listening thread.
		cin.ignore();

		cout << "Enter a message: " << endl;
		//char input[200];
		

		//Until "end" is entered, send each message to Connection.
		while (true) {
			cin.getline(input, 200, '\n');

			connection.sendMessage(input);
			if (strcmp(input, "end") == 0) {
				break;
			}
			cin.clear();
		}
	}

	connection.thread_obj.join();
	return 0;
}