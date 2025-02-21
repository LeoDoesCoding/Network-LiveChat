#pragma once
#include "Connection.h"
#include <ws2tcpip.h>


class Client : public Connection {
private:
	void sendConverted(char*, unsigned short) override;
	void recieveMessage();
	void end() override;
	bool configSet(short) override;

public:
	Client() {
		options.push_back(make_pair("Name", string("USER")));
	}
	bool start(function<void(string)>, const wstring IP);
};