#pragma once
#include "Connection.h"
#include <mutex>
#include <list>
#include <ctime>

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
    char logName[13]; //Only used for persistant log

    bool setup();
    void sendConverted(char*, unsigned short) override;
    void recieveMessage(User&);
    void removeUser(User&);
    void end() override;
    bool configSet(short) override;
    string logString() {
        switch(get<LogState>(options[1].second)) {
        case NOLOG: return "No log";
        case TEMPORARY: return "Temporary";
        case PERSISTANT: return "Text file";
        default: return "Unknown";
        }
    }

public:
    Host() {
        options.push_back(make_pair("Name", string("HOST")));
        options.push_back(make_pair("Log state", NOLOG));
        time_t timestamp = time(nullptr);
        struct tm datetime;
        localtime_s(&datetime, &timestamp);
        strftime(logName, sizeof(logName), "%m%d%Y.%H%M", &datetime);
        cout << logName << endl;
    }
    void start(function<void(string)>);
};