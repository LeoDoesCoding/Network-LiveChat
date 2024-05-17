#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <atlbase.h>
#include <thread>

#include "Connection.h"

using namespace std;

//In theory, this can be merged onto Connection.cpp and make its start() the main().
//For practice, I will stick with this file.
int main() {
	Connection connection;

	connection.start();

	return 0;
}