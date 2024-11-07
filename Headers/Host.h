#pragma once
#include "Connection.h"
#include <mutex>
#include <list>


class Host : public Connection {
private:
    //const char* name = "USER"; //Host's name
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
    void sendMessage(char*, unsigned short) override;
    void prepMsg(string) override;
    void recieveMessage(User&);
    void removeUser(User&);
    void end() override;
    bool configSet(short) override;

public:
    Host() {
        options.push_back(make_pair("Name", string("HOST")));
        options.push_back(make_pair("Log state", "NOLOG"));
    }
    void start(function<void(string)>);
};