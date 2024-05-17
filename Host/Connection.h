#include <iostream>
#include <winsock2.h>
#include <string>
#include <list>
#include <mutex>

using namespace std;


class Connection {
private:
    SOCKET serverSocket, acceptSocket;
    int port = 55555;
    bool online = false;
    struct User {
        SOCKET socket;
        string name;
        User(SOCKET s, string n) {
            socket = s;
            name = n;
        }
    };
    list<User> users;
    list<thread> threads;
    mutex usersMutex;

    bool setup();
    void recieveMessage(User& sender);
public:
    void start();
    void end();
};