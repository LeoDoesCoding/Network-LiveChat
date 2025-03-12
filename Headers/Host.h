#pragma once
#include "Connection.h"
#include <list>
#include <ctime>

class Host : public Connection {
private:
    struct User {
        SOCKET socket;
        string name = "USER";
        User(SOCKET s) {
            socket = s;
        }

        bool operator==(const User& other) const {
            return socket == other.socket;
        }
    };
    list<User> users;
    mutex usersMutex;

    bool setup();
    void sendConverted(char*, unsigned short) override;
    void recieveMessage(User&);
    void removeUser(User&);
    void end() override;
    bool configSet(short) override;

public:
    void start(function<void(string)>);
};