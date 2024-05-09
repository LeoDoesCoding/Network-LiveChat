#include <iostream>
#include "Connection.h"
using namespace std;

int main() {
	Connection connection;
	/*wstring IP;
	int port;

	cout << "Please enter the IP to connect to: " << endl;
	wcin >> IP;

	cout << "Please enter the port to connect to: " << endl;
	cin >> port;
	*/
	if (connection.setup()) {
		cout << "Connection was a success!" << endl;
	}
}