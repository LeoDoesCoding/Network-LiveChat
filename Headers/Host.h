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
        User(SOCKET s) {
            socket = s;
        }

        bool operator==(const User& other) const {
            return socket == other.socket;
        }
    };
    thread host;
    list<User> users;
    mutex usersMutex;

    bool setup();
    void sendMessage(char*, unsigned short) override;
    void recieveMessage(User&);
    void hostMessanger();
    void removeUser(User&);
    void end() override;
public:
    void start(function<void(string)>);
};