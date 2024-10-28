#pragma once
#include <iostream>
#include <winsock2.h>
#include <string>
#include <list>
#include <mutex>
#include <thread>
#include <functional>

using namespace std;


class Connection {
private:
    SOCKET serverSocket, acceptSocket;
    int port = 55555;
    bool online = false;
    const char* name = "USER"; //Host's name
    struct User {
        SOCKET socket;
        const char* name = "USER";
        thread listen;
        User(SOCKET s) {
            socket = s;
        }
    };
    list<User> users;
    list<thread> threads;
    mutex usersMutex;

    bool setup();
    void recieveMessage(User& sender);
    void hostMessanger();
    void sendMessage(char*);

    function<void(string)> toManager; //Callback
public:
    void start(function<void(string)>);
    void end();
};