#pragma once
#include "Connection.h"
#include <ws2tcpip.h>


class Client : public Connection {
private:
	thread thread_obj;
	void sendMessage(char*, unsigned short) override;
	void recieveMessage();
	void end() override;
public:
	bool start(function<void(string)>, const wstring IP);
};