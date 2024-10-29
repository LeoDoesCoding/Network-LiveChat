#pragma once
#include "Connection.h"
#include <mutex>
#include <list>


class Host : public Connection {
private:
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
    void sendMessage(char*, unsigned short) override;
    void recieveMessage(User&);
    void hostMessanger();
    void end() override;
public:
    void start(function<void(string)>);
};