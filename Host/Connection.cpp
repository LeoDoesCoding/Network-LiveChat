//Maintains connection to client

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <atlbase.h>
#include <string>
#include <thread>

#include "Connection.h"
using namespace std;

bool Connection::setup() {
    cout << "Sockets test - SERVER" << endl;

    //Settup Winsock
    WSADATA wsaData;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);
    //Check if Winsock dll found
    if (wsaerr == 0) {
        cout << "Winsock dll was found." << endl;
        cout << "Status: " << wsaData.szSystemStatus << endl;
    } else {
        cout << "The Winsock dll not found." << endl;
    }


    //Settup socket
    serverSocket = INVALID_SOCKET;
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //Check socket is setup
    if (serverSocket == INVALID_SOCKET) {
        cout << "Error at socket():" << WSAGetLastError() << endl;
        WSACleanup();
        return false;
    } else {
        cout << "Socket() is OK!" << endl;
    }

    //Bind socket to IP and port.
    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = INADDR_ANY;
    service.sin_port = htons(port);
    if (bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
        cout << "Bind() failed: " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        return false;
    } else {
        cout << "Bind() OK!" << endl;
    }

    //Setup listening process
    if (listen(serverSocket, 1) == SOCKET_ERROR) {
        cout << "Listen(): Error listening on socket" << WSAGetLastError() << endl;
        return false;
    } else {
        cout << "listen() is OK, I am waiting for connections." << endl;
    }

    //Wait until a connection is made, attempt request acceptence.
    acceptSocket = accept(serverSocket, NULL, NULL);
    if (acceptSocket == INVALID_SOCKET) {
        cout << "Accept failed: " << WSAGetLastError() << endl;
        return false;
    }

    cout << "Connection made!" << endl;

    //Connection setup complete successfully.
    online = true;
    return true;
}


//Recieving message from client (send it back)
void Connection::recieveMessage() {
    char buffer[200];

    while (online) {
        int byteCount = recv(acceptSocket, buffer, 200, 0);
        if (byteCount > 0) {
            if (strcmp(buffer, "end") == 0) {
                end();
            } else {
                cout << "Messaged recieved: " << buffer << endl;
                byteCount = send(acceptSocket, buffer, 200, 0);
            }
        } else {
            cout << "Error recieving message." << endl;
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