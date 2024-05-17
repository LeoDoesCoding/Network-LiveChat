#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <atlbase.h>
#include <thread>

#include "Connection.h"

using namespace std;

int main() {
	Connection connection;

	connection.setup();
	connection.listenThread.join();

	return 0;
}