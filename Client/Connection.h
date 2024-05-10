#include <iostream>
#include <winsock2.h>
#include <string>

using namespace std;


class Connection {
private:
    SOCKET clientSocket, acceptSocket;
    //int port = 55555;
public:
    bool setup(const wstring IP, int port);
};
