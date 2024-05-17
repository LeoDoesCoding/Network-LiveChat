//Maintains connection to server

#include <iostream>
#include <winsock2.h>
#include <string>
#include <thread>

#include "Connection.h"
using namespace std;


bool Connection::setup(const wstring IP) {
    WSADATA wsaData;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);
    if (wsaerr != 0) {
        cout << "Winsock dll not found." << endl;
        return false;
    } else {
        //cout << "Winsock dll found." << endl;
    }

    clientSocket = INVALID_SOCKET;
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        cout << "Error at socket(): " << WSAGetLastError() << endl;
        return false;
    } else {
        //cout << "Socket() is OK!" << endl;
    }

    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    InetPton(AF_INET, IP.c_str(), &clientService.sin_addr.s_addr);
    clientService.sin_port = htons(port);
    if (connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
        cout << "Client: connect() - Failed to connect" << endl;
        return false;
    } else {
        //cout << "Client connect() is OK" << endl;
    }

    //Connection setup complete successfully.
    online = true;
    return true;
}


//Sends message to sever (Called by handler.cpp).
void Connection::sendMessage(char* message) {
    int byteCount = send(clientSocket, message, 200, 0);
    if (byteCount < 0) {
        cout << "There was an issue sending the message." << endl;
        end();
    }
}

//Recieving message from server.
void Connection::recieveMessage() {
    char buffer[200];

    while (online) {
        int byteCount = recv(clientSocket, buffer, 200, 0);
        if (byteCount > 0) {
            if (strcmp(buffer, "end") == 0) {
                end();
            } else { //TODO: pass name of sender.
                cout << "Someone: " << buffer << endl; //Message as recieved and sent from server
            }
        } else {
            end();
        }
    }
}


void Connection::end() {
    online = false;
    WSACleanup();
    cout << "Connection ended." << endl;
    system("pause");
}