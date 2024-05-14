#include <iostream>
#include <winsock2.h>
#include <string>

using namespace std;


class Connection {
private:
    SOCKET serverSocket, acceptSocket;
    int port = 55555;
    bool online = false;
public:
    thread thread_obj;

    bool setup();
    void end();
    void recieveMessage();
};
