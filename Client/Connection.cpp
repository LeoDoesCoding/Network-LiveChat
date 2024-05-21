//Maintains connection to server

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <thread>

#include "Connection.h"
using namespace std;


bool Connection::setup(const wstring IP) {
    //Settup Winsock
    WSADATA wsaData;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);

    //Check if Winsock dll found
    if (wsaerr != 0) {
        cout << "Winsock dll not found." << endl;
        return false;
    } else {
        //cout << "Winsock dll found." << endl;
    }

    //Settup socket
    clientSocket = INVALID_SOCKET;
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        cout << "Error at socket(): " << WSAGetLastError() << endl;
        return false;
    }


    //Connect to socket
    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    InetPton(AF_INET, IP.c_str(), &clientService.sin_addr.s_addr);
    clientService.sin_port = htons(port);
    if (connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
        cout << "Client: connect() - Failed to connect" << endl;
        return false;
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
    char buffer[220];
    int byteCount, bytesRecieved;

    while (online) {
        bytesRecieved = 0;
        //memset(buffer, 0, sizeof(buffer));

        //Read data up to 200. Clip rest of data (stand-in).
        byteCount = recv(clientSocket, buffer, 200, 0);
        if (byteCount > 0) {
            bytesRecieved += byteCount;
        } else {
            break;
        }


        if (byteCount > 0) {
            cout << "Byte size: " << bytesRecieved << endl;
            if (strcmp(buffer, "end") == 0) {
                end();
            } else {
                cout << buffer << endl; //Message as recieved and sent from server
            }
        } else {
            cout << "Some werd eeorrr." << endl;
        }
    }
}


void Connection::end() {
    online = false;
    WSACleanup();
    cout << "Connection ended." << endl;
    system("pause");
}