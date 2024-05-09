#include <iostream>
#include <winsock2.h>


class Connection {
private:
    SOCKET clientSocket, acceptSocket;
    int port = 55555;
    LPCWSTR IP;
public:
    boolean setup();
};
