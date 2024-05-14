#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <atlbase.h>
#include <thread>

#include "Connection.h"

using namespace std;

int main() {
	Connection connection;

	//If connection is successful, initiate the listening loop
	if (connection.setup()) {
		connection.thread_obj = thread(&Connection::recieveMessage, &connection);
		connection.thread_obj.join();
	}

	return 0;
}