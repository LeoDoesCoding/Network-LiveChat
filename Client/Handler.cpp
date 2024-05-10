#include <iostream>
#include "Connection.h"
using namespace std;

int main() {
	Connection connection;
	wstring IP;
	int port;

	cout << "Please enter the IP to connect to: " << endl;
	wcin >> IP;
	
	if (connection.setup(IP)) {
		cout << "Connection was a success!" << endl;
	}
}