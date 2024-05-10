// C++ LiveChat.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
//#include "stdafx.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <atlbase.h>

#include "Connection.h"

using namespace std;


int main() {
	Connection connection;
	connection.setup();
}