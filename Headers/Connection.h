#pragma once
#include <iostream>
#include <winsock2.h>
#include <string>
#include <thread>
#include <functional>
#include <vector>
#include <variant>

using namespace std;

class Connection {
protected:
    SOCKET mySocket, acceptSocket;
    int port = 55555;
    bool online = false;
    bool ready = true; //Disabled during config
    function<void(string)> toManager; //Callback

    void handleInput();
    void config();

    virtual void sendMessage(char*, unsigned short) = 0;
    virtual void prepMsg(string) = 0;
    virtual void end() = 0;

    //Config options
    enum LogState { //Server log type
        NOLOG, //No log is kept
        SERVER, //Server keeps log, client retrives log
        CLIENT //Client keeps personal log
    };
    vector<pair<string, variant<string, bool, LogState>>> options; //description, value
    string getName() { return get<string>(options[0].second); }
    virtual bool configSet(short) = 0; //Derivative-specific additional config handling
    
public:
};