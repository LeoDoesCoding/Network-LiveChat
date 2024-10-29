#pragma once
#include <iostream>
#include <winsock2.h>
#include <string>
#include <thread>
#include <functional>

using namespace std;

class Connection {
public:
    SOCKET mySocket, acceptSocket;
    int port = 55555;
    bool online = false;
    function<void(string)> toManager; //Callback

    virtual void sendMessage(char*, unsigned short) = 0;
    virtual void end() = 0;
};