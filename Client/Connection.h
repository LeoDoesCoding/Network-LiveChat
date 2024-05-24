#pragma once

#include <iostream>
#include <winsock2.h>
#include <string>
#include <thread>

using namespace std;


class Connection {
private:
    SOCKET clientSocket, acceptSocket;
    int port = 55555;
    bool online = false;
public:
    thread thread_obj;

    bool setup(const wstring IP);
    void end();
    void sendMessage(char* message, unsigned short length);
    void recieveMessage();
};
