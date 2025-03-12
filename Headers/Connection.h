#pragma once
#include <iostream>
#include <winsock2.h>
#include <string>
#include <thread>
#include <functional>
#include <vector>
#include <variant>
#include <algorithm>
#include <fstream>
#include <mutex>

using namespace std;

class Connection {
protected:
    SOCKET mySocket, acceptSocket;
    int port = 55555;
    bool online = false;
    bool ready = true; //Disabled during config
    function<void(string)> toManager; //Callback
    mutex fileMutex;

    void handleInput();
    void config();
    void sendMessage(string);

    virtual void sendConverted(char*, unsigned short) = 0;
    virtual void end() = 0;

    //Config options
    enum LogState { //Server log type
        NOLOG, //No log is kept
        TEMPORARY, //User keeps personal log (in application)
        PERSISTANT, //User keeps personal log (as text file)
        SERVER, //User retrives log from server (if applicable)
    };
    string logString() {
        switch(getLogState()) {
        case NOLOG: return "No log";
        case TEMPORARY: return "Temporary";
        case PERSISTANT: return "Text file";
        case SERVER: return "Server";
        default: return "Unknown";
        }
    }
    LogState getLogState() { return get<LogState>(options[1].second); }
    fstream logFile;
    char logName[18];
    string log = "";

    vector<pair<string, variant<string, bool, LogState>>> options; //description, value
    string getName() { return get<string>(options[0].second); }
    virtual bool configSet(short) = 0; //Derivative-specific additional config handling
    
public:
    Connection() {
        options.push_back(make_pair("Name", string("")));
        options.push_back(make_pair("Log state", TEMPORARY));
        time_t timestamp = time(nullptr);
        struct tm datetime;
        localtime_s(&datetime, &timestamp);
        strftime(logName, sizeof(logName), "%m%d%Y.%H%M.txt", &datetime);
    }

    ~Connection() {
        if(logFile.is_open()) {
            logFile.close();
        }
    }
};