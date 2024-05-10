#include <iostream>
#include <winsock2.h>
#include <string>

using namespace std;


class Connection {
private:
    int port = 55555;
public:
    bool setup();
};
